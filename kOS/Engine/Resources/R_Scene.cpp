#include "Config/pch.h"
#include "R_Scene.h"
#include "Scene/SceneManager.h"

void R_Scene::Load()
{
	scenes::SceneManager::m_GetInstance()->LoadScene(m_filePath);
}

void R_Scene::Unload()
{

	//scenes::SceneManager::m_GetInstance()->ClearScene(m_filePath.filename().string());

}
