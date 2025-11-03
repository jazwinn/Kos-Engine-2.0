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
	OctreeObject::OctreeObject(const ecs::TransformComponent* tc, const ecs::BoxColliderComponent* bc) {

		bounds.center.x = tc->WorldTransformation.position.x;
		bounds.center.y = tc->WorldTransformation.position.y;
		bounds.center.z = tc->WorldTransformation.position.z;
		bounds.max.x = bc->box.bounds.max.x;
		bounds.max.y = bc->box.bounds.max.y;
		bounds.max.z = bc->box.bounds.max.z;
		bounds.min.x = bc->box.bounds.min.x;
		bounds.min.y = bc->box.bounds.min.y;
		bounds.min.z = bc->box.bounds.min.z;



		glm::vec3 boundSize = glm::vec3(1.f, 1.f, 1.f) * std::max(std::max(bc->box.bounds.max.x - bc->box.bounds.min.x,
			bc->box.bounds.max.y - bc->box.bounds.min.y),
			bc->box.bounds.max.z - bc->box.bounds.min.z) * 0.5f;

		bounds.size.x = boundSize.x;
		bounds.size.y = boundSize.y;
		bounds.size.z = boundSize.z;
	}

	bool OctreeObject::Intersects(Bounds boundsToCheck) {
		return bounds.Intersects(boundsToCheck);
	}
}