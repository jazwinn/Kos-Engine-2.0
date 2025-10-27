/********************************************************************/
/*!
\file      CanvasRendererComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the CanvasRendererComponent class, which defines
           rendering behavior for UI canvases within the ECS system.

           The CanvasRendererComponent determines how UI elements
           are drawn relative to the screen or world space. It
           includes:
           - A flag indicating whether the canvas is rendered in
             screen space (true) or world space (false).

           This component is typically used to manage UI rendering
           contexts such as HUDs, menus, and in-world interfaces.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef CANVASCOM_H
#define CANVASCOM_H

#include "Component.h"

namespace ecs {

	class CanvasRendererComponent : public Component {

	public:
		bool isScreenSpace{ true };

		REFLECTABLE(CanvasRendererComponent, isScreenSpace);
	};

}

#endif //CANVASCOM_H
