/******************************************************************/
/*!
\file      TransformSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Sept 29, 2024
\brief   



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "ECS/ECS.h"
#include "ECS/Hierachy.h"
#include "TransformSystem.h"
#include "Utility/MathUtility.h"

namespace ecs {
	
	void TransformSystem::Init(){
		//onRegister.Add([](EntityID id) {
		//	ECS* ecs = ECS::GetInstance();
		//	TransformComponent* transformComp = ecs->GetComponent<TransformComponent>(id);
		//	std::cout << "test add: " << id << std::endl;

		//});
		//onDeregister.Add([](EntityID id) {
		//	ECS* ecs = ECS::GetInstance();
		//	TransformComponent* transformComp = ecs->GetComponent<TransformComponent>(id);
		//	std::cout << "test add: " << id << std::endl;

		//	});

	}

	void TransformSystem::Update() {
		ECS* ecs = ECS::GetInstance();
		const auto& entities = m_entities.Data();
		for (const EntityID id : entities) {
			TransformComponent* transformComp = ecs->GetComponent<TransformComponent>(id);
			if (transformComp->m_haveParent) continue;

			NameComponent* NameComp = ecs->GetComponent<NameComponent>(id);
			if (!ecs->layersStack.m_layerBitSet.test(NameComp->Layer) || NameComp->hide) continue;
			
			CalculateAllTransform(transformComp);
		}
	}

	void TransformSystem::CalculateAllTransform(TransformComponent* transformComp, const glm::mat4& parentWorldMtx) {
		if (!transformComp) return;

		CalculateLocalTransformMtx(transformComp);
		transformComp->transformation = transformComp->m_haveParent ? parentWorldMtx * transformComp->localTransform : transformComp->localTransform;
		math::DecomposeMtxIntoTRS(transformComp->transformation, transformComp->WorldTransformation.position, transformComp->WorldTransformation.rotation, transformComp->WorldTransformation.scale);
		for (const EntityID childID : transformComp->m_childID) {
			TransformComponent* child = ECS::GetInstance()->GetComponent<TransformComponent>(childID);
			if (child) {
				CalculateAllTransform(child, transformComp->transformation);
			}
		}
	}

	// Ideally not great to constantly convert to radians, data should be stored in radians but due to the restriction of ImGui might be tough to show data in deg
	// Unless TransformComponent is customised
	void TransformSystem::CalculateLocalTransformMtx(TransformComponent* transformComp) {
		if (!transformComp) return;
		constexpr glm::mat4 identity(1.0f);
		transformComp->localTransform = glm::translate(identity, transformComp->LocalTransformation.position) *
										glm::mat4_cast(glm::quat(glm::radians(transformComp->LocalTransformation.rotation))) *
										glm::scale(identity, transformComp->LocalTransformation.scale);
	}

	void TransformSystem::SetImmediateWorldPosition(TransformComponent* transformComp, glm::vec3&& pos){
		if (!transformComp) return;
		constexpr glm::mat4 identity(1.0f);
		transformComp->WorldTransformation.position = pos;
		transformComp->transformation = glm::translate(identity, transformComp->WorldTransformation.position) *
										glm::mat4_cast(glm::quat(glm::radians(transformComp->WorldTransformation.rotation))) *
										glm::scale(identity, transformComp->WorldTransformation.scale);

		TransformComponent* parentTrans = ECS::GetInstance()->GetComponent<TransformComponent>(transformComp->m_parentID);
		if (parentTrans) {
			transformComp->localTransform = glm::inverse(parentTrans->transformation) * transformComp->transformation;
			math::DecomposeMtxIntoTRS(transformComp->localTransform, transformComp->LocalTransformation.position, transformComp->LocalTransformation.rotation, transformComp->LocalTransformation.scale);
		}
		else {
			transformComp->LocalTransformation.position = transformComp->WorldTransformation.position;
			transformComp->localTransform = transformComp->transformation;
		}
	}

	void TransformSystem::SetImmediateWorldRotation(TransformComponent* transformComp, glm::vec3&& rot){
		if (!transformComp) return;
		constexpr glm::mat4 identity(1.0f);
		transformComp->WorldTransformation.rotation = rot;
		transformComp->transformation = glm::translate(identity, transformComp->WorldTransformation.position) *
										glm::mat4_cast(glm::quat(glm::radians(transformComp->WorldTransformation.rotation))) *
										glm::scale(identity, transformComp->WorldTransformation.scale);

		TransformComponent* parentTrans = ECS::GetInstance()->GetComponent<TransformComponent>(transformComp->m_parentID);
		if (parentTrans) {
			transformComp->localTransform = glm::inverse(parentTrans->transformation) * transformComp->transformation;
			math::DecomposeMtxIntoTRS(transformComp->localTransform, transformComp->LocalTransformation.position, transformComp->LocalTransformation.rotation, transformComp->LocalTransformation.scale);
		}
		else {
			transformComp->LocalTransformation.rotation = transformComp->WorldTransformation.rotation;
			transformComp->localTransform = transformComp->transformation;
		}
	}

	void TransformSystem::SetImmediateWorldScale(TransformComponent* transformComp, glm::vec3&& scale){
		if (!transformComp) return;
		constexpr glm::mat4 identity(1.0f);
		transformComp->WorldTransformation.scale = scale;
		transformComp->transformation = glm::translate(identity, transformComp->WorldTransformation.position) *
			glm::mat4_cast(glm::quat(glm::radians(transformComp->WorldTransformation.rotation))) *
			glm::scale(identity, transformComp->WorldTransformation.scale);

		TransformComponent* parentTrans = ECS::GetInstance()->GetComponent<TransformComponent>(transformComp->m_parentID);
		if (parentTrans) {
			transformComp->localTransform = glm::inverse(parentTrans->transformation) * transformComp->transformation;
			math::DecomposeMtxIntoTRS(transformComp->localTransform, transformComp->LocalTransformation.position, transformComp->LocalTransformation.rotation, transformComp->LocalTransformation.scale);
		}
		else {
			transformComp->LocalTransformation.scale = transformComp->WorldTransformation.scale;
			transformComp->localTransform = transformComp->transformation;
		}
	}

	void TransformSystem::SetImmediateLocalPosition(TransformComponent* transformComp, glm::vec3&& pos){
		if (!transformComp) return;
		transformComp->LocalTransformation.position = pos;
		CalculateAllTransform(transformComp);
	}

	void TransformSystem::SetImmediateLocalRotation(TransformComponent* transformComp, glm::vec3&& rot){
		if (!transformComp) return;
		transformComp->LocalTransformation.rotation = rot;
		CalculateAllTransform(transformComp);
	}

	void TransformSystem::SetImmediateLocalScale(TransformComponent* transformComp, glm::vec3&& scale){
		if (!transformComp) return;
		transformComp->LocalTransformation.scale = scale;
		CalculateAllTransform(transformComp);
	}
}
