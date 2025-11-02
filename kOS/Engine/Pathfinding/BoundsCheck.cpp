#include "BoundsCheck.h"

namespace Octrees {
	Bounds::Bounds() : center{ 0.f, 0.f, 0.f }, size{ 0.f, 0.f, 0.f }, min{ 0.f, 0.f, 0.f }, max{ 0.f, 0.f, 0.f } {
	}

	Bounds::Bounds(glm::vec3 _center, glm::vec3 _size) : center{ _center }, size{ _size } {
		glm::vec3 boundMin = center - size, boundMax = center + size;
		min = boundMin;
		max = boundMax;
	}

	bool Bounds::Intersects(Bounds otherBounds) {
		return ((std::abs(center.x - otherBounds.center.x) < (size.x + otherBounds.size.x)) &&
			(std::abs(center.y - otherBounds.center.y) < (size.y + otherBounds.size.y)) &&
			(std::abs(center.z - otherBounds.center.z) < (size.z + otherBounds.size.z)));
	}

	void Bounds::SetMinMax(glm::vec3 _min, glm::vec3 _max) {
		min = _min;
		max = _max;
		//center = (min + max) * 0.5f;
		//size = (max - min);
	}

	bool Bounds::Contains(glm::vec3 point) {
		//glm::vec3 d = point - center;

		//glm::vec3 rotationInDegrees(rotation);
		//glm::vec3 rotationInRad = glm::radians(rotationInDegrees);
		//glm::quat q = glm::quat(rotationInRad);
		//glm::vec3 forward = q * glm::vec3(0.f, 0.f, 1.f);
		//glm::vec3 right = q * glm::vec3(1.f, 0.f, 0.f);
		//glm::vec3 up = q * glm::vec3(0.f, 1.f, 0.f);

		//glm::vec3 localAxes = {
		//	glm::normalize(right).x,
		//	glm::normalize(up).y,
		//	glm::normalize(forward).z
		//};

		//glm::vec3 projectedLocalAxes = localAxes * d;

		//if (std::abs(projectedLocalAxes.x) <= size.x && std::abs(projectedLocalAxes.x) <= size.y && std::abs(projectedLocalAxes.x) <= size.z) {
		//	return true;
		//}

		//return false;

		// Convert degrees to radians
		//glm::vec3 rotationInRad = glm::radians()

		return
			min.x < point.x && max.x > point.x &&
			min.y < point.y && max.y > point.y &&
			min.z < point.z && max.z > point.z;
	}
}