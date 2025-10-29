/********************************************************************/
/*!
\file      SceneManager.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the definitions for the `SceneManager` class,
           which handles the creation, loading, saving, clearing, and management
           of scenes within the ECS framework.
           - m_CreateNewScene: Creates a new JSON file for a scene.
           - m_LoadScene: Loads entities from a JSON file into the ECS system.
           - m_ReloadScene: Reloads all active scenes.
           - m_ClearAllScene: Clears all non-prefab scenes.
           - m_ClearScene: Removes all entities from a specified scene.
           - m_SaveScene: Saves the current state of a specified scene to a JSON file.
           - m_SaveAllActiveScenes: Saves all active scenes.
           - m_SwapScenes: Moves an entity from one scene to another.
           - GetSceneByEntityID: Finds the scene that contains a specified entity.

This file supports scene management by providing functions for creating, saving,
loading, and clearing scenes within a game, allowing dynamic control of game states.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "SceneManager.h"
#include <RAPIDJSON/document.h>
#include <RAPIDJSON/writer.h>
#include <RAPIDJSON/filewritestream.h>
#include <RAPIDJSON/stringbuffer.h>
#include "ECS/Hierachy.h"
#include "Resources/ResourceManager.h"

namespace scenes {
    std::shared_ptr<SceneManager> SceneManager::m_InstancePtr = nullptr;

    SceneManager::SceneManager()
    {
        m_ecs = ecs::ECS::GetInstance();
	}

    bool SceneManager::CreateNewScene(const std::filesystem::path& scene)
    {
        std::ifstream checkFile(scene.string());
        //check if file name exist
        if (checkFile) {
            //if file name exist
            LOGGING_WARN("JSON file already exist, select another name");
            return false;
        }
        //create a json file

        FILE* fp = std::fopen(scene.string().c_str(), "wb");

        if (fp == nullptr) {
            LOGGING_ERROR("Fail to create new Scene");
            return false; 
        }

        fprintf(fp, "[]");
        char writeBuffer[1];  // Buffer to optimize file writing
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

        std::fclose(fp);

		Serialization::SaveScene(scene.string());

        // return file path
        return true;
    }

    void SceneManager::LoadScene(const std::filesystem::path& scene)
    {
        m_loadQueue.push_back(scene);
    }

    void SceneManager::ReloadScene()
    {
		//retrieve open scenes

		std::vector<std::string> sce;
		for (auto& scenes : m_ecs->sceneMap) {
			sce.push_back(scenes.first);
		}

		//store scene path
		std::vector<std::string> scenepath;
		for (auto& scene : sce) {
			scenepath.push_back(loadScenePath.find(scene)->second.string());
		}

		//clear all scenes
		ClearAllScene();

		//load baack previous scene

		for (auto& scene : scenepath) {
			LoadScene(scene);
		}
    }

    void SceneManager::ClearAllScene(bool includePrefabs) //EXCEPT PREFABS
    {
        for (auto& [scene, data] : m_ecs->sceneMap) {

            if(includePrefabs){
                ClearScene(scene);
            }
            else{
                if(!data.isPrefab){
                    ClearScene(scene);
                }
			}
            
        }
    }

    std::vector<std::filesystem::path> SceneManager::GetAllScenesPath() {
        std::vector<std::string> sce;
        for (auto& scenes : m_ecs->sceneMap) {
            sce.push_back(scenes.first);
        }

        //store scene path
        std::vector<std::filesystem::path> scenepath;
        for (auto& scene : sce) {
            scenepath.push_back(loadScenePath.find(scene)->second.string());
        }

        return scenepath;
    }

    void SceneManager::ClearAllSceneImmediate(){

        std::vector<std::string> sce;
        for (auto& scenes : m_ecs->sceneMap) {
            sce.push_back(scenes.first);

        }

        for (auto& scenes : sce) {
            ImmediateClearScene(scenes);
        }
    }

    void SceneManager::ClearScene(const std::string& scene)
    {
        m_clearQueue.push_back(scene);
    }

    void SceneManager::SaveScene(const std::string& scene)
    {
        const auto& scenepath = loadScenePath.find(scene);
        if (scenepath != loadScenePath.end()) {
            Serialization::SaveScene(scenepath->second.string());
        }
    }

    void SceneManager::SaveAllActiveScenes(bool includeprefab)
    {
        if (m_ecs->GetState() != ecs::STOP) {
            LOGGING_WARN("Cannot save scene while in play mode");
            return;
        }

        for (auto& scenes : m_ecs->sceneMap) {
            //skip prefabs
            if (includeprefab && scenes.second.isPrefab) continue;
            SaveScene(scenes.first);
        }
    }

	void SceneManager::Update()
	{
        if (!m_clearQueue.empty()) {

			for (auto const& scene : m_clearQueue) {
				ImmediateClearScene(scene);
            }
            m_clearQueue.clear();

            //call on the asset manager garbage collection
            ResourceManager::GetInstance()->CollectGarbage();
        }

		if (!m_loadQueue.empty()) {

			for (auto const& scene : m_loadQueue) {
				ImmediateLoadScene(scene);
			}
            m_loadQueue.clear();
		}
	}

	bool SceneManager::ImmediateLoadScene(const std::filesystem::path& scene)
	{

		if (m_ecs->sceneMap.find(scene.filename().string()) != m_ecs->sceneMap.end()) {

			LOGGING_WARN("Scene already loaded");

			return false;
		}

		// Ensure the JSON file exists

		std::ifstream checkFile(scene.string());
		if (!checkFile) {
			if (!CreateNewScene(scene)) {
				LOGGING_ERROR("Fail to Create file");
				return false;
			}

		}

		//contain scene path
		loadScenePath[scene.filename().string()] = scene;


		std::string scenename = scene.filename().string();

		//create new scene
		m_ecs->sceneMap[scenename];
		//check if file is prefab or scene


		// Load entities from the JSON file
		LOGGING_INFO("Loading entities from: {}", scene.string().c_str());
		Serialization::LoadScene(scene.string());  // Load into ECS




        onSceneLoaded.Invoke(m_ecs->sceneMap.at(scenename));


		LOGGING_INFO("Entities successfully loaded!");
        return true;
	}

	void SceneManager::ImmediateClearScene(const std::string& scene)
	{
		ecs::ECS* m_ecs = ecs::ECS::GetInstance();

		size_t numberOfEntityInScene = m_ecs->sceneMap.find(scene)->second.sceneIDs.size();
		for (int n{}; n < numberOfEntityInScene; n++) {
			if (m_ecs->sceneMap.find(scene)->second.sceneIDs.size() <= 0) break;
			auto entityid = m_ecs->sceneMap.find(scene)->second.sceneIDs.begin();
			if (!ecs::Hierachy::GetParent(*entityid)) {
				m_ecs->DeleteEntity(*entityid);
			}
		}


		//remove scene from activescenes
		m_ecs->sceneMap.erase(scene);
	}


	void SceneManager::SwapScenes(const std::string& oldscene, const std::string& newscene, ecs::EntityID id)
    {
        ecs::ECS* m_ecs = ecs::ECS::GetInstance();
        std::vector<ecs::EntityID>& vectorenityid = m_ecs->sceneMap.find(oldscene)->second.sceneIDs;
        std::vector<ecs::EntityID>::iterator it = std::find(vectorenityid.begin(), vectorenityid.end(), id);
        if (it == vectorenityid.end()) {
            LOGGING_ERROR("Entity not in old scene");
            return;
        }
        
        vectorenityid.erase(it);

        m_ecs->sceneMap.find(newscene)->second.sceneIDs.push_back(id);

    }


    void SceneManager::SetSceneActive(const std::string& scene, bool active) {
        if(active){

            auto& sceneData = m_ecs->sceneMap.at(scene);
            sceneData.isActive = true;
            
            for (const auto id : sceneData.sceneIDs) {
                m_ecs->RegisterEntity(id);
            }

        }
        else {
            auto& sceneData = m_ecs->sceneMap.at(scene);
            sceneData.isActive = false;

            for (const auto id : sceneData.sceneIDs) {
                m_ecs->DeregisterEntity(id);
            }

		}
    }

    //void SceneManager::AssignEntityNewScene(const std::string& scene, m_ecs::EntityID id)
    //{
    //    m_ecs::ECS* m_ecs = m_ecs::ECS::GetInstance();
    //    //assign all of entity's scene component into new scene
    //    const auto& componentKey = m_ecs->GetComponentKeyData();
    //    for (const auto& [componentName, key] : componentKey) {
    //        if (m_ecs->GetEntitySignature(id).test(key)) {
    //            m_ecs::Component* comp = m_ecs->GetIComponent<m_ecs::Component*>(componentName, id);
    //            if (comp) {
    //                comp->scene = scene;
    //            }

    //        }
    //    }

    //    //if id has children, call recurse
    //    const auto& child = m_ecs::Hierachy::m_GetChild(id);
    //    if (child.has_value()) {
    //        for (auto id2 : child.value()) {
    //            AssignEntityNewScene(scene , id2);
    //        }
    //        
    //    }

    //}
}

