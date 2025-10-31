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

namespace ecs {

    FMOD::System* AudioSystem::s_fmod = nullptr;
    bool AudioSystem::s_paused = false;

    void AudioSystem::Init() {
        if (!s_fmod) {
            FMOD_RESULT r = FMOD::System_Create(&s_fmod);

            //Intialize fmod
            if (r == FMOD_OK && s_fmod) {
                s_fmod->init(64, FMOD_INIT_NORMAL, nullptr);
                R_Audio::SetGlobalSystem(s_fmod);
            }
        }
    }

    // pause/unpause
    void AudioSystem::SetPaused(bool paused) {
        s_paused = paused;
        if (!s_fmod) return;
        FMOD::ChannelGroup* master = nullptr;
        if (s_fmod->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->setPaused(paused);
        }
    }

    // stop all
    void AudioSystem::StopAll() {
        if (!s_fmod) return;
        FMOD::ChannelGroup* master = nullptr;
        if (s_fmod->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->stop();
        }

    }
    void AudioSystem::Update() {
        auto rm = ResourceManager::GetInstance();
        ECS* ecs = ECS::GetInstance();
       
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
                if (af.audioGUID.empty()) continue;

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
                    ch->setPan(std::clamp(af.pan, -1.0f, 1.0f));
                   
                    if (af.loop) {
                        ch->setMode(FMOD_LOOP_NORMAL);
                        ch->setLoopCount(-1); //Infinite loop
                    }
                    else {
                        ch->setMode(FMOD_LOOP_OFF); //Oneshot
                        ch->setLoopCount(0);
                    }
                }

                //Ensure only play once per frame
                af.requestPlay = false;
            }
        }

        //Update sound every frame
        if (s_fmod) s_fmod->update();
    }
}
