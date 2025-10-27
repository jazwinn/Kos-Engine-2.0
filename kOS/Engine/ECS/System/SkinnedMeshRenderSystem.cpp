/********************************************************************/
/*!
\file      SkinnedMeshRenderSystem.cpp
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Defines the SkinnedMeshRenderSystem class, which handles
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


#include "Config/pch.h"
#include "ECS/ECS.h"

#include "SkinnedMeshRenderSystem.h"
#include "ECS/Component/SkinnedMeshRendererComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "ECS/Component/NameComponent.h"
#include "Resources/ResourceManager.h"

namespace ecs {

    void SkinnedMeshRenderSystem::Init()
    {
        // Initialize skinned mesh rendering resources if needed
    }

    void SkinnedMeshRenderSystem::Update(const std::string& scene)
    {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
            NameComponent* nameComp = ecs->GetComponent<NameComponent>(id);
            SkinnedMeshRendererComponent* skinnedMesh = ecs->GetComponent<SkinnedMeshRendererComponent>(id);

            // Skip entities not in this scene or hidden
            if ((skinnedMesh->scene != scene) || !ecs->layersStack.m_layerBitSet.test(nameComp->Layer) || nameComp->hide)
                continue;

            // TODO: Update bone matrices via AnimatorSystem
            // TODO: Submit skinned mesh (skinnedMesh->meshFile, skinnedMesh->materialFile) for rendering
        }
    }

}