#include "RigidBody2D.h"



glm::vec3 RigidBody2D::CalculateMassCenter() {
	massCenter = glm::vec2(0);
	return glm::vec3(0);
}
glm::vec3 RigidBody2D::CalculateMassCenter(IShape& sh) {
	auto vs = sh.GetVertices();
	glm::vec2 answ(0);
	for (auto& v : vs) {
		answ += glm::vec2(v.position * density);
	}
	massCenter = answ/mass;
	return glm::vec3(answ/mass, 0);
}

				