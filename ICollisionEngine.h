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


// ����� ������� ��������� ICanCollide � ���������� �������� ����������� �� ����
  // � ������ ����� ������ ����� .getboundingbox 


class IBroadPhase {
public:
	//std::vector<bool> collisionMap; // ���-�� ����� ������� ��������� ������������
	virtual std::vector<bool> MayColllide(std::vector<IShape>& shps) = 0; // ������ �������� ������ ��������� ������������ ��� ��� �������� ���� Shape
protected:
private:
};

class INarrowPhase	
{
public:
	//std::vector<CollisionProperties> cprps; // ��� ������������ ������������ ��� ����������
	virtual CollisionDetails GetCollisionProperties(IShape& a, IShape& b) = 0;
	//virtual std::vector<CollisionProperties> CalculateCollisions(std::vector<IShape>& shps) = 0;
protected:
private:

};


#endif // ICOLLISION_H