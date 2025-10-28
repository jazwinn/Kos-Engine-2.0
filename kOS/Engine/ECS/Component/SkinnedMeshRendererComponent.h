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
        std::string skinnedMeshGUID{}; // Skinned mesh asset
        std::string materialGUID{};    // Material asset
        std::string skeletonGUID{};

        //Temporary
        std::string diffuseMaterialGUID{};   // Path or ID for material asset
        std::string specularMaterialGUID{};
        std::string normalMaterialGUID{};
        std::string ambientOcclusionMaterialGUID{};
        std::string roughnessMaterialGUID{};

        REFLECTABLE(SkinnedMeshRendererComponent, skinnedMeshGUID, materialGUID, skeletonGUID, diffuseMaterialGUID, specularMaterialGUID, normalMaterialGUID, ambientOcclusionMaterialGUID, roughnessMaterialGUID);

        std::string cachedSkinnedMeshGUID{};
        std::string cachedMaterialGUID{};
        std::string cachedSkeletonGUID{};
        void* cachedSkinnedMeshResource{};
        void* cachedMaterialResource{};
        void* cachedSkeletonResource{};
    };

}

#endif // SKINNEDMESHRENDERER_H