#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_studio_common.h>

namespace audio {

    // ----------------------------- Frame Data ------------------------------
    struct ListenerData {
        glm::vec3 position{ 0.0f };
        glm::vec3 forward{ 0.0f, 0.0f, -1.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 velocity{ 0.0f };
    };

    struct PlayRequest {
        int entityID{ -1 };
        std::string guidOrPath;   // Studio GUID ("{...}") or Core file path
        bool is3D{ true };
        bool loop{ false };
        float volume{ 1.0f };
        float pan{ 0.0f };
        float pitch{ 1.0f };
        float minDist{ 1.0f };
        float maxDist{ 100.0f };
        std::string route{ "SFX" }; // "SFX","Music","UI","VO"... (Bus/Group)
    };

    struct StopRequest {
        int entityID{ -1 };
        int fadeMs{ 0 };
    };

    enum class ParamType { Volume, Pan, Pitch, LPF, BusVolume };

    struct ParamRequest {
        int entityID{ -1 };
        ParamType type{};
        float value{ 0.0f };
    };

    struct SpatialUpdate {
        int entityID{ -1 };
        glm::vec3 position{ 0.0f };
        glm::vec3 velocity{ 0.0f };
    };

    // ----------------------------- Manager ---------------------------------
    class AudioManager {
    public:
        AudioManager() = default;
        ~AudioManager() = default;
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;

        static std::shared_ptr<AudioManager> GetInstance();

        // lifecycle
        void Initialize();
        void Shutdown();

        // per-frame
        void Update(float dt);
        void ClearFrameQueues();

        // ECS-facing push API
        void PushPlayRequest(const PlayRequest& req);
        void PushStopRequest(const StopRequest& req);
        void PushParamRequest(const ParamRequest& req);
        void PushSpatialUpdate(const SpatialUpdate& req);
        void SetListener(const ListenerData& data);

        // global controls
        void PauseAll(bool paused);
        void StopAll(int fadeMs = 0);

        // optional query (for editor UI)
        bool IsEntityPlaying(int entityID) const;

    private:


        // helpers (defined in .cpp)
        struct CoreImpl;   // FMOD Core bridge
        struct StudioImpl; // FMOD Studio bridge

        bool m_initialized{ false };
        std::unique_ptr<CoreImpl>   m_core;
        std::unique_ptr<StudioImpl> m_studio;

        // transient queues (cleared every frame)
        std::vector<PlayRequest>   m_playRequests;
        std::vector<StopRequest>   m_stopRequests;
        std::vector<ParamRequest>  m_paramRequests;
        std::vector<SpatialUpdate> m_spatialUpdates;
        ListenerData               m_listener{};
    };

} // namespace audio
