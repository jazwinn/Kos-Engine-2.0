#pragma once

#ifndef BOUNDSCHECK_H
#define BOUNDSCHECK_H

#include "Config/pch.h"
#include "ECS/ECS.h"

namespace Octrees {
	struct Bounds {
		//glm::vec3 center, size, min, max, rotation;
		Bounds();
		Bounds(glm::vec3 _center, glm::vec3 _size);

		glm::vec3 center, size, min, max;

		bool Intersects(Bounds otherBounds);
		void SetMinMax(glm::vec3 _min, glm::vec3 _max);
		bool Contains(glm::vec3 point);
	};
}

#endif