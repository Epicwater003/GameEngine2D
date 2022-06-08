#include "GameObject.h"

void GameObject::Update(float dt) { //TODO: Refactor
	

	
	Body->AddVelocity(Body->GetAcceleration()*float(dt));// *dt + Body->GetForce() * (1.f / Body->GetMass()) * dt);
	glm::vec3 vel = Body->GetVelocity();
	
	Body->AddPosition(vel * float(dt));
	

	//Body->AddAngularVelocity(Body->GetAngularAcceleration() * dt + Body->GetTorque()*(1.f/Body->GetMomentOfInertia())*dt);
	float aV = Body->GetAngularVelocity();

	Body->AddAngle(aV * dt);
	
	float angle = Body->GetAngle();

	Body->SetAngle(angle);

	glm::vec3 position = Body->GetPosition();

	glm::mat4 model(1);
	
	model = glm::translate(model, position);
	model = glm::rotate(model, float(glm::degrees(angle)), glm::vec3(0, 0, 1));

	Shape->SetModelMatrix(model);
}