/******************************************************************/
/*!
\file      TransformSystem.h
\author    Jaz winn
\par       jazwinn.ng@digipen.edu
\date      Sept 29, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef TRANSFORMSYS_H
#define TRANSFORMSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class TransformSystem : public ISystem {
    public:
        using ISystem::ISystem;
        void Init() override;
        void Update() override;



		//TODO,find a better way of impelementing this
        static void CalculateAllTransform(ECS& ecs, TransformComponent* transComp, const glm::mat4& parentWorldMtx = glm::mat4(1.0f));
        static void CalculateLocalTransformMtx(ECS& ecs, TransformComponent* transformComp);
        static void SetImmediateWorldPosition(ECS& ecs, TransformComponent* transformComp, glm::vec3&& pos);
        static void SetImmediateWorldRotation(ECS& ecs, TransformComponent* transformComp, glm::vec3&& rot);
        static void SetImmediateWorldScale(ECS& ecs, TransformComponent* transformComp, glm::vec3&& scale);
        static void SetImmediateLocalPosition(ECS& ecs, TransformComponent* transformComp, glm::vec3&& pos);
        static void SetImmediateLocalRotation(ECS& ecs, TransformComponent* transformComp, glm::vec3&& rot);
        static void SetImmediateLocalScale(ECS& ecs, TransformComponent* transformComp, glm::vec3&& scale);
        REFLECTABLE(TransformSystem)
    };

}

#endif TRANSFORMSYS_H
