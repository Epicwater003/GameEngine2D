#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody2D.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody2D 
{
	glm::vec2 position     = { 0., 0. }; // Позиция
	glm::vec2 direction    = { 0., 1. }; // Вектор направления(лицо объекта)
	glm::vec2 velocity     = { 0., 0. }; // Скорость
	glm::vec2 acceleration = { 0., 0. }; // Ускорение
	glm::vec2 massCenter   = { 0., 0. }; // Позиция центра масс https://habr.com/ru/post/579364/
	float density = 1.;                  // Плотность
	float mass    = 1.;	                 // Масса
	float angle   = 0.;	                 // Угол поворота тела
	float angularVelocity     = 0.;	     // Угловая скорость
	float angularAcceleration = 0.;	     // Угловое ускорение
protected:
	void Move(glm::vec2 position)          ;         // Перемещение в соответствии с вектором скорости и временем кадра
	void Rotate(float angle)               ;         // Поворот относительно центра масс по времени
	virtual glm::vec2 CalculateMassCenter();         // Расчет центра масс
    glm::vec2 CalculateMassCenter(Mesh m);           // Универсальный расчет центра масс в зависимости от меша
};


#endif // !IRIGIDBODY_H

