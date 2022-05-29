#ifndef ICOLLISIONENGINE_H
#define ICOLLISIONENGINE_H

#include <glm/glm.hpp>
#include <vector>
#include "IShape.h"

struct CollisionDetails {
	bool isCollide = false;
	float penetration = 0.;
	glm::vec3 Normal = { 0,0,0 };
	glm::vec3 CollidePointA = {0,0,0};
	glm::vec3 CollidePointB = {0,0,0};
	glm::vec3 ReactionA = {0,0,0};
	glm::vec3 ReactionB = {0,0,0};
};


// Можно создать интерфейс ICanCollide и реализации объектов наследовать от него
  // в классе будут методы вроде .getboundingbox 


class IBroadPhase {
public:
	//std::vector<bool> collisionMap; // Что-то вроде массива возможных столкновений
	virtual std::vector<bool> MayColllide(std::vector<IShape>& shps) = 0; // Должна отдавать массив возможных столкновений для пар объектов типа Shape
protected:
private:
};

class INarrowPhase	
{
public:
	//std::vector<CollisionProperties> cprps; // Все обработанные столкновения без повторений
	virtual CollisionDetails GetCollisionProperties(IShape& a, IShape& b) = 0;
	//virtual std::vector<CollisionProperties> CalculateCollisions(std::vector<IShape>& shps) = 0;
protected:
private:

};


#endif // ICOLLISION_H