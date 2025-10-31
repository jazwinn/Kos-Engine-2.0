/********************************************************************/
/*!
\file       ColliderSystem.h
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Declares the ColliderSystem class, an ECS system responsible 
            for initializing and updating all static collider components 
            in the scene.

            The system interacts directly with the PhysicsManager to 
            create and maintain PhysX PxShape and PxRigidStatic objects 
            that correspond to ECS entities containing collider data.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef COLLIDERSYSTEM_H
#define COLLIDERSYSTEM_H

#include "ECS/ECS.h"
#include "System.h"

namespace ecs {
	class ColliderSystem : public ISystem {
	public:
		void Init() override;
		void Update() override;
		REFLECTABLE(ColliderSystem)
	};
}

#endif