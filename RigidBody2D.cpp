#include "RigidBody2D.h"

void RigidBody2D::MoveToPosition(glm::vec3 position) {
	this->position = { position.x,position.y };
}
void RigidBody2D::Move(glm::vec3 direction) {
	this->position.x += direction.x;
	this->position.y += direction.y;
}

void RigidBody2D::Rotate(float angle) {
	this->angle = angle;
}
glm::vec3 RigidBody2D::CalculateMassCenter() {
	return glm::vec3(0);
}
glm::vec3 RigidBody2D::CalculateMassCenter(Mesh m) {
	return glm::vec3(0); // TODO: Сделать так: Для всех точек посчитать dense*position, сложить и поделить полученную сумму на массу тела
}

				