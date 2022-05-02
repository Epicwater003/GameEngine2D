#ifndef IRIGIDBODY_H
#define IRIGIDBODY_H

#include "IBaseInterface.h"
#include <glm/glm.hpp>

class IRigidBody: 
	public IBaseInterface 
{
	glm::vec2 position = { 0., 0. };     // Позиция
	glm::vec2 direction = { 0., 1. };    // Вектор направления(лицо объекта)
	glm::vec2 velocity = { 0., 0. };     // Скорость
	glm::vec2 acceleration = { 0., 0. }; // Ускорение
	glm::vec2 massCenter = { 0., 0. };   // Позиция центра масс https://habr.com/ru/post/579364/
	float density = 1.;                  // Плотность
	float mass = 1.;	                 // Масса
	float angle = 0.;	                 // Угол поворота тела
	float angularVelocity = 0.;	         // Угловая скорость
	float angularAcceleration = 0.;	     // Угловое ускорение
protected:
	virtual void Move(float deltaTime);					 // Перемещение в соответствии с вектором скорости и временем кадра
	virtual void Rotate(double deltaTime);  		     // Поворот относительно центра масс по времени
	virtual glm::vec2 CalculateMassCenter() = 0;         // Расчет центра масс
};


#endif // !IRIGIDBODY_H

