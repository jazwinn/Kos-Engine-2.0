/********************************************************************/
/*!
\file       CharacterControllerComponent.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Defines the CharacterControllerComponent class, which 
            encapsulates data used by the PhysX Character Controller 
            for managing player or AI movement.

            Includes slope limits, step offsets, skin width, and 
            runtime data such as vertical velocity and grounded state.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef CHARACTERCONTROLLERCOMPONENT_H
#define CHARACTERCONTROLLERCOMPONENT_H

#include "Component.h"

namespace ecs {
	class CharacterControllerComponent : public Component {
	public:
		float slopeLimit{ 45.0f };
		float stepOffset{ 0.3f };
		float skinWidth{ 0.08f };
		float minMoveDistance{ 0.0f };

		float yVelocity{ 0.0f };
		bool isGrounded{ false };

		void* controller = nullptr;
		glm::vec3 prevPos{ 0.0f };
		bool isChanged = false;

		REFLECTABLE(CharacterControllerComponent, slopeLimit, stepOffset, skinWidth, minMoveDistance)
	};
}

#endif