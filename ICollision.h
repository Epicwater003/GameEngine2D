/*
* Класс ICollision - интерфейс декларирующий контракт для проверки столкновения объектов
*/

#ifndef ICOLLISION_H
#define ICOLLISION_H

#include "IBody2D.h"
#include "IGameObject.h"

class ICollision	
{
public:
	virtual bool isPossibleToCollide(IGameObject& a, IGameObject& b) = 0;

protected:
private:

};


#endif // ICOLLISION_H