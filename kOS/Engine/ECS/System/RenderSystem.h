/******************************************************************/
/*!
\file      RenderSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the Render System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef RENDERSYS_H
#define RENDERSYS_H


#include "System.h"
#include "ECS/ECSList.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ecs{

	class RenderSystem : public ISystem {

	public:


		void Init() override;
		void Update() override;

		REFLECTABLE(RenderSystem)
	};
}


#endif