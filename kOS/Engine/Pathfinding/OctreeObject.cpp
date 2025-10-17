/******************************************************************/
/*!
\file      OctreeObject.cpp
\author    Yeo See Kiat Raymond, seekiatraymond.yeo, 2301268
\par       seekiatraymond.yeo@digipen.edu
\date      October 3, 2025
\brief     This file contains the object logic for octree objects,
			check if it overlaps with another object.


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "OctreeObject.h"

namespace Octrees {
	OctreeObject::OctreeObject(ecs::EntityID entityID) {
		ecs::ECS* ecs = ecs::ECS::GetInstance();

		bounds.center.x = ecs->GetComponent<ecs::TransformComponent>(entityID)->WorldTransformation.position.x;
		bounds.center.y = ecs->GetComponent<ecs::TransformComponent>(entityID)->WorldTransformation.position.y;
		bounds.center.z = ecs->GetComponent<ecs::TransformComponent>(entityID)->WorldTransformation.position.z;
		bounds.max.x = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.x;
		bounds.max.y = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.y;
		bounds.max.z = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.z;
		bounds.min.x = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.x;
		bounds.min.y = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.y;
		bounds.min.z = ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.z;



		glm::vec3 boundSize = glm::vec3(1.f, 1.f, 1.f) * std::max(std::max(ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.x - ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.x,
			ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.y - ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.y),
			ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.max.z - ecs->GetComponent<ecs::BoxColliderComponent>(entityID)->box.bounds.min.z) * 0.5f;

		bounds.size.x = boundSize.x;
		bounds.size.y = boundSize.y;
		bounds.size.z = boundSize.z;
	}

	bool OctreeObject::Intersects(Bounds boundsToCheck) {
		return bounds.Intersects(boundsToCheck);
	}
}