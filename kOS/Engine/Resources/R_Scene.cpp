#include "Config/pch.h"
#include "R_Scene.h"

void R_Scene::Load()
{

}

void R_Scene::Unload()
{

	//scenes::SceneManager::m_GetInstance()->ClearScene(m_filePath.filename().string());

}

//void R_Scene::LoadScene()
//{
//	scenes::SceneManager::m_GetInstance()->LoadScene(m_filePath);
//}
//
//int R_Scene::DuplicatePrefabIntoScene(const std::string& scene) {
//	if(m_filePath.empty() || scene.empty()) {
//		LOGGING_ERROR("Prefab file path is empty, cannot duplicate prefab into scene.");
//		return -1;
//	}
//
//	auto* sm = ComponentRegistry::GetSceneInstance();
//	sm->LoadSceneToCurrent(scene, m_filePath);
//	auto* ecs = ComponentRegistry::GetECSInstance();
//	const auto& sceneData = m_ecs.GetSceneData(scene);
//	ecs::EntityID newID;
//
//	//retrieve the new id that just spawned (the one without parent) - TODO: improve this later
//	for (auto it = sceneData.sceneIDs.rbegin(); it != sceneData.sceneIDs.rend(); ++it) {
//		const auto& entityID = *it;
//		auto* tc = m_ecs.GetComponent<ecs::TransformComponent>(entityID);
//		if (tc->m_haveParent == false) {
//			return static_cast<int>(entityID);
//		}
//	}
//
//	return -1;
//}
