#ifndef OCTREEGENERATORCOMPONENT_H
#define OCTREEGENERATORCOMPONENT_H

#include "Component.h"

namespace ecs {
	class OctreeGeneratorComponent : public Component {
	public:
		bool drawWireframe = true;
		REFLECTABLE(OctreeGeneratorComponent, drawWireframe)
	};
}

#endif 