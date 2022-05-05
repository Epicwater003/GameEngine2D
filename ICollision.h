/*
* Класс ICollision - интерфейс декларирующий контракт для проверки столкновения объектов
*/

#ifndef ICOLLISION_H
#define ICOLLISION_H

#include "IBody2D.h"
#include "IGameObject.h"
// Можно создать интерфейс ICanCollide и реализации объектов наследовать от него
  // в классе будут методы вроде .getboundingbox 


class ICollision	
{
public:
	virtual bool isPossibleToCollide(IGameObject& a, IGameObject& b) = 0;
	//virtual void DrawCollisionBox();

protected:
private:

};


#endif // ICOLLISION_H