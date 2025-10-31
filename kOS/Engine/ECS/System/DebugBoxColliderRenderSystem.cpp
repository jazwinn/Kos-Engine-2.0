/********************************************************************/
/*!
\file      DebugBoxColliderRenderSystem.cpp
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Defines the DebugBoxColliderRenderSystem class, which
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

#include "Config/pch.h"
#include "ECS/ECS.h"

#include "DebugBoxColliderRenderSystem.h"
#include "Resources/ResourceManager.h"

#include "Graphics/GraphicsManager.h"

namespace ecs {

    void DebugBoxColliderRenderSystem::Init()
    {
        // Initialize static mesh rendering resources if needed
    }

    void DebugBoxColliderRenderSystem::Update()
    {
        ECS* ecs = ECS::GetInstance();
        std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();
        std::shared_ptr<ResourceManager> rm = ResourceManager::GetInstance();
        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
            BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id);

            if (!transform || !box) { continue; }

            glm::vec3 scale = transform->WorldTransformation.scale;
            glm::vec3 size   = box->box.size * scale;
            glm::vec3 center = box->box.center * scale + transform->WorldTransformation.position;

            glm::mat4 result{ 1.0f };
            result = glm::translate(result, center) * glm::mat4_cast(glm::quat(glm::radians(transform->WorldTransformation.rotation))) * glm::scale(result, size);

            gm->gm_PushCubeDebugData(BasicDebugData{ result });
        }
    }

}