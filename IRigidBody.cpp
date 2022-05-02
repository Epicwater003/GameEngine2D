#include "IRigidBody.h"

void IRigidBody::Move(float deltaTime) {
	position += velocity * deltaTime;
}

void IRigidBody::Rotate(double deltaTime) {
	angle = angularVelocity * deltaTime;
}

				