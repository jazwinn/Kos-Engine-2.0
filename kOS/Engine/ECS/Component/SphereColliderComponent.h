/********************************************************************/
/*!
\file       SphereColliderComponent.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Defines the SphereColliderComponent class, representing 
            a sphere-shaped collision body used for simple physics 
            interactions or trigger zones.

            The shape can function as a solid collider or as a 
            trigger, and is linked to the physics system through a 
            PhysX PxShape pointer.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef SPHERECOLLIDERCOMPONENT_H
#define SPHERECOLLIDERCOMPONENT_H

#include "Component.h"
#include "Shapes/Shapes.h"

namespace ecs {
	class SphereColliderComponent : public Component {
	public:
		bool isTrigger = false;
		shapes::Sphere sphere;
		void* actor = nullptr;
		void* shape = nullptr;

		REFLECTABLE(SphereColliderComponent, isTrigger, sphere)
	};
}

#endif