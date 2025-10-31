#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ECS/ECS.h"
#include "System.h"

namespace ecs {
	class PhysicsSystem : public ISystem {
	public:
		void Init() override;
		void Update() override;
		REFLECTABLE(PhysicsSystem)
	};
}

#endif