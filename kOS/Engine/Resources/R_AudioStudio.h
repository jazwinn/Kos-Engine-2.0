#pragma once
#include "Config/pch.h"
#include "Resource.h"
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod.hpp>

namespace FMOD {
    namespace Studio {
        class System;
        class Bank;
        class EventDescription;
        class EventInstance;
    }
}

class R_AudioStudio : public Resource {
public:
    using Resource::Resource;

    void Load() override;
    void Unload() override;
    ~R_AudioStudio() override { Unload(); }

    FMOD::Studio::Bank* GetBank() const { return m_bank; }
    static FMOD::Studio::System* GetStudioSystem() { return s_globalStudio; }

    static void SetGlobalStudio(FMOD::Studio::System* sys) { s_globalStudio = sys; }

    FMOD::Studio::EventDescription* GetEventDescription(const std::string& eventPath);
    FMOD::Studio::EventInstance* CreateEventInstance(const std::string& eventPath);

    REFLECTABLE(R_AudioStudio);

private:
    FMOD::Studio::Bank* m_bank = nullptr;           //!< Loaded FMOD bank
    static FMOD::Studio::System* s_globalStudio;   //!< Global Studio system reference
};
