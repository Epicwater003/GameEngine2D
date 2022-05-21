#include "GameObject.h"

void GameObject::Update() { //TODO: Refactor
	float     angle    = Body->GetAngle();
	glm::vec3 position = Body->GetPosition();

	glm::mat4 model(1);
	
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));

	Shape->SetModelMatrix(model);
}