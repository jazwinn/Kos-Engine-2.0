#include "BoundsCheck.h"

namespace Octrees {
	bool Bounds::Intersects(Bounds otherBounds) {
		return ((std::abs(center.x - otherBounds.center.x) < (size.x + otherBounds.size.x)) &&
			(std::abs(center.y - otherBounds.center.y) < (size.y + otherBounds.size.y)) &&
			(std::abs(center.z - otherBounds.center.z) < (size.z + otherBounds.size.z)));
	}
}