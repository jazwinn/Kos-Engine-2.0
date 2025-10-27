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

#include "Config/pch.h"
#include "MathUtility.h"

void math::DecomposeMtxIntoTRS(glm::mat4 const& mtx, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
	position = mtx[3];
	scale = glm::vec3(glm::length(mtx[0]), glm::length(mtx[1]), glm::length(mtx[2]));
	glm::mat3 rotationMtx;
	if (scale.x == 0.f || scale.y == 0.f || scale.z == 0.f) {
		//LOGGING_ASSERT_WITH_MSG("Division by 0");
	}
	rotationMtx[0] = mtx[0] / scale.x;
	rotationMtx[1] = mtx[1] / scale.y;
	rotationMtx[2] = mtx[2] / scale.z;
	if (glm::determinant(rotationMtx) < 0.0f) {
		scale.x = -scale.x;
		rotationMtx[0] = -rotationMtx[0];
	}
	rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMtx)));
}
