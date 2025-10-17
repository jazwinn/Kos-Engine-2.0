/********************************************************************/
/*!
\file      CanvasTextRenderSystem.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the CanvasTextRenderSystem class, responsible for
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


#ifndef CANVASTEXTRENDERSYS_H
#define CANVASTEXTRENDERSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class CanvasTextRenderSystem : public ISystem {

    public:

        void Init() override;
        void Update(const std::string&) override;

        REFLECTABLE(CanvasTextRenderSystem)
    };

}

#endif // CANVASTEXTRENDERSYS_H