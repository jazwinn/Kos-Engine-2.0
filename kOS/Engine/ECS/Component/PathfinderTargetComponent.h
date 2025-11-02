#ifndef PATHFINDERTARGETCOMPONENT_H
#define PATHFINDERTARGETCOMPONENT_H

#include "Component.h"

namespace ecs {
	class PathfinderTargetComponent : public Component {
	public:
		REFLECTABLE(PathfinderTargetComponent)
	};
}

#endif 