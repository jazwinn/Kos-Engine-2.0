#include "Config/pch.h"
#include "DebugSphereColliderRenderSystem.h"
#include "Graphics/GraphicsManager.h"
#include "Resources/ResourceManager.h"

namespace ecs {
	void DebugSphereColliderRenderSystem::Init() {}

	void DebugSphereColliderRenderSystem::Update() {
		const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            TransformComponent* transform = m_ecs.GetComponent<TransformComponent>(id);
            SphereColliderComponent* sphere = m_ecs.GetComponent<SphereColliderComponent>(id);

            if (!transform || !sphere) { continue; }

            glm::vec3 scale = transform->WorldTransformation.scale;
            float maxScale = std::max(std::max(scale.x, scale.y), scale.z);

            glm::vec3 center = sphere->sphere.center * scale + transform->WorldTransformation.position;
            float radius = sphere->sphere.radius * maxScale;

            glm::mat4 result{ 1.0f };
            result = glm::translate(glm::mat4{ 1.0f }, center) * glm::scale(glm::mat4{ 1.0f }, glm::vec3{ radius });


            m_graphicsManager.gm_PushSphereDebugData(BasicDebugData{ result });
        }
	}
}