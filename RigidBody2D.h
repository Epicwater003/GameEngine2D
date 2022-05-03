#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody2D.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody2D 
{
	glm::vec2 GetPosition()        { return position           ; }
	glm::vec2 GetDirection()       { return direction          ; }
	glm::vec2 GetVelocity()        { return velocity           ; }
	glm::vec2 GetAcceleration()    { return acceleration       ; }
	glm::vec2 GetMassCenter()      { return massCenter         ; }
	float GetDensity()             { return density            ; }
	float GetMass()                { return mass               ; }
	float GetAngle()               { return angle              ; }
	float GetAngularVelocity()     { return angularVelocity    ; }
	float GetAngularAcceleration() { return angularAcceleration; }

	void SetPosition(glm::vec2 p)        { position = p           ; }
	void SetDirection(glm::vec2 d)       { direction = d          ; }
	void SetVelocity(glm::vec2 v)        { velocity = v           ; }
	void SetAcceleration(glm::vec2 a)    { acceleration = a       ; }
	void SetMassCenter(glm::vec2 mc)     { massCenter = mc        ; }
	void SetDensity(float d)             { density = d            ; }
	void SetMass(float m)                { mass = m               ; }
	void SetAngle(float a)               { angle = a              ; }
	void SetAngularVelocity(float w)     { angularVelocity = w    ; }
	void SetAngularAcceleration(float e) { angularAcceleration = e; }

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
	glm::vec2 CalculateMassCenter();         // Расчет центра масс
    glm::vec2 CalculateMassCenter(Mesh m);           // Универсальный расчет центра масс в зависимости от меша
};


#endif // !IRIGIDBODY_H

