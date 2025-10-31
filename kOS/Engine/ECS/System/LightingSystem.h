/********************************************************************/
/*!
\file      LightingSystem.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the LightingSystem class, which manages light
           components and updates lighting data in the ECS system.

           This system:
           - Updates and uploads light data (directional, point,
             and spot lights) to GPU shaders.
           - Interfaces with deferred and forward rendering passes.
           - Ensures lighting information remains synchronized
             across the rendering pipeline.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#pragma once
#ifndef LIGHTINGSYS_H
#define LIGHTINGSYS_H


#include "System.h"
#include "ECS/ECSList.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ecs {

	class LightingSystem : public ISystem {

	public:


		void Init() override;
		void Update() override;

		REFLECTABLE(LightingSystem)
	};
}


#endif