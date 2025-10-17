/********************************************************************/
/*!
\file      SkinnedMeshRendererComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the SkinnedMeshRendererComponent class, which
           defines data for rendering animated (skinned) meshes
           within the ECS system.

           This component provides GUID references to both the
           skinned mesh and its associated material, enabling
           skeletal animation and skinning in the rendering pipeline.

           Typically used for characters, creatures, or any model
           requiring bone-based deformation.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef SKINNEDMESHRENDERER_H
#define SKINNEDMESHRENDERER_H

#include "Component.h"

namespace ecs {

    class SkinnedMeshRendererComponent : public Component {

    public:
        std::string meshGUID{};        // Skinned mesh asset
        std::string materialGUID{};    // Material asset

        REFLECTABLE(SkinnedMeshRendererComponent, meshGUID, materialGUID);
    };

}

#endif // SKINNEDMESHRENDERER_H