#include "Config/pch.h"
#include "ECS/ECS.h"

#include "AnimatorSystem.h"
#include "Resources/ResourceManager.h"

namespace ecs {

    void AnimatorSystem::Init()
    {
        // Initialize animation playback resources if needed
    }

    void AnimatorSystem::Update()
    {
        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            AnimatorComponent* animator = m_ecs.GetComponent<AnimatorComponent>(id);
            NameComponent* nameComp = m_ecs.GetComponent<NameComponent>(id);

            // Skip entities not in this scene or hidden
            if ( nameComp->hide)
                continue;

            // TODO: Advance playback time
            // TODO: Sample animation curves
            // TODO: Update bone transforms (stored in ECS transform hierarchy)
        }
    }

}