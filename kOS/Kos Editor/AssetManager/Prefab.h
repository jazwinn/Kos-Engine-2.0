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
	int m_CreatePrefab(std::string prefabscene, std::string insertscene = {});
	void m_SaveEntitytoPrefab(ecs::EntityID);
	void OverwriteScenePrefab(ecs::EntityID id);
	void UpdateAllPrefab(const std::string& prefabSceneName);
	void DeepUpdatePrefab(ecs::EntityID idA, ecs::EntityID idB);
	void OverwritePrefab_Component(ecs::EntityID id, const std::string& componentName, const std::string& prefabSceneName);
	void RevertToPrefab_Component(ecs::EntityID id, const std::string& componentName, const std::string& prefabSceneName);
	void LoadPrefab(const std::filesystem::path& filepath);
	void LoadAllPrefabs();
	ecs::ComponentSignature ComparePrefabWithInstance(ecs::EntityID id);
	void RefreshComponentDifferenceList(std::vector<std::string>& diffComp, ecs::EntityID entityID);
}
#endif PREFAB_H

