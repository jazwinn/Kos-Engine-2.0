/********************************************************************/
/*!
\file      MeshRendererComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the MeshRendererComponent class, which defines
           material data for rendering static meshes in the ECS
           system.

           This component contains GUID references to material
           textures used in physically based rendering (PBR),
           including:
           - Diffuse
           - Specular
           - Normal
           - Ambient Occlusion
           - Roughness maps

           Used together with MeshFilterComponent to fully describe
           how an entity’s mesh should be shaded and displayed
           in the rendering pipeline.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Component.h"

namespace ecs {

    class MeshRendererComponent : public Component {

    public:
        std::string diffuseMaterialGUID{};   // Path or ID for material asset
        std::string specularMaterialGUID{};
        std::string normalMaterialGUID{};
        std::string ambientOcclusionMaterialGUID{};
        std::string roughnessMaterialGUID{};

        REFLECTABLE(MeshRendererComponent, diffuseMaterialGUID, specularMaterialGUID, normalMaterialGUID, ambientOcclusionMaterialGUID, roughnessMaterialGUID);

    };

   
}

#endif // MESHRENDERER_H