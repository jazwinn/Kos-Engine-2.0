#ifndef PATHFINDERCOMPONENT_H
#define PATHFINDERCOMPONENT_H

#include "Component.h"

namespace ecs {
	class PathfinderComponent : public Component {
	public:
		float pathfinderMovementSpeed = 0.f;
		bool chase = false;
		REFLECTABLE(PathfinderComponent, pathfinderMovementSpeed, chase)
	};
}

#endif 