#include "OctreeNode.h"

/******************************************************************/
/*!
\file      OctreeNode.cpp
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

namespace Octrees {
	int OctreeNode::nextId = 0;

	OctreeNode::OctreeNode()
	{
		//bounds.center;
		id = -1;
	}

	OctreeNode::OctreeNode(Bounds _bounds, float _minNodeSize) {
		childBounds.resize(8);

		id = nextId++;

		bounds.center.x = _bounds.center.x;
		bounds.center.y = _bounds.center.y;
		bounds.center.z = _bounds.center.z;
		bounds.size.x = _bounds.size.x;
		bounds.size.y = _bounds.size.y;
		bounds.size.z = _bounds.size.z;
		bounds.min.x = _bounds.min.x;
		bounds.min.y = _bounds.min.y;
		bounds.min.z = _bounds.min.z;
		bounds.max.x = _bounds.max.x;
		bounds.max.y = _bounds.max.y;
		bounds.max.z = _bounds.max.z;


		minNodeSize = _minNodeSize;
		glm::vec3 newSize = bounds.size * 0.5f;
		glm::vec3 centerOffset = bounds.size * 0.25f;
		glm::vec3 parentCenter = bounds.center;

		for (int i = 0; i < 8; ++i) {
			glm::vec3 childCenter = parentCenter;
			childCenter.x += newSize.x * ((i & 1) == 0 ? -1 : 1);
			childCenter.y += newSize.y * ((i & 2) == 0 ? -1 : 1);
			childCenter.z += newSize.z * ((i & 4) == 0 ? -1 : 1);
			childBounds[i] = Bounds(childCenter, newSize);
		}
	}

	bool OctreeNode::IsLeaf() {
		return children.empty();
	}

	void OctreeNode::Divide(ecs::EntityID id) {
		Divide(OctreeObject(id));
	}

	void OctreeNode::Divide(OctreeObject octreeObject) {
		if (bounds.size.x <= minNodeSize) {
			AddObject(octreeObject);
			return;
		}

		if (children.empty()) {
			children.resize(8);
		}

		bool intersectedChild = false;

		for (int i = 0; i < 8; ++i) {
			if (children[i].id == -1) {
				
				children[i] = OctreeNode(childBounds[i], minNodeSize);
			}

			if (octreeObject.Intersects(childBounds[i])) {
				children[i].Divide(octreeObject);
				intersectedChild = true;
			}
		}

		if (!intersectedChild) {
			AddObject(octreeObject);
		}
	}

	void OctreeNode::AddObject(OctreeObject octreeObject) {
		objects.push_back(octreeObject);
	}

	void OctreeNode::DrawNode() {
		std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();
		glm::mat4 model{ 1.f };
		model = glm::translate(model, bounds.center) * glm::scale(model, bounds.size * 2.f);
		BasicDebugData basicDebug;
		basicDebug.worldTransform = model;
		gm->gm_PushCubeDebugData(BasicDebugData{ basicDebug });

		if (!children.empty()) {
			for (OctreeNode child : children) {
				if (child.id != -1) {
					child.DrawNode();
				}
			}
		}
	}

	bool OctreeNode::operator==(const OctreeNode& other) const {
		return id == other.id;
	}
}