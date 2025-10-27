#pragma once

/******************************************************************/
/*!
\file      OctreeObject.h
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

#ifndef OCTREEOBJECT_H
#define OCTREEOBJECT_H

#include "Config/pch.h"
#include "ECS/ECS.h"

#include "BoundsCheck.h"

namespace Octrees {
	struct OctreeObject {
		Bounds bounds;

		//OctreeObject();
		OctreeObject(ecs::EntityID id);
		bool Intersects(Bounds boundsToCheck);
	};
}

#endif