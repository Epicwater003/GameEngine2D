#ifndef IRESPONSEENGINE_H
#define IRESPONSEENGINE_H

#include "ICollisionEngine.h"
#include "IShape.h"
#include "IBody.h"




class IResponseEngine
{
public:
	virtual void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) = 0;
protected:
private:
};


#endif // IRESPONSE_H