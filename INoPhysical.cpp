#include "INoPhysical.h"

void INoPhysical::Move(glm::vec2 position) {
	this->position = position;
	
}
void INoPhysical::Move(glm::vec2 direction, float scalar) {
	position += direction * scalar;
	
}

void INoPhysical::Rotation(float angle) {
	this->angle = angle;
}
void INoPhysical::Rotate(float angle) {
	this->angle += angle;
}
