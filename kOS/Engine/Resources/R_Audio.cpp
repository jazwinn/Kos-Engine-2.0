/******************************************************************/
/*!
\file      R_Audio.h
\author    Chiu Jun Jie
\par	   junjie.c@digipen.edu
\date      Oct 03, 2025
\brief     This file contains the definition of the Audio Resource
		   class. It handles the loading and unloading of the Audio
		   through the resource manager.


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Config/pch.h"
#include "R_Audio.h"
#include <filesystem>
#include <iostream>

FMOD::System* R_Audio::s_globalCore = nullptr;
FMOD::Studio::System* R_Audio::s_globalStudio = nullptr;

void R_Audio::Load()
{
	using namespace std;
	namespace fs = std::filesystem;

	if (m_filePath.empty()) {
		std::cout << "[Audio] Empty file path.\n";
		return;
	}

	fs::path path = m_filePath;

	if (!fs::exists(path)) {
		std::cout << "[Audio] file not found: " << path.string()
			<< " | cwd=" << fs::current_path().string() << "\n";
		return;
	}

	std::string ext = path.extension().string();
	for (auto& c : ext) c = static_cast<char>(::tolower(c));

	// Studio Banks
	if (ext == ".bank") {
		if (!s_globalStudio) {
			std::cout << "[AudioStudio] No FMOD Studio system available.\n";
			return;
		}

		if (m_bank) {
			m_bank->unload();
			m_bank = nullptr;
		}

		FMOD_RESULT r = s_globalStudio->loadBankFile(
			path.string().c_str(),
			FMOD_STUDIO_LOAD_BANK_NORMAL,
			&m_bank
		);

		if (r != FMOD_OK || !m_bank) {
			std::cout << "[AudioStudio] Failed to load bank: " << path.string()
				<< " | FMOD Error: " << r << "\n";
			return;
		}

		std::cout << "[AudioStudio] Loaded bank: " << path.string() << "\n";
		return;
	}


	FMOD::System* sys = m_system ? m_system : s_globalCore;
	if (!sys) {
		return;
	}

	if (m_filePath.empty()) {
		return;
	}

	//Check if file path exist
	if (!std::filesystem::exists(m_filePath)) {
		std::cout << "[Audio] file not found: " << m_filePath.string() << "\n";
		return;
	}

	//Reset sound if have sound
	if (m_sound) { 
		m_sound->release(); 
		m_sound = nullptr; }

	unsigned int flags = FMOD_DEFAULT;
	if (m_createFlags != 0) {
		flags = m_createFlags;
	}
	FMOD_RESULT r = sys->createSound(m_filePath.string().c_str(), flags, nullptr, &m_sound);

	if (r != FMOD_OK || !m_sound) {
		r = sys->createSound(m_filePath.string().c_str(), flags | FMOD_CREATESTREAM, nullptr, &m_sound);
		if (r != FMOD_OK || !m_sound) {
			m_sound = nullptr;
			return;
		}
	}
}

void R_Audio::Unload()
{
	//Release all sounds
	if (m_sound) {
		m_sound->release();
		m_sound = nullptr;
	}

	// Release all audio bank
	if (m_bank) {
		m_bank->unload();
		m_bank = nullptr;
	}
}

//Helper for EVENT in FMOD sTUDIOS
FMOD::Studio::EventDescription* R_Audio::GetEventDescription(const std::string& eventPath)
{
	if (!s_globalStudio) return nullptr;
	FMOD::Studio::EventDescription* desc = nullptr;
	FMOD_RESULT r = s_globalStudio->getEvent(eventPath.c_str(), &desc);
	if (r != FMOD_OK || !desc) {
		std::cout << "[AudioStudio] Event not found: " << eventPath << "\n";
		return nullptr;
	}
	return desc;
}

FMOD::Studio::EventInstance* R_Audio::CreateEventInstance(const std::string& eventPath)
{
	FMOD::Studio::EventDescription* desc = GetEventDescription(eventPath);
	if (!desc) return nullptr;

	FMOD::Studio::EventInstance* inst = nullptr;
	FMOD_RESULT r = desc->createInstance(&inst);
	if (r != FMOD_OK || !inst) {
		std::cout << "[AudioStudio] Failed to create instance: " << eventPath
			<< " | FMOD Error: " << r << "\n";
		return nullptr;
	}
	return inst;
}
