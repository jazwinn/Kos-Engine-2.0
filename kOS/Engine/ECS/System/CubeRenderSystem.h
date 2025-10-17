#ifndef CUBERENDERSYS_H
#define CUBERENDERSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class CubeRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update(const std::string&) override;

        REFLECTABLE(CubeRenderSystem)
    };

}

#endif