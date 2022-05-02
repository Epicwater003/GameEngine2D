#include "GameObject.h"
GameObject::GameObject() {
}



void GameObject::Update() {
	model = glm::mat4(1);     // TODO: Отрефакторить этот костыльный код
	model = glm::translate(model, glm::vec3(position, 0.));
	model = glm::rotate(model, angle, glm::vec3(0., 0., 1.));
}
