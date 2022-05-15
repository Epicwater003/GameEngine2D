#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>

struct AxisAligned2D {
	AxisAligned2D(){};
	AxisAligned2D(glm::vec2 min, glm::vec2 max) {
		this->min = min;
		this->max = max;
	}
	glm::vec2 min = { 0,0 };
	glm::vec2 max = { 0,0 };
};


#endif // !BOUNDINGBOX_H

