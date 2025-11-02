/******************************************************************/
/*!
\file      AudioSystem.cpp
\author    Chiu Jun Jie
\par       junjie.c@digipen.edu
\date      Oct 03, 2025
\brief     This file implements the definition of the AudioSystem
           class. It handles the logic behind the Audio played in
           the engine and works with the Audio Manager interface.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Config/pch.h"
#include "AudioSystem.h"
#include "Resources/ResourceManager.h"
#include <filesystem>
#include <iostream>
#include "Utility/GUID.h"  

namespace ecs {

    ////CORE
    //FMOD::System* AudioSystem::s_fmod = nullptr;
    //bool AudioSystem::s_paused = false;

    //// STUDIO
    //FMOD::Studio::System* AudioSystem::s_studio = nullptr;
    //bool AudioSystem::s_studioReady = false;
    FMOD::System* AudioSystem::s_coreForControls = nullptr;

    void AudioSystem::Init() {
        InitCore_();
        InitStudioIfBanksExist_();
    }

    void AudioSystem::Update() {
        auto rm = ResourceManager::GetInstance();
        ECS* ecs = ECS::GetInstance();

        UpdateListener_();

        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            auto* transform = ecs->GetComponent<TransformComponent>(id);
            auto* nameComp = ecs->GetComponent<NameComponent>(id);
            auto* audioComp = ecs->GetComponent<AudioComponent>(id);

            if (!transform || !nameComp || !audioComp) continue;

            //Scene layer visbility filter
            if (!ecs->layersStack.m_layerBitSet.test(nameComp->Layer)) continue;
            if (nameComp->hide) continue;

            //Loop through all audio files
            for (auto& af : audioComp->audioFiles) {
                if (!af.requestPlay) continue;
                if (af.audioGUID.Empty()) continue;

                //Get GUID
                auto res = rm->GetResource<R_Audio>(af.audioGUID);
                if (!res) continue;

                //LOad sound
                if (!res->GetSound()) {
                    res->Load();
                }

                FMOD::Sound* sound = res->GetSound();
                FMOD::System* system = res->GetSystem();

                if (!sound || !system) continue;

                //Player sound
                FMOD::Channel* ch = nullptr;
                if (system->playSound(sound, nullptr, false, &ch) == FMOD_OK && ch) {
                    ch->setVolume(std::clamp(af.volume, 0.0f, 1.0f));

                    if (af.use3D) {
                        sound->setMode(FMOD_3D);
                        sound->set3DMinMaxDistance(af.minDistance, af.maxDistance);

                        FMOD_VECTOR pos{
                            transform->WorldTransformation.position.x,
                            transform->WorldTransformation.position.y,
                            transform->WorldTransformation.position.z
                        };
                        FMOD_VECTOR vel{ 0,0,0 };
                        ch->set3DAttributes(&pos, &vel);
                    }
                    else {
                        sound->setMode(FMOD_2D);
                        ch->setPan(std::clamp(af.pan, -1.0f, 1.0f));
                    }

                    if (af.loop) {
                        ch->setMode(FMOD_LOOP_NORMAL);
                        ch->setLoopCount(-1);
                    }
                    else {
                        ch->setMode(FMOD_LOOP_OFF);
                        ch->setLoopCount(0);
                    }

                    af.channelPtr = ch;
                }

                af.requestPlay = false;
            }

        }

        for (const EntityID id : entities) {
            auto* transform = ecs->GetComponent<TransformComponent>(id);
            auto* audioComp = ecs->GetComponent<AudioComponent>(id);
            if (!transform || !audioComp) continue;

            for (auto& af : audioComp->audioFiles) {
                if (!af.use3D || !af.channelPtr) continue;

                auto* ch = reinterpret_cast<FMOD::Channel*>(af.channelPtr);
                FMOD_VECTOR pos{
                    transform->WorldTransformation.position.x,
                    transform->WorldTransformation.position.y,
                    transform->WorldTransformation.position.z
                };
                FMOD_VECTOR vel{ 0,0,0 };
                ch->set3DAttributes(&pos, &vel);
            }
        }

        //Update sound every frame
        if (s_fmod) s_fmod->update();

        if (s_studioReady && s_studio) s_studio->update();

    }

    void AudioSystem::InitCore_() {
        if (s_fmod) {
            return;
        }

        FMOD_RESULT r = FMOD::System_Create(&s_fmod);
        if (r != FMOD_OK || !s_fmod) {
            std::cout << "[FMOD Core] Failed to create system: " << r << "\n";
            s_fmod = nullptr;

            return;
        }

        // Enable 3D right handed 
        r = s_fmod->init(64, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr);
        if (r != FMOD_OK) {
            std::cout << "[FMOD Core] init failed: " << r << "\n";
        }
        else {
            s_fmod->set3DSettings(1.0f, 1.0f, 1.0f); // 1 unit = 1m
        }

        R_Audio::SetGlobalSystem(s_fmod);

    }

    void AudioSystem::InitStudioIfBanksExist_() {
        if (s_studioReady || s_studio) return;

        // Look for banks
        const std::filesystem::path bankDir = "Assets/AudioBanks/";
        const auto master = bankDir / "Master.bank";
        const auto strings = bankDir / "Master.strings.bank";

        if (!std::filesystem::exists(master) || !std::filesystem::exists(strings)) {
            return;
        }

        // Create Studio
        FMOD_RESULT r = FMOD::Studio::System::create(&s_studio);
        if (r != FMOD_OK || !s_studio) {
            std::cout << "[FMOD Studio] Failed to create system: " << r << "\n";
            s_studio = nullptr;
            return;
        }

        // Core
        FMOD::System* core = nullptr;
        r = s_studio->getCoreSystem(&core);
        if (r == FMOD_OK && core) {
            core->set3DSettings(1.0f, 1.0f, 1.0f);
        }

        r = s_studio->initialize(
            1024,
            FMOD_STUDIO_INIT_NORMAL | FMOD_STUDIO_INIT_LIVEUPDATE,
            FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED,
            nullptr);
        if (r != FMOD_OK) {
            std::cout << "[FMOD Studio] initialize failed: " << r << "\n";
            s_studio->release(); s_studio = nullptr;
            return;
        }

        // Load master banks 
        FMOD::Studio::Bank* masterBank = nullptr;
        FMOD::Studio::Bank* stringsBank = nullptr;

        s_studio->loadBankFile(master.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
        s_studio->loadBankFile(strings.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

        s_studioReady = true;
        std::cout << "[FMOD Studio] Initialized + Master banks loaded.\n";
    }

    // pause/unpause
    void AudioSystem::SetPaused(bool paused) {
        FMOD::System* core = s_coreForControls;
        FMOD::ChannelGroup* master = nullptr;
        if (core->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->setPaused(paused);
        }
    }

    // stop all
    void AudioSystem::StopAll() {
        FMOD::System* core = s_coreForControls;
        FMOD::ChannelGroup* master = nullptr;
        if (core->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->stop();
        }
    }

    void AudioSystem::SetListenerFromCamera(const glm::vec3& pos,
        const glm::vec3& fwd,
        const glm::vec3& up)
    {
        s_camPos = pos;
        s_camFwd = glm::normalize(fwd);
        s_camUp = glm::normalize(up);
        s_camValid = true;
    }

    void AudioSystem::UpdateListener_() {
        // FETCH CAMERA POS HERE
        glm::vec3 camPos = s_camValid ? s_camPos : glm::vec3(0, 0, 0);
        glm::vec3 camFwd = s_camValid ? s_camFwd : glm::vec3(0, 0, -1);
        glm::vec3 camUp = s_camValid ? s_camUp : glm::vec3(0, 1, 0);

        // Core listener
        if (s_fmod) {
            auto toF = [](const glm::vec3& v) { return FMOD_VECTOR{ v.x, v.y, v.z }; };
            FMOD_VECTOR p = toF(camPos), v{ 0,0,0 }, f = toF(camFwd), u = toF(camUp);
            s_fmod->set3DListenerAttributes(0, &p, &v, &f, &u);
        }

        // Studio listener 
        if (s_studioReady && s_studio) {
            FMOD_3D_ATTRIBUTES lis{};
            lis.position = { camPos.x, camPos.y, camPos.z };
            lis.forward = { camFwd.x, camFwd.y, camFwd.z };
            lis.up = { camUp.x,  camUp.y,  camUp.z };
            lis.velocity = { 0,0,0 };
            s_studio->setListenerAttributes(0, &lis);
        }
    }
}