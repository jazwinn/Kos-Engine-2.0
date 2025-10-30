#include "Config/pch.h"
#include "R_AudioStudio.h"
#include <filesystem>
#include <iostream>

FMOD::Studio::System* R_AudioStudio::s_globalStudio = nullptr;

void R_AudioStudio::Load()
{
    FMOD::Studio::System* sys = s_globalStudio;
    if (!sys) {
        std::cout << "[AudioStudio] No global FMOD Studio system set!\n";
        return;
    }

    if (m_filePath.empty()) {
        std::cout << "[AudioStudio] File path empty.\n";
        return;
    }

    if (!std::filesystem::exists(m_filePath)) {
        std::cout << "[AudioStudio] Bank not found: " << m_filePath.string() << "\n";
        return;
    }

    // Release old bank
    if (m_bank) {
        m_bank->unload();
        m_bank = nullptr;
    }

    // Load the new bank file
    FMOD_RESULT result = sys->loadBankFile(m_filePath.string().c_str(),
        FMOD_STUDIO_LOAD_BANK_NORMAL, &m_bank);

    if (result != FMOD_OK || !m_bank) {
        std::cout << "[AudioStudio] Failed to load bank: " << m_filePath.string()
            << " | FMOD Error: " << result << "\n";
        return;
    }

    std::cout << "[AudioStudio] Loaded bank: " << m_filePath.string() << "\n";
}

void R_AudioStudio::Unload()
{
    if (m_bank) {
        m_bank->unload();
        m_bank = nullptr;
        std::cout << "[AudioStudio] Bank unloaded.\n";
    }
}

FMOD::Studio::EventDescription* R_AudioStudio::GetEventDescription(const std::string& eventPath)
{
    if (!s_globalStudio) return nullptr;
    FMOD::Studio::EventDescription* desc = nullptr;
    FMOD_RESULT r = s_globalStudio->getEvent(eventPath.c_str(), &desc);
    if (r != FMOD_OK || !desc) {
        std::cout << "[AudioStudio] Failed to find event: " << eventPath << "\n";
        return nullptr;
    }
    return desc;
}

FMOD::Studio::EventInstance* R_AudioStudio::CreateEventInstance(const std::string& eventPath)
{
    FMOD::Studio::EventDescription* desc = GetEventDescription(eventPath);
    if (!desc) return nullptr;

    FMOD::Studio::EventInstance* inst = nullptr;
    FMOD_RESULT r = desc->createInstance(&inst);
    if (r != FMOD_OK || !inst) {
        std::cout << "[AudioStudio] Failed to create instance: " << eventPath << "\n";
        return nullptr;
    }
    return inst;
}