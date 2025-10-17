#pragma once
#include "Config/pch.h"
#include "Scripting/ScriptManager.h"
#include "Scene/SceneManager.h"
#include "Configs/ConfigPath.h"
#include <filesystem>


inline void ScriptHotReload() {
	
	auto* sm = ScriptManager::m_GetInstance;
    //save all active scenes
    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
    scenemanager->SaveAllActiveScenes();

    auto scenelist = scenemanager->GetAllScenesPath();

    //CLEAR ALL SCENES BEFORE RELOADING DLL, ELSE CRASH
    scenemanager->ClearAllSceneImmediate();



    //Unload the DLL
    ScriptManager::m_GetInstance()->UnloadDLL();


    Sleep(1000);

    std::filesystem::path source = std::filesystem::absolute(configpath::scriptWatherFilePath);

    // Get parent directory of the current folder (up one level)
    std::filesystem::path targetDir = source.parent_path().parent_path(); // moves up one directory
    std::filesystem::path target = targetDir / "script.dll";

    try {
        std::filesystem::copy_file(source, target, std::filesystem::copy_options::overwrite_existing);
        std::cout << "Copied to: " << target << std::endl;
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
    }


    //load the DLL
    ScriptManager::m_GetInstance()->RunDLL();

    for (const auto& scenepath : scenelist) {
        scenemanager->LoadScene(scenepath);
    }

}