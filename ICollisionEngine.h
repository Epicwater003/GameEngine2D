#ifndef ICOLLISIONENGINE_H
#define ICOLLISIONENGINE_H

#include <glm/glm.hpp>
#include <vector>
#include "IShape.h"

struct CollisionDetails {
	bool isCollide = false;
	float penetration = 0.;
	glm::vec3 Normal = { 0,0,0 };
	std::vector<glm::vec3> CollidePointsA;
	std::vector<glm::vec3> CollidePointsB;
	glm::vec3 ReactionA = {0,0,0};
	glm::vec3 ReactionB = {0,0,0};
};

class ICollisionEngine	
{
public:
	virtual CollisionDetails GetCollisionProperties(IShape& a, IShape& b) = 0;
protected:
private:
};


#endif // ICOLLISION_H