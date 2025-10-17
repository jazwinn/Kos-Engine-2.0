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

		/******************************************************************/
		/*!
			\fn        void m_UpdateAllPrefabEntity(std::string prefab)
			\brief     Updates all entities in the current scene that match a specified prefab.
			\param[in] prefab The name of the prefab to update entities for.
			\details   Synchronizes all instances of the prefab with the latest data from the prefab source.
		*/
		/******************************************************************/
		static void m_UpdateAllPrefabEntity(std::string prefab);

		/******************************************************************/
		/*!
			\fn        void m_UpdatePrefab(ecs::EntityID sceneprefabID, ecs::EntityID prefabid, bool isPrefabChild = false)
			\brief     Updates a specific prefab instance to match the source prefab entity data.
			\param[in] sceneprefabID  The ID of the source prefab entity.
			\param[in] prefabid       The ID of the prefab instance to update.
			\param[in] isPrefabChild  Flag indicating if the prefab instance is a child entity.
			\details   Updates the components of the instance to match the source prefab, creating or removing components as needed.
		*/
		/******************************************************************/
		static void m_UpdatePrefab(ecs::EntityID sceneprefabID, ecs::EntityID prefabid, bool isPrefabChild = false);

		/******************************************************************/
		/*!
			\fn        void m_OverWriteScenePrafab(ecs::EntityID id)
			\brief     Overwrites an existing prefab scene with a new prefab entity.
			\param[in] id The ID of the new entity to use for the prefab.
			\details   Deletes the existing prefab instance, duplicates the new entity to the scene, and saves the scene.
		*/
		/******************************************************************/
		static void m_OverWriteScenePrafab(ecs::EntityID id);

		//static void m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id);

	private:

	};
}
#endif PREFAB_H

