#pragma once

#include "ScriptAdapter/TemplateSC.h"

class AudioScript : public TemplateSC {
public:

    std::string GUID;

    void Start() override {

        //Audio example
        //Get audio component
        auto* ac = ecsPtr->GetComponent<ecs::AudioComponent>(entity);
        if (!ac || ac->audioFiles.empty()) return;

        //Play the audio that say playonstart if not go to the first audio
        for (auto& af : ac->audioFiles) {
            if (af.playOnStart && !af.audioGUID.empty()) {
                GUID = af.audioGUID;
                af.requestPlay = true;
                return; 
            }
        }
        
    }

    void Update() override {

    }

    REFLECTABLE(AudioScript,GUID);
};
