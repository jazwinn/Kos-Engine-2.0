/******************************************************************/
/*!
\file      R_Audio.h
\author    Chiu Jun Jie
\par
\date      Oct 03, 2025
\brief     This file contains the definition of the Audio Resource
           class. It handles the loading and unloading of the Audio
           through the resource manager.


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once
#include "Config/pch.h"
#include "Resource.h"
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>

namespace FMOD {
    class System;
    class Sound;

    namespace Studio {
        class System;
        class Bank;
        class EventDescription;
        class EventInstance;
    }
}

class R_Audio : public Resource {
public:
    using Resource::Resource;

    void Load() override;
    void Unload() override;

    ~R_Audio() override { Unload(); }
    
    //Core
    FMOD::Sound* GetSound()  const { return m_sound; }
    FMOD::System* GetSystem() const { return m_system ? m_system : s_globalCore; }
    void SetSystem(FMOD::System* sys) { m_system = sys; }
    static void SetGlobalSystem(FMOD::System* sys) { s_globalCore = sys; }

    // studio
    static void SetGlobalStudio(FMOD::Studio::System* sys) { s_globalStudio = sys; }
    static FMOD::Studio::System* GetStudioSystem() { return s_globalStudio; }

    // studio audio bank
    FMOD::Studio::Bank* GetBank() const { return m_bank; }
    FMOD::Studio::EventDescription* GetEventDescription(const std::string& eventPath);
    FMOD::Studio::EventInstance* CreateEventInstance(const std::string& eventPath);

    REFLECTABLE(R_Audio);

private:
    // Core
    FMOD::System* m_system = nullptr;
    FMOD::Sound* m_sound = nullptr;
    unsigned int  m_createFlags = 0;
    static FMOD::System* s_globalCore;

    // Studio
    FMOD::Studio::Bank* m_bank = nullptr;
    static FMOD::Studio::System* s_globalStudio;
};
