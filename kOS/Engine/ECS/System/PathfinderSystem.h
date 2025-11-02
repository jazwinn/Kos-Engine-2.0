#ifndef PATHFINDERSYSTEM_H
#define PATHFINDERSYSTEM_H

#include "ECS/ECS.h"
#include "System.h"

namespace ecs {
	class PathfinderSystem : public ISystem {
	public:
		void Init() override;
		void Update() override;
		REFLECTABLE(PathfinderSystem)
	};
}

#endif