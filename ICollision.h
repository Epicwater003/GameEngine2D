/*
* ����� ICollision - ��������� ������������� �������� ��� �������� ������������ ��������
*/

#ifndef ICOLLISION_H
#define ICOLLISION_H

#include "IBody2D.h"
#include "IGameObject.h"
// ����� ������� ��������� ICanCollide � ���������� �������� ����������� �� ����
  // � ������ ����� ������ ����� .getboundingbox 


class ICollision	
{
public:
	virtual bool isPossibleToCollide(IGameObject& a, IGameObject& b) = 0;
	//virtual void DrawCollisionBox();

protected:
private:

};


#endif // ICOLLISION_H