#ifndef OCTREEGENERATORCOMPONENT_H
#define OCTREEGENERATORCOMPONENT_H

#include "Component.h"

namespace ecs {
	class OctreeGeneratorComponent : public Component {
	public:
		bool drawWireframe = true;
		bool drawNodes = true;
		bool drawBound = true;
		REFLECTABLE(OctreeGeneratorComponent, drawWireframe, drawNodes, drawBound)
	};
}

#endif 