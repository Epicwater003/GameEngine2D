#ifndef IBODY2D_H
#define IBODY2D_H

#include <glm/glm.hpp>

class IBody2D
{
public:
	// TODO: Явно задать контракт на геттеры и сеттеры
	

	virtual glm::vec2 GetPosition() = 0;
	virtual glm::vec2 GetDirection() = 0;
	virtual glm::vec2 GetVelocity() = 0;
	virtual glm::vec2 GetAcceleration() = 0;
	virtual glm::vec2 GetMassCenter() = 0;
	virtual float GetDensity() = 0;
	virtual float GetMass() = 0;
	virtual float GetAngle() = 0;
	virtual float GetAngularVelocity() = 0;
	virtual float GetAngularAcceleration() = 0;

	virtual void SetPosition(glm::vec2) = 0;
	virtual void SetDirection(glm::vec2) = 0;
	virtual void SetVelocity(glm::vec2) = 0;
	virtual void SetAcceleration(glm::vec2) = 0;
	virtual void SetMassCenter(glm::vec2) = 0;
	virtual void SetDensity(float) = 0;
	virtual void SetMass(float) = 0;
	virtual void SetAngle(float) = 0;
	virtual void SetAngularVelocity(float) = 0;
	virtual void SetAngularAcceleration(float) = 0;

	virtual void Move(glm::vec2 position)   = 0;		   // Перемещение в соответствии с вектором скорости и временем кадра
	virtual void Rotate(float angle)        = 0;  		   // Поворот относительно центра масс по времени
	virtual glm::vec2 CalculateMassCenter() = 0;           // Расчет центра масс
};


#endif // IBODY_H

