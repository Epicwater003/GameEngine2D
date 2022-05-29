#ifndef IRESPONSEENGINE_H
#define IRESPONSEENGINE_H

#include "ICollisionEngine.h"
#include "IShape.h"
#include "IBody.h"

struct ResponseDetails {
	glm::vec3 impulse = {0,0,0};
	glm::vec3 impulseComponent = { 0,0,0 };
	glm::vec3 frictionImpulse = { 0,0,0 };
};


class IResponseEngine
{
public:
	virtual void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps ) = 0;
protected:
private:
};


#endif // IRESPONSE_H