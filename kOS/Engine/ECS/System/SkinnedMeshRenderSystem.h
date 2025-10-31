/********************************************************************/
/*!
\file      SkinnedMeshRenderSystem.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the SkinnedMeshRenderSystem class, which handles
           rendering of animated (skinned) meshes in the ECS system.

           This system:
           - Works with SkinnedMeshRendererComponent.
           - Uploads bone transformation matrices to shaders.
           - Ensures smooth skeletal animation during rendering.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#ifndef SKINNEDMESHRENDERSYS_H
#define SKINNEDMESHRENDERSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class SkinnedMeshRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update() override;

        REFLECTABLE(SkinnedMeshRenderSystem)
    };

}

#endif // SKINNEDMESHRENDERSYS_H