/********************************************************************/
/*!
\file      SceneManager.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Sep 11, 2025
\brief     This file declares the definitions for the `SceneManager` class,
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

This file supports camera management by providing functions to calculate
view and projection matrices for rendering 3D scenes and UI elements.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include "Config/pch.h"
#include "DeSerialization/json_handler.h"
#include "Events/Delegate.h"
#include "SceneData.h"

namespace scenes {

	class SceneManager {
	public:
		static SceneManager* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new SceneManager{});
			}
			return m_InstancePtr.get();
		}
		void Update();
		bool CreateNewScene(const std::filesystem::path& scenepath);

		void LoadScene(const std::filesystem::path& scenepath);
		void ReloadScene();

		void ClearAllScene();
		void ClearScene(const std::string& scene);
		void SaveScene(const std::string& scene);

		std::vector<std::filesystem::path> GetAllScenesPath();
		void ClearAllSceneImmediate();

		//Do not call this in the script
		bool ImmediateLoadScene(const std::filesystem::path& scenepath);
		//Do not call this in the script
		void ImmediateClearScene(const std::string& scene);

		void SaveAllActiveScenes(bool includeprefab = false);
		void SwapScenes(const std::string& oldscene, const std::string& newscene , ecs::EntityID id);
		void AssignEntityNewScene(const std::string& scene, ecs::EntityID id);
		//EVENTS

		Delegate<const SceneData&> onSceneLoaded;




	private:



		std::vector<std::filesystem::path> m_loadQueue;
		std::vector<std::string> m_clearQueue;


		std::unordered_map<std::string, std::filesystem::path> unloadScenePath;
		std::unordered_map<std::string, std::filesystem::path> loadScenePath;
		std::vector<std::filesystem::path> m_recentFiles;

		/******************************************************************/
		/*!
		\var     static std::unique_ptr<SceneManager> m_InstancePtr
		\brief   Unique pointer to the singleton instance of SceneManager.
		*/
		/******************************************************************/
		static std::shared_ptr<SceneManager> m_InstancePtr;

	};
}
#endif SCENE_H
