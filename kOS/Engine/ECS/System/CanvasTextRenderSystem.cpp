/********************************************************************/
/*!
\file      CanvasTextRenderSystem.cpp
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Defines the CanvasTextRenderSystem class, responsible for
           rendering text elements on UI canvases within the ECS
           system.

           This system:
           - Draws text from entities containing TextComponents under 
             CanvasRenderer Components.
           - Supports both screen-space and world-space text rendering.
           - Interfaces with font rendering and shader management for
             accurate text display.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "Config/pch.h"
#include "ECS/ECS.h"

#include "CanvasTextRenderSystem.h"
#include "Resources/ResourceManager.h"
#include "ECS/Hierachy.h"
#include "Graphics/GraphicsManager.h"

namespace ecs {

    void CanvasTextRenderSystem::Init()
    {
        // Initialize UI rendering resources if needed
    }

    void CanvasTextRenderSystem::Update()
    {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();

        for (const EntityID id : entities) {
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);
            NameComponent* nameComp = ecs->GetComponent<NameComponent>(id);
            CanvasRendererComponent* canvas = ecs->GetComponent<CanvasRendererComponent>(id);

            // Skip entities not in this scene or hidden
            if (!ecs->layersStack.m_layerBitSet.test(nameComp->Layer) || nameComp->hide)
                continue;

            std::optional<std::vector<EntityID>> childEntities = hierachy::m_GetChild(id);
            if (!childEntities.has_value()) continue;

            for (EntityID childID : childEntities.value())
            {
                std::shared_ptr<ResourceManager> rm = ResourceManager::GetInstance();
                std::shared_ptr<GraphicsManager> gm = GraphicsManager::GetInstance();
                if (ecs->HasComponent<TextComponent>(childID))
                {
                    TextComponent* textComp = ecs->GetComponent<TextComponent>(childID);
                    TransformComponent* childTransform = ecs->GetComponent<TransformComponent>(childID);
                    if (!textComp->fontGUID.empty())
                    {
                        std::shared_ptr<R_Font> fontResource = rm->GetResource<R_Font>(textComp->fontGUID);
                        gm->gm_PushScreenTextData(ScreenTextData{ childTransform->WorldTransformation.position,
                                                       glm::vec2{ childTransform->WorldTransformation.scale.x,
                                                                  childTransform->WorldTransformation.scale.y},
                                                                  childTransform->WorldTransformation.rotation.x,
                                                       glm::vec4{ textComp->color.x,textComp->color.y,textComp->color.z,1.f}, 
                                                                  fontResource.get(), textComp->text , textComp->fontSize, false});
                    }
                }
     
            }
        }
    }

}