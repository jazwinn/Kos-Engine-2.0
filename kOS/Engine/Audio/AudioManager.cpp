// AudioManager.cpp
#include "AudioManager.h"

#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <unordered_set>

// FMOD headers live only in the .cpp to keep the interface clean
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_studio_common.h>

namespace audio {

    // --------------------- FMOD error helper -------------------------------
#define FMOD_CHECK(x) do { FMOD_RESULT __r = (x); if (__r != FMOD_OK) { /* add your logger here */ } } while(0)

// --------------------- Small utilities ---------------------------------
    static inline bool LooksLikeGuid(const std::string& s) {
        // naive check: "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"
        return !s.empty() && s.front() == '{' && s.back() == '}' && s.size() >= 38;
    }

    // --------------------- Core (low-level) impl ----------------------------
    struct AudioManager::CoreImpl {
        FMOD::System* core{ nullptr };

        // caches
        std::unordered_map<std::string, FMOD::Sound*> soundCache; // path to sound
        std::unordered_map<int, FMOD::Channel*> channelByEntity;  // entity to channel
        std::unordered_map<std::string, FMOD::ChannelGroup*> groups; // "SFX","Music","UI","VO"

        void Initialize() {
            FMOD_CHECK(FMOD::System_Create(&core));
            unsigned int version{};
            FMOD_CHECK(core->getVersion(&version));
            // init: 1024 channels is plenty; adjust to your project
            FMOD_CHECK(core->init(1024, FMOD_INIT_3D_RIGHTHANDED, nullptr));
            // 3D defaults (meters)
            FMOD_CHECK(core->set3DSettings(1.0f, 1.0f, 1.0f));
            // create default groups lazily on demand
        }

        void Shutdown() {
            for (auto& kv : soundCache) { if (kv.second) kv.second->release(); }
            soundCache.clear();
            // groups are owned by core, no need to release explicitly
            groups.clear();
            if (core) { core->close(); core->release(); core = nullptr; }
            channelByEntity.clear();
        }

        FMOD::ChannelGroup* GetOrCreateGroup(const std::string& name) {
            auto it = groups.find(name);
            if (it != groups.end()) return it->second;
            FMOD::ChannelGroup* cg = nullptr;
            FMOD_CHECK(core->createChannelGroup(name.c_str(), &cg));
            FMOD::ChannelGroup* master = nullptr;
            FMOD_CHECK(core->getMasterChannelGroup(&master));
            if (master && cg) FMOD_CHECK(master->addGroup(cg));
            groups[name] = cg;
            return cg;
        }

        FMOD::Sound* GetOrCreateSound(const PlayRequest& r) {
            auto it = soundCache.find(r.guidOrPath);
            if (it != soundCache.end()) return it->second;

            FMOD_MODE mode = FMOD_DEFAULT;
            if (r.is3D) mode |= FMOD_3D; else mode |= FMOD_2D;
            if (r.loop) mode |= FMOD_LOOP_NORMAL; else mode |= FMOD_LOOP_OFF;

            FMOD::Sound* s = nullptr;
            FMOD_CHECK(core->createSound(r.guidOrPath.c_str(), mode, nullptr, &s));
            if (r.is3D) {
                FMOD_CHECK(s->set3DMinMaxDistance(r.minDist, r.maxDist));
            }
            soundCache[r.guidOrPath] = s;
            return s;
        }

        void ApplyChannelParams(FMOD::Channel* ch, const PlayRequest& r) {
            if (!ch) return;
            FMOD_CHECK(ch->setVolume(std::clamp(r.volume, 0.0f, 10.0f)));
            FMOD_CHECK(ch->setPitch(std::max(0.01f, r.pitch)));
            FMOD_CHECK(ch->setPan(std::clamp(r.pan, -1.0f, 1.0f)));
            if (r.is3D) {
                // min/max already on sound; spatial attrs come from SpatialUpdate each frame
            }
        }

        void Set3DAttributes(FMOD::Channel* ch, const SpatialUpdate& s) {
            if (!ch) return;
            FMOD_VECTOR pos{ s.position.x, s.position.y, s.position.z };
            FMOD_VECTOR vel{ s.velocity.x, s.velocity.y, s.velocity.z };
            FMOD_CHECK(ch->set3DAttributes(&pos, &vel));
        }

        void SetListener(const ListenerData& L) {
            FMOD_VECTOR p{ L.position.x, L.position.y, L.position.z };
            FMOD_VECTOR v{ L.velocity.x, L.velocity.y, L.velocity.z };
            FMOD_VECTOR f{ L.forward.x,  L.forward.y,  L.forward.z };
            FMOD_VECTOR u{ L.up.x,       L.up.y,       L.up.z };
            FMOD_CHECK(core->set3DListenerAttributes(0, &p, &v, &f, &u));
        }

        void StopEntity(int entityID, int fadeMs) {
            auto it = channelByEntity.find(entityID);
            if (it == channelByEntity.end() || !it->second) return;
            FMOD::Channel* ch = it->second;
            if (fadeMs > 0) {
                // simple fade via volume ramp; in production you'd schedule DSP or timeline fades
                float v = 0.0f;
                FMOD_CHECK(ch->getVolume(&v));
                const float step = v / std::max(1, fadeMs);
                // NOTE: For brevity we don't implement per-sample ramp here; consider moving fades to Update().
            }
            FMOD_CHECK(ch->stop());
            it->second = nullptr;
            channelByEntity.erase(it);
        }

        bool IsPlaying(int entityID) const {
            auto it = channelByEntity.find(entityID);
            if (it == channelByEntity.end() || !it->second) return false;
            bool playing = false;
            it->second->isPlaying(&playing);
            return playing;
        }
    };

    // --------------------- Studio (high-level) impl -------------------------
    struct AudioManager::StudioImpl {
        FMOD::Studio::System* studio{ nullptr };
        // caches
        std::unordered_map<std::string, FMOD::Studio::EventDescription*> eventCache; // GUID to desc
        std::unordered_map<int, FMOD::Studio::EventInstance*> instanceByEntity;      // entity to inst
        std::unordered_map<std::string, FMOD::Studio::Bus*> buses;                   // "SFX" to bus:/SFX
        bool banksLoaded{ false };

        void Initialize(FMOD::System* core) {
            FMOD_CHECK(FMOD::Studio::System::create(&studio));
            FMOD_CHECK(studio->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, nullptr));
            // Studio will use the same underlying core system
            // (FMOD automatically links Studio?Core inside initialize)
            // Optional: load banks here if you have a fixed path. Left as lazy/no-op for safety.
        }

        void Shutdown() {
            for (auto& kv : instanceByEntity) {
                if (kv.second) { kv.second->stop(FMOD_STUDIO_STOP_IMMEDIATE); kv.second->release(); }
            }
            instanceByEntity.clear();
            eventCache.clear(); // descriptions owned by studio bank system
            buses.clear();
            if (studio) { studio->unloadAll(); studio->release(); studio = nullptr; }
        }

        FMOD::Studio::Bus* GetOrCreateBus(const std::string& simple) {
            auto it = buses.find(simple);
            if (it != buses.end()) return it->second;
            std::string path = "bus:/" + simple; // convention: "SFX" to "bus:/SFX"
            FMOD::Studio::Bus* b = nullptr;
            if (studio->getBus(path.c_str(), &b) == FMOD_OK && b) {
                buses[simple] = b;
                return b;
            }
            // fallback: master
            if (studio->getBus("bus:/", &b) == FMOD_OK && b) {
                buses[simple] = b;
                return b;
            }
            return nullptr;
        }

        FMOD::Studio::EventDescription* GetOrCreateEventDesc(const std::string& guid) {
            auto it = eventCache.find(guid);
            if (it != eventCache.end()) return it->second;
            FMOD_GUID g{};
            if (FMOD::Studio::parseID(guid.c_str(), &g) != FMOD_OK) return nullptr;
            FMOD::Studio::EventDescription* desc = nullptr;
            if (studio->getEventByID(&g, &desc) != FMOD_OK) return nullptr;
            eventCache[guid] = desc;
            return desc;
        }

        FMOD::Studio::EventInstance* GetOrCreateInstance(const PlayRequest& r) {
            auto it = instanceByEntity.find(r.entityID);
            if (it != instanceByEntity.end() && it->second) return it->second;

            FMOD::Studio::EventDescription* desc = GetOrCreateEventDesc(r.guidOrPath);
            if (!desc) return nullptr;
            FMOD::Studio::EventInstance* inst = nullptr;
            if (desc->createInstance(&inst) != FMOD_OK || !inst) return nullptr;

            // route to bus by setting an assigned bus volume? In Studio, routing is defined by the event/bus graph;
            // you usually control **bus volume** instead. We'll keep bus lookup for global controls.
            instanceByEntity[r.entityID] = inst;
            return inst;
        }

        void ApplyParams(FMOD::Studio::EventInstance* inst, const PlayRequest& r) {
            if (!inst) return;
            // Studio doesn’t have direct "pan"; you’d expose parameters in the event, or use 3D panning.
            inst->setPitch(std::max(0.01f, r.pitch));
            inst->setVolume(std::clamp(r.volume, 0.0f, 10.0f));
            // 3D attributes if 3D event
            if (r.is3D) {
                // actual 3D goes via Set3DAttributes when SpatialUpdate arrives
            }
        }

        void Set3DAttributes(FMOD::Studio::EventInstance* inst, const SpatialUpdate& s) {
            if (!inst) return;
            FMOD_3D_ATTRIBUTES a{};
            a.position = { s.position.x, s.position.y, s.position.z };
            a.velocity = { s.velocity.x, s.velocity.y, s.velocity.z };
            a.forward = { 0,0,-1 }; // not strictly needed for point sources
            a.up = { 0,1, 0 };
            inst->set3DAttributes(&a);
        }

        void SetListener(const ListenerData& L) {
            FMOD_3D_ATTRIBUTES a{};
            a.position = { L.position.x, L.position.y, L.position.z };
            a.velocity = { L.velocity.x, L.velocity.y, L.velocity.z };
            a.forward = { L.forward.x,  L.forward.y,  L.forward.z };
            a.up = { L.up.x,       L.up.y,       L.up.z };
            studio->setListenerAttributes(0, &a);
        }

        void StopEntity(int entityID, int fadeMs) {
            auto it = instanceByEntity.find(entityID);
            if (it == instanceByEntity.end() || !it->second) return;
            it->second->stop(fadeMs > 0 ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
            it->second->release();
            instanceByEntity.erase(it);
        }

        bool IsPlaying(int entityID) const {
            auto it = instanceByEntity.find(entityID);
            if (it == instanceByEntity.end() || !it->second) return false;
            FMOD_STUDIO_PLAYBACK_STATE st{};
            if (it->second->getPlaybackState(&st) != FMOD_OK) return false;
            return st == FMOD_STUDIO_PLAYBACK_PLAYING || st == FMOD_STUDIO_PLAYBACK_STARTING;
        }
    };

    // --------------------- AudioManager public API --------------------------
    std::shared_ptr<AudioManager> AudioManager::GetInstance() {
        static std::shared_ptr<AudioManager> s = std::make_shared<AudioManager>();
        return s;
    }

    void AudioManager::Initialize() {
        if (m_initialized) return;
        m_core = std::make_unique<CoreImpl>();
        m_core->Initialize();

        m_studio = std::make_unique<StudioImpl>();
        m_studio->Initialize(m_core->core);

        m_initialized = true;
    }

    void AudioManager::Shutdown() {
        if (!m_initialized) return;
        if (m_studio) { m_studio->Shutdown(); m_studio.reset(); }
        if (m_core) { m_core->Shutdown();   m_core.reset(); }
        m_initialized = false;
    }

    void AudioManager::PushPlayRequest(const PlayRequest& req) { m_playRequests.emplace_back(req); }
    void AudioManager::PushStopRequest(const StopRequest& req) { m_stopRequests.emplace_back(req); }
    void AudioManager::PushParamRequest(const ParamRequest& req) { m_paramRequests.emplace_back(req); }
    void AudioManager::PushSpatialUpdate(const SpatialUpdate& r) { m_spatialUpdates.emplace_back(r); }
    void AudioManager::SetListener(const ListenerData& d) { m_listener = d; }

    void AudioManager::PauseAll(bool paused) {
        if (!m_initialized) return;
        // Core: pause master
        FMOD::ChannelGroup* master{};
        if (m_core && m_core->core) {
            FMOD_CHECK(m_core->core->getMasterChannelGroup(&master));
            if (master) FMOD_CHECK(master->setPaused(paused));
        }
        // Studio: pause master bus
        if (m_studio && m_studio->studio) {
            FMOD::Studio::Bus* bus{};
            if (m_studio->studio->getBus("bus:/", &bus) == FMOD_OK && bus) {
                bus->setPaused(paused);
            }
        }
    }

    void AudioManager::StopAll(int fadeMs) {
        if (!m_initialized) return;
        // Core
        if (m_core) {
            for (auto& kv : m_core->channelByEntity) {
                if (kv.second) FMOD_CHECK(kv.second->stop());
            }
            m_core->channelByEntity.clear();
        }
        // Studio
        if (m_studio) {
            for (auto& kv : m_studio->instanceByEntity) {
                if (kv.second) kv.second->stop(fadeMs > 0 ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
            }
            m_studio->instanceByEntity.clear();
        }
    }

    // --------------------- Per-frame Update ---------------------------------
    void AudioManager::Update(float /*dt*/) {
        if (!m_initialized) return;

        // 1) Listener
        if (m_core)   m_core->SetListener(m_listener);
        if (m_studio) m_studio->SetListener(m_listener);

        // 2) PLAY
        for (const auto& r : m_playRequests) {
            const bool studioEvent = LooksLikeGuid(r.guidOrPath) && (m_studio && m_studio->studio);

            if (studioEvent) {
                // Studio path
                auto* inst = m_studio->GetOrCreateInstance(r);
                if (!inst) continue;
                m_studio->ApplyParams(inst, r);
                inst->start();
            }
            else {
                // Core path
                FMOD::Sound* s = m_core->GetOrCreateSound(r);
                if (!s) continue;

                FMOD::Channel* ch = nullptr;
                FMOD_CHECK(m_core->core->playSound(s, nullptr, true, &ch)); // start paused so we can route/params
                if (!ch) continue;

                // route
                if (!r.route.empty()) {
                    FMOD::ChannelGroup* g = m_core->GetOrCreateGroup(r.route);
                    if (g) FMOD_CHECK(ch->setChannelGroup(g));
                }

                // params
                m_core->ApplyChannelParams(ch, r);

                // store mapping and unpause
                m_core->channelByEntity[r.entityID] = ch;
                FMOD_CHECK(ch->setPaused(false));
            }
        }

        // 3) PARAMS
        for (const auto& p : m_paramRequests) {
            // Core
            if (m_core) {
                auto it = m_core->channelByEntity.find(p.entityID);
                if (it != m_core->channelByEntity.end() && it->second) {
                    switch (p.type) {
                    case ParamType::Volume: FMOD_CHECK(it->second->setVolume(std::clamp(p.value, 0.0f, 10.0f))); break;
                    case ParamType::Pan:    FMOD_CHECK(it->second->setPan(std::clamp(p.value, -1.0f, 1.0f)));   break;
                    case ParamType::Pitch:  FMOD_CHECK(it->second->setPitch(std::max(0.01f, p.value)));        break;
                    case ParamType::LPF:    /* expose via DSP chain if needed */                               break;
                    case ParamType::BusVolume: /* set group volume */ {
                        // if you want: find a group name from elsewhere and set; omitted here
                    } break;
                    }
                }
            }
            // Studio: typically parameters are custom event params; omitted here unless you expose names/IDs
        }

        // 4) SPATIAL
        for (const auto& s : m_spatialUpdates) {
            if (m_core) {
                auto it = m_core->channelByEntity.find(s.entityID);
                if (it != m_core->channelByEntity.end() && it->second) {
                    m_core->Set3DAttributes(it->second, s);
                }
            }
            if (m_studio) {
                auto it = m_studio->instanceByEntity.find(s.entityID);
                if (it != m_studio->instanceByEntity.end() && it->second) {
                    m_studio->Set3DAttributes(it->second, s);
                }
            }
        }

        // 5) STOP
        for (const auto& s : m_stopRequests) {
            if (m_studio) m_studio->StopEntity(s.entityID, s.fadeMs);
            if (m_core)   m_core->StopEntity(s.entityID, s.fadeMs);
        }

        // 6) Pump FMOD
        if (m_studio && m_studio->studio) m_studio->studio->update();
        if (m_core && m_core->core)     m_core->core->update();
    }

    void AudioManager::ClearFrameQueues() {
        m_playRequests.clear();
        m_stopRequests.clear();
        m_paramRequests.clear();
        m_spatialUpdates.clear();
    }

    bool AudioManager::IsEntityPlaying(int entityID) const {
        bool corePlaying = false, studioPlaying = false;
        if (m_core)   corePlaying = m_core->IsPlaying(entityID);
        if (m_studio) studioPlaying = m_studio->IsPlaying(entityID);
        return corePlaying || studioPlaying;
    }

} // namespace audio
