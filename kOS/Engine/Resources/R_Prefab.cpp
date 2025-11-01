#include "Config/pch.h"
#include "R_Prefab.h"
#include "Config/ComponentRegistry.h"

void R_Prefab::Load()
{
	
}

void R_Prefab::Unload()
{

	//scenes::SceneManager::m_GetInstance()->ClearScene(m_filePath.filename().string());

}

int R_Prefab::DuplicatePrefabIntoScene(const std::string& scene) {
	auto* sm = ComponentRegistry::GetSceneInstance();
	sm->LoadSceneToCurrent(scene, m_filePath);
	auto* ecs = ComponentRegistry::GetECSInstance();
	const auto& sceneData = ecs->GetSceneData(scene);
	ecs::EntityID newID;

	//retrieve the new id that just spawned (the one without parent) - TODO: improve this later
	for (auto it = sceneData.sceneIDs.rbegin(); it != sceneData.sceneIDs.rend(); ++it) {
		const auto& entityID = *it;
		auto* tc = ecs->GetComponent<ecs::TransformComponent>(entityID);
		if (tc->m_haveParent == false) {
			return static_cast<int>(entityID);
		}
	}

	return -1;
}