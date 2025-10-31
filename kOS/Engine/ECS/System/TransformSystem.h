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
        void Init() override;
        void Update() override;
        static void CalculateAllTransform(TransformComponent* transComp, const glm::mat4& parentWorldMtx = glm::mat4(1.0f));
        static void CalculateLocalTransformMtx(TransformComponent* transformComp);
        static void CalculateWorldTransformMtx(TransformComponent* transformComp);
        static void SetImmediateWorldPosition(TransformComponent* transformComp, glm::vec3&& pos);
        static void SetImmediateWorldRotation(TransformComponent* transformComp, glm::vec3&& rot);
        static void SetImmediateWorldScale(TransformComponent* transformComp, glm::vec3&& scale);
        static void SetImmediateLocalPosition(TransformComponent* transformComp, glm::vec3&& pos);
        static void SetImmediateLocalRotation(TransformComponent* transformComp, glm::vec3&& rot);
        static void SetImmediateLocalScale(TransformComponent* transformComp, glm::vec3&& scale);
        REFLECTABLE(TransformSystem)
    };

}

#endif TRANSFORMSYS_H
