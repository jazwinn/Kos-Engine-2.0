/******************************************************************/
/*!
\file      SpriteComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the sprite component


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"

namespace ecs {

	class SpriteComponent : public Component {

	public:
		std::string spriteGUID{};
		glm::vec4 color{ 1.f,1.f,1.f,1.f };
		
		REFLECTABLE(SpriteComponent, spriteGUID, color);
	};

}

#endif SPRITE_H
