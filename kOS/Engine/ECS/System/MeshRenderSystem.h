/********************************************************************/
/*!
\file      MeshRenderSystem.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the MeshRenderSystem class, responsible for
           rendering static meshes within the ECS system.

           This system:
           - Handles entities with MeshFilterComponent and
             MeshRendererComponent.
           - Submits mesh draw calls to the rendering pipeline.
           - Supports material and texture binding for PBR rendering.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#ifndef MESHRENDERSYS_H
#define MESHRENDERSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class MeshRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update() override;

        REFLECTABLE(MeshRenderSystem)
    };

}

#endif // MESHRENDERSYS_H