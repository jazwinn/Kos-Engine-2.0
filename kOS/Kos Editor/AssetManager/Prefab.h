/******************************************************************/
/*!
\file      Prefab.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file the class prefabs that read an json file and 
		   stores each object into the prefabs. When the function
		   is called. The prefab, would create and clone its data
		   onto the entity


Copyright (C) 2024 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef PREFAB_H
#define PREFAB_H

#include "ECS/ECSList.h"

namespace prefab {

	class Prefab {

	public:

		/******************************************************************/
		/*!
			\fn        int m_CreatePrefab(std::string prefabscene, std::string insertscene = {})
			\brief     Creates a prefab in the specified scene by duplicating entities from an existing scene.
			\param[in] prefabscene  The name of the scene from which to create the prefab.
			\param[in] insertscene  The name of the scene where the prefab will be inserted. If empty, uses the first scene in the ECS scene map.
			\return    The ID of the created prefab entity or -1 if an error occurs.
		*/
		/******************************************************************/
		static int m_CreatePrefab(std::string prefabscene, std::string insertscene = {});

		/******************************************************************/
		/*!
			\fn        void m_SaveEntitytoPrefab(ecs::EntityID id)
			\brief     Saves an entity and its components as a new prefab, creating a JSON file for the prefab data.
			\param[in] id  The ID of the entity to save as a prefab.
			\details   Serializes the entity and its hierarchy into a JSON file and loads it as a prefab scene.
		*/
		/******************************************************************/
		static void m_SaveEntitytoPrefab(ecs::EntityID);


		//A being the true prefab
		static void UpdateAllPrefab(const std::string& prefabSceneName);
		static void DeepUpdatePrefab(ecs::EntityID idA , ecs::EntityID idB);



		//static void m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id);
		/******************************************************************/
		/*!
			\fn        void LoadAllPrefabs()
			\brief     Loads All Prefab files stored in Editor Prefab Folders into memory
		*/
		/******************************************************************/
		static void OverwritePrefab_Component(ecs::EntityID id, const std::string& componentName, const std::string& prefabSceneName);
		static void RevertToPrefab_Component(ecs::EntityID id, const std::string& componentName, const std::string& prefabSceneName);
		static void LoadAllPrefabs();
		static ecs::ComponentSignature ComparePrefabWithInstance(ecs::EntityID id);
		static void RefreshComponentDifferenceList(std::vector<std::string>& diffComp, ecs::EntityID entityID);

	private:

	};
}
#endif PREFAB_H

