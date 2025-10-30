#include "Config/pch.h"
#include "AudioSystemStudio.h"
#include "Resources/ResourceManager.h"
#include <filesystem>
#include <iostream>

namespace ecs {

    FMOD::Studio::System* AudioSystemStudio::s_studio = nullptr;
    FMOD::System* AudioSystemStudio::s_core = nullptr;
    bool                  AudioSystemStudio::s_initialized = false;
    bool                  AudioSystemStudio::s_paused = false;

    void AudioSystemStudio::Init() {
        if (s_initialized) return;

        // Create FMOD Studio system
        FMOD_RESULT result = FMOD::Studio::System::create(&s_studio);
        if (result != FMOD_OK || !s_studio) {
            std::cout << "[FMOD Studio] Failed to create system: " << result << "\n";
            return;
        }

        // Get Core 
        result = s_studio->getCoreSystem(&s_core);
        if (result != FMOD_OK || !s_core) {
            std::cout << "[FMOD Studio] Failed to get core system: " << result << "\n";
            return;
        }

        // Initialize Studio
        result = s_studio->initialize(
            1024,
            FMOD_STUDIO_INIT_NORMAL | FMOD_STUDIO_INIT_LIVEUPDATE,
            FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED,           
            nullptr);

        if (result != FMOD_OK) {
            std::cout << "[FMOD Studio] Initialization failed: " << result << "\n";
            return;
        }

        // 3D scaling (1.0f = 1m)
        s_core->set3DSettings(1.0f, 1.0f, 1.0f);

        // Load Master bank
        std::filesystem::path bankPath = "Assets/AudioBanks/";
        std::filesystem::path masterBank = bankPath / "Master.bank";
        std::filesystem::path stringsBank = bankPath / "Master.strings.bank";

        if (std::filesystem::exists(masterBank)) {
            FMOD::Studio::Bank* master = nullptr;
            s_studio->loadBankFile(masterBank.string().c_str(),
                FMOD_STUDIO_LOAD_BANK_NORMAL, &master);
        }

        if (std::filesystem::exists(stringsBank)) {
            FMOD::Studio::Bank* strings = nullptr;
            s_studio->loadBankFile(stringsBank.string().c_str(),
                FMOD_STUDIO_LOAD_BANK_NORMAL, &strings);
        }

        s_initialized = true;
        std::cout << "[FMOD Studio] Initialized successfully.\n";
    }

    void AudioSystemStudio::Update(const std::string& scene) {
        if (!s_initialized || !s_studio) return;

        //PUT STUDIO EVENT HERE?
        s_studio->update();
    }

    void AudioSystemStudio::SetPaused(bool paused) {
        s_paused = paused;
        if (!s_core) return;

        FMOD::ChannelGroup* master = nullptr;
        if (s_core->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->setPaused(paused);
        }
    }

    void AudioSystemStudio::StopAll() {
        if (!s_core) return;
        FMOD::ChannelGroup* master = nullptr;
        if (s_core->getMasterChannelGroup(&master) == FMOD_OK && master) {
            master->stop();
        }
    }

    void AudioSystemStudio::Shutdown() {
        if (!s_studio) return;

        s_studio->unloadAll();
        s_studio->release();
        s_studio = nullptr;
        s_core = nullptr;
        s_initialized = false;

        std::cout << "[FMOD Studio] Shutdown complete.\n";
    }
}