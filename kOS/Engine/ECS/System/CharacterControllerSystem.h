/********************************************************************/
/*!
\file       CharacterControllerSystem.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Declares the CharacterControllerSystem class, part of the ECS 
            physics layer. This system updates and manages PhysX character 
            controllers for player or NPC entities.

            Responsibilities:
            - Initialize controllers based on Capsule or Box colliders
            - Handle movement input (WASD) and jump
            - Apply gravity and update vertical velocity
            - Maintain grounding and synchronize ECS TransformComponent 
              with PhysX controller position

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef CHARACTERCONTROLLERSYSTEM_H
#define CHARACTERCONTROLLERSYSTEM_H

#include "ECS/ECS.h"
#include "System.h"

namespace ecs {
	class CharacterControllerSystem : public ISystem {
	public:
		void Init() override;
		void Update() override;
		REFLECTABLE(CharacterControllerSystem)
	};
}

#endif