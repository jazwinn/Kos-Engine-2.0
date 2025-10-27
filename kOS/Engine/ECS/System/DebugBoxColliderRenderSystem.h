/********************************************************************/
/*!
\file      DebugBoxColliderRenderSystem.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the DebugBoxColliderRenderSystem class, which
           visualizes box colliders for debugging purposes within
           the ECS system.

           This system:
           - Draws wireframe representations of box colliders.
           - Aids in debugging physics and collision components.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#pragma once
#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class  DebugBoxColliderRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update(const std::string&) override;

        REFLECTABLE(DebugBoxColliderRenderSystem)
    };

}