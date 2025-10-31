#pragma once
#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class  DebugCapsuleColliderRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update() override;

        REFLECTABLE(DebugCapsuleColliderRenderSystem)
    };

}