/******************************************************************/
/*!
\file      RenderSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderSystem
		   class. It passes the transform data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "ECS/ECS.h"

#include "RenderSystem.h"
#include "ECS/Component/SpriteComponent.h"
#include "Resources/ResourceManager.h"

namespace ecs {



	void RenderSystem::Init()
	{
		
	}

	void RenderSystem::Update(const std::string& scene)
	{
		ECS* ecs = ECS::GetInstance();
		const auto& entities = m_entities.Data();

		for (const EntityID id : entities) {
			TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
			NameComponent* NameComp = ecs->GetComponent<NameComponent>(id);
			SpriteComponent* sprite = ecs->GetComponent<SpriteComponent>(id);

			//skip component not of the scene
			if ((sprite->scene != scene) || !ecs->layersStack.m_layerBitSet.test(NameComp->Layer) || NameComp->hide) continue;
		






		}



	}


}

