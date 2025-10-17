/******************************************************************/
/*!
\file      
\author    
\par       
\date      
\brief

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef MATHUTILITY
#define MATHUTILITY

#include "Config/pch.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp" // for glm::quat

namespace math{
	void DecomposeMtxIntoTRS(glm::mat4 const& mtx, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
}

#endif MATHUTILITY
