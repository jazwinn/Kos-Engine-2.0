#pragma once

/******************************************************************/
/*!
\file      OctreeNode.h
\author    Yeo See Kiat Raymond, seekiatraymond.yeo, 2301268
\par       seekiatraymond.yeo@digipen.edu
\date      October 3, 2025
\brief     This file contains the object logic for octree nodes,
			holds the data for the bounds, ida and determines
			when to expand.


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef OCTREENODE_H
#define OCTREENODE_H

#include "Config/pch.h"
#include "ECS/ECS.h"

#include "../../Graphics/GraphicsManager.h"

#include "BoundsCheck.h"
#include "OctreeObject.h"

namespace Octrees {
	struct OctreeNode {
		std::vector<OctreeObject> objects;

		static int nextId;
		int id;

		Bounds bounds;
		std::vector<Bounds> childBounds;
		std::vector<OctreeNode> children;
		bool IsLeaf();

		float minNodeSize = 1.f;

		OctreeNode();
		OctreeNode(Bounds bounds, float minNodeSize);
		void Divide(ecs::EntityID id);
		void Divide(OctreeObject octreeObject);
		void AddObject(OctreeObject octreeObject);
		void DrawNode();

		bool operator==(const OctreeNode& other) const;
	};
}

#endif
