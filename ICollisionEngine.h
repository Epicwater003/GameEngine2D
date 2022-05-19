#ifndef ICOLLISIONENGINE_H
#define ICOLLISIONENGINE_H

#include <glm/glm.hpp>

struct CollisionProperties2D {
	bool isCollide = false;
	float penetration = 0.;
	glm::vec2 Normal = { 0,0 };
	glm::vec2 CollidePointA = {0,0};
	glm::vec2 CollidePointB = {0,0};
	glm::vec2 ReactionA = {0,0};
	glm::vec2 ReactionB = {0,0};
};


// Можно создать интерфейс ICanCollide и реализации объектов наследовать от него
  // в классе будут методы вроде .getboundingbox 
class IShape;

class ICollisionEngine	
{
public:
	virtual bool isCollide(IShape& a, IShape& b) = 0;
protected:
private:

};


#endif // ICOLLISION_H