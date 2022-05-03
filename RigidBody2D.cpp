#include "RigidBody2D.h"

void RigidBody2D::Move(glm::vec2 position) {
	this->position = position;
}

void RigidBody2D::Rotate(float angle) {
	this->angle = angle;
}
glm::vec2 RigidBody2D::CalculateMassCenter() {
	return glm::vec2(0);
}
glm::vec2 RigidBody2D::CalculateMassCenter(Mesh m) {
	return glm::vec2(0); // TODO: Сделать так: Для всех точек посчитать dense*position, сложить и поделить полученную сумму на массу тела
}

				