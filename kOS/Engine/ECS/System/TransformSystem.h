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
        void Update(const std::string&) override;
        void SetTransform(TransformComponent* transComp, const glm::mat4& parentWorldMtx = glm::mat4(1.0f));
        void SetLocalTransformMtx(TransformComponent* transformComp);
        REFLECTABLE(TransformSystem)
    };

}

#endif TRANSFORMSYS_H
