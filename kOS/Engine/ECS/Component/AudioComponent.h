#pragma once
/******************************************************************/
/*!
\file      AudioComponent.h
\author    Chiu Jun Jie
\par       junjie.c@digipen.edu
\date      Oct 03, 2025
\brief     This file contains the audio data


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef AUDIOCOM_H
#define AUDIOCOM_H

#include "Component.h"

namespace ecs {

    struct AudioFile {

        bool loop{ false };
        bool playOnStart{ false };
        bool hasPlayed{ false };
        bool isBGM{ false };
        bool isSFX{ true };
        bool hasChanged = false;
        bool lastLoopState{ false };
        float volume{ 1.0f };
        float pan{ 0.0f };
        float lastVolume{ -1.0f };
        std::string name;

        bool requestPlay{ false };

        utility::GUID audioGUID{};

        bool  use3D{ false };        // if true  use 3D spatialization instead of pan
        float minDistance{ 1.0f };   // full volume radius
        float maxDistance{ 30.0f };  // silent distance
        void* channelPtr{ nullptr };

        REFLECTABLE(AudioFile,
            audioGUID, volume, loop, playOnStart, hasPlayed, pan,
            isBGM, isSFX, use3D, minDistance, maxDistance
        )
    };

    struct StudioEventRef {
        std::string eventPath;
        bool  playOnStart{ false };
        bool  loop{ false };
        bool  requestPlay{ false };
        bool  is3D{ true };
        float volume{ 1.0f };

        void* instancePtr{ nullptr };

        REFLECTABLE(StudioEventRef,
            eventPath, playOnStart, loop, is3D, volume
        )
    };

    class AudioComponent : public Component {
    public:
        // Core
        std::vector<AudioFile>      audioFiles;
        // Studio event
        std::vector<StudioEventRef> studioEvents;

        REFLECTABLE(AudioComponent, audioFiles, studioEvents)
    };
}
#endif AUDIOCOM_H