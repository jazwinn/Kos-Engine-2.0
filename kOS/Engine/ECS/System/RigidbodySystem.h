/********************************************************************/
/*!
\file       RigidbodySystem.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Declares the RigidbodySystem class, part of the ECS 
            physics layer. This system updates and synchronizes all 
            dynamic RigidbodyComponents within a scene.

            The system creates, updates, and synchronizes PhysX 
            PxRigidDynamic actors corresponding to ECS entities, 
            ensuring that physical simulation results are reflected 
            in ECS TransformComponents each frame.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "ECS/ECS.h"
#include "System.h"

namespace ecs {
	class RigidbodySystem : public ISystem {
	public:
		void Init() override;
		void Update() override;
		REFLECTABLE(RigidbodySystem)
	};
}

#endif