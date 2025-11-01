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

void R_Prefab::DuplicatePrefabIntoScene(const std::string& scene) {
	auto* sm = ComponentRegistry::GetSceneInstance();
	sm->LoadSceneToCurrent(scene, m_filePath);

}