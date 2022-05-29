#include "GameObject.h"

void GameObject::Update(float dt) { //TODO: Refactor
	const glm::vec3 gravity(0., -9.8, 0.);
	
	//Body->AddVelocity(Body->GetAcceleration() * dt + Body->GetForce() * (1.f / Body->GetMass()) * dt + gravity*dt);
	Body->AddPosition(Body->GetVelocity() * dt);

	//Body->AddAngularVelocity(Body->GetAngularAcceleration() * dt + Body->GetTorque()*(1.f/Body->GetMomentOfInertia())*dt);
	Body->AddAngle(Body->GetAngularVelocity() * dt);
	
	float     angle = Body->GetAngle();
	glm::vec3 position = Body->GetPosition();

	glm::mat4 model(1);
	
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));

	Shape->SetModelMatrix(model);
}