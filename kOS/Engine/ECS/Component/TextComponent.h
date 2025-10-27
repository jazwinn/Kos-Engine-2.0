/********************************************************************/
/*!
\file      TextComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the TextComponent class, which defines data for
		   rendering text within the ECS system.

		   The TextComponent stores the following properties:
		   - Font reference (GUID)
		   - Text string content
		   - Font size
		   - Text color (RGB)

		   Used by the UI rendering system to display both world-space
		   and screen-space text elements.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef TEXTCOM_H
#define TEXTCOM_H

#include "Component.h"

namespace ecs {

	class TextComponent : public Component {

	public:
		std::string fontGUID{};
		std::string text{};
		glm::vec3 color{ 1.f,1.f,1.f };
		float fontSize{ 128.f };

		REFLECTABLE(TextComponent, fontGUID, text, color, fontSize);
	};

}

#endif //TEXTCOM_H

