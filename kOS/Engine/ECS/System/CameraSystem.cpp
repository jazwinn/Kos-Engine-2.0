/******************************************************************/
/*!
\file      CameraSystem.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Oct 02, 2025
\brief     This file contains the definition of the Camera Sytem
		   class. It passes the debauging data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Config/pch.h"
#include "ECS/ECS.h"

#include "CameraSystem.h"
#include "Graphics/GraphicsManager.h"


namespace ecs {

	void CameraSystem::Init() {
		

	}

	void CameraSystem::Update(const std::string& scene) {

		ECS* ecs = ECS::GetInstance();

		const auto& entities = m_entities.Data();


		for (const EntityID id : entities) {
			TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
			NameComponent* NameComp = ecs->GetComponent<NameComponent>(id);
			CameraComponent* camera = ecs->GetComponent<CameraComponent>(id);

			//skip component not of the scene
			if ((transform->scene != scene) || !ecs->layersStack.m_layerBitSet.test(NameComp->Layer) || NameComp->hide) continue;
			
			GraphicsManager::GetInstance()->gm_PushGameCameraData(CameraData{ camera->fov, camera->nearPlane, camera->farPlane,
																			camera->size, transform->WorldTransformation.position,transform->LocalTransformation.rotation,
																			camera->target, camera->active });

		}

		
	}


}
