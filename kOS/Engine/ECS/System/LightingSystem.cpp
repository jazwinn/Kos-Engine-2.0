/********************************************************************/
/*!
\file      LightingSystem.cpp
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Defines the LightingSystem class, which manages light
		   components and updates lighting data in the ECS system.

		   This system:
		   - Updates and uploads light data (directional, point,
			 and spot lights) to GPU shaders.
		   - Interfaces with deferred and forward rendering passes.
		   - Ensures lighting information remains synchronized
			 across the rendering pipeline.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "ECS/ECS.h"

#include "LightingSystem.h"
#include "ECS/Component/LightComponent.h"
#include "Graphics/GraphicsManager.h"

namespace ecs {



	void LightingSystem::Init()
	{

	}

	void LightingSystem::Update()
	{
		ECS* ecs = ECS::GetInstance();
		const auto& entities = m_entities.Data();
		std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();

		for (const EntityID id : entities) {
			TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
			NameComponent* nameComp = ecs->GetComponent<NameComponent>(id);
			LightComponent* light = ecs->GetComponent<LightComponent>(id);

			//skip component not of the scene
			if (!ecs->layersStack.m_layerBitSet.test(nameComp->Layer) || nameComp->hide) continue;

			switch (light->lightType)
			{
			case LightComponent::LightType::POINTLIGHT:
				gm->gm_PushPointLightData(PointLightData{ transform->LocalTransformation.position, light->color, light->diffuseStrength,
														light->specularStrength,light->linear,light->quadratic,light->intesnity,light->shadowCast });
				break;
			case LightComponent::LightType::DIRECTIONAL:
				gm->gm_PushDirectionalLightData(DirectionalLightData{ transform->LocalTransformation.position, light->color, light->diffuseStrength,
														light->specularStrength,light->linear,light->quadratic, light->intesnity,light->direction });
				break;
			case LightComponent::LightType::SPOTLIGHT:
				gm->gm_PushSpotLightData(SpotLightData{ transform->LocalTransformation.position, light->color, light->diffuseStrength,
														light->specularStrength,light->linear,light->quadratic,light->intesnity,light->direction, light->cutOff,
														light->outerCutOff});
				break;
			default:
				break;
			}

		}

	}


}

