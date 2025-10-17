#include "Config/pch.h"
#include "ECS/ECS.h"

#include "DebugCapsuleColliderRenderSystem.h"
#include "Resources/ResourceManager.h"

#include "Graphics/GraphicsManager.h"

namespace ecs {

    void DebugCapsuleColliderRenderSystem::Init()
    {
        // Initialize static mesh rendering resources if needed
    }

    void DebugCapsuleColliderRenderSystem::Update(const std::string& scene)
    {
        ECS* ecs = ECS::GetInstance();
        std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();
        std::shared_ptr<ResourceManager> rm = ResourceManager::GetInstance();
        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
            CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id);

            //gm->gm_PushCubeDebugData(BasicDebugData{ transform->transformation });
        }
    }

}