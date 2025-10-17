/********************************************************************/
/*!
\file       CapsuleColliderComponent.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Defines the CapsuleColliderComponent class, representing 
            a capsule-shaped collision volume typically used for 
            character controllers or rounded body parts.

            The capsule parameters include radius, height, and 
            orientation, with an optional trigger flag for 
            non-physical detection.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef CAPSULECOLLIDERCOMPONENT_H
#define CAPSULECOLLIDERCOMPONENT_H

#include "Component.h"
#include "Shapes/Shapes.h"

namespace ecs {
	class CapsuleColliderComponent : public Component {
	public:
		bool isTrigger = false;
		shapes::Capsule capsule;

		void* shape = nullptr;

		REFLECTABLE(CapsuleColliderComponent, isTrigger, capsule)
	};
}

#endif