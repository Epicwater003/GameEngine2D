/*
* TODO: Возможно нужно унаследоваться от GameObject
* 
*/

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody 
{
public:

	IBody*    GetBody()            { return this; }
	glm::vec3 GetPosition()        { return glm::vec3(position.x, position.y, zIndex); }
	glm::vec3 GetDirection()       { return glm::vec3(direction.x,direction.y,0); }
	glm::vec3 GetVelocity() { return glm::vec3(velocity.x, velocity.y, 0);}
	glm::vec3 GetAcceleration() { return glm::vec3(acceleration.x, acceleration.y, 0); }
	glm::vec3 GetMassCenter()      { return glm::vec3(massCenter.x,massCenter.y,0); }
	float GetDensity()             { return density            ; }
	float GetMass()                { return mass               ; }
	float GetAngle()               { return angle              ; }
	float GetAngularVelocity()     { return angularVelocity    ; }
	float GetAngularAcceleration() { return angularAcceleration; }

	void SetBody(IBody& body) {
		//position = body.GetPosition(); TODO: Реализовать
	};
	void SetPosition(glm::vec3 p) { position = { p.x,p.y }; }
	void SetDirection(glm::vec3 d) { direction = { d.x, d.y }; }
	void SetVelocity(glm::vec3 v) { velocity = { v.x,v.y }; }
	void SetAcceleration(glm::vec3 a) { acceleration = { a.x,a.y }; }
	void SetMassCenter(glm::vec3 mc) { massCenter = { mc.x,mc.y }; }
	void SetDensity(float d)             { density = d            ; }
	void SetMass(float m)                { mass = m               ; }
	void SetAngle(float a)               { angle = a              ; }
	void SetAngularVelocity(float w)     { angularVelocity = w    ; }
	void SetAngularAcceleration(float e) { angularAcceleration = e; }
	
protected:
	void Move(glm::vec3 position)          ;         // Перемещение в соответствии с вектором скорости и временем кадра
	void Rotate(float angle)               ;         // Поворот относительно центра масс по времени
	glm::vec3 CalculateMassCenter();			     // Расчет центра масс
    glm::vec3 CalculateMassCenter(Mesh m);           // Универсальный расчет центра масс в зависимости от меша
private:
	float zIndex  = 0.;
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
};


#endif // !IRIGIDBODY_H

