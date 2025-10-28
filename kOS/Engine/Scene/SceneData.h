#pragma once
#include "Config/pch.h"
#include "ECS/ECSList.h"

class SceneData
{
public:
	std::vector<std::string> skyBoxGUID;
	float ambientIntensity{ 1.0f };
	
	
	REFLECTABLE(SceneData, skyBoxGUID, ambientIntensity);


public:
	bool isPrefab{ false };
	bool isActive{ true };
	ecs::EntityID prefabID = 0;
	std::vector<ecs::EntityID> sceneIDs;
};