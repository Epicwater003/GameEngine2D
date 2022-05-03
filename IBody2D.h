#ifndef IBODY2D_H
#define IBODY2D_H

#include <glm/glm.hpp>

class IBody2D
{
public:
	// TODO: Явно задать контракт на геттеры и сеттеры
	virtual void Move(glm::vec2 position)   = 0;		   // Перемещение в соответствии с вектором скорости и временем кадра
	virtual void Rotate(float angle)        = 0;  		   // Поворот относительно центра масс по времени
	virtual glm::vec2 CalculateMassCenter() = 0;           // Расчет центра масс
};


#endif // IBODY_H

