/*
* Класс INoPhysical отвечает за:
*	Перемещение объекта без учета физики
*	Возможно за столкновения(если не появится что-то вроде класса ICanCollide)
*
* 
*/


#ifndef NOPHYSICAL_H
#define NOPHYSICAL_H

#include "IBaseInterface.h"
#include <glm/glm.hpp>

class INoPhysical:
	public IBaseInterface
{
public:
	virtual void Move(glm::vec2 position);				 // Перемещение в указанную точку
	virtual void Move(glm::vec2 direction, float scalar);// Перемещение по заданному вектору на заданное расстояние
	virtual void Rotation(float angle);					 // Установка угла объекта
	virtual void Rotate(float angle);                    // Поворот относительно центра масс на заданный угол
	
protected:
	glm::vec2 position = { 0., 0. };                     // Позиция
	float angle = 0.;	                                 // Угол поворота тела
private:
	
};

#endif // !NOPHYSICAL_H

