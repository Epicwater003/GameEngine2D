/*
* TODO: Возможно нужно унаследоваться от GameObject
* 
*/

/*
Rock       Density : 0.6  Restitution : 0.1
Wood       Density : 0.3  Restitution : 0.2
Metal      Density : 1.2  Restitution : 0.05
BouncyBall Density : 0.3  Restitution : 0.8
SuperBall  Density : 0.3  Restitution : 0.95
Pillow     Density : 0.1  Restitution : 0.2
Static     Density : 0.0  Restitution : 0.4
*/

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody.h"
#include "IShape.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody 
{
public:

	//IBody*    GetBody()            { return this; }
	glm::vec3 GetPosition()        { return glm::vec3(position.x, position.y, zIndex)   ; }
	glm::vec3 GetDirection()       { return glm::vec3(direction.x,direction.y,0)        ; }
	glm::vec3 GetVelocity()        { return glm::vec3(velocity.x, velocity.y, 0)        ; }
	glm::vec3 GetAcceleration()    { return glm::vec3(acceleration.x, acceleration.y, 0); }
	glm::vec3 GetMassCenter()      { return glm::vec3(massCenter.x,massCenter.y,0)      ; }
	glm::vec3 GetForce()           { return glm::vec3(force.x, force.y, 0)              ; }
	float GetDensity()             { return density            ; }
	float GetRestitution()         { return restitution        ; }
	float GetStaticFriction()      { return staticFriction     ; }
	float GetDynamicFriction()     { return dynamicFriction    ; }
	float GetMass()                { return mass               ; }
	float GetAngle()               { return angle              ; }
	float GetAngularVelocity()     { return angularVelocity    ; }
	float GetAngularAcceleration() { return angularAcceleration; }
	float GetTorque() { return torque; }
	float GetMomentOfInertia() { return momentOfInertia; }

	void SetBody(IBody& body) {
		//position = body.GetPosition(); TODO: Реализовать
	};
	void SetPosition(glm::vec3 p)     { position     = { p.x, p.y }   ; }
	void SetDirection(glm::vec3 d)    { direction    = { d.x, d.y }   ; }
	void SetVelocity(glm::vec3 v)     { velocity     = { v.x, v.y }   ; }
	void SetAcceleration(glm::vec3 a) { acceleration = { a.x, a.y }   ; }
	void SetMassCenter(glm::vec3 mc)  { massCenter   = { mc.x, mc.y } ; }
	void SetForce(glm::vec3 f)        { force        = { f.x, f.y }   ; }
	void SetDensity(float d)             { density = d            ; }
	void SetRestitution(float r)         { restitution = r        ; }
	void SetStaticFriction(float sf)     { staticFriction = sf    ; }
	void SetDynamicFriction(float df)    { dynamicFriction = df   ; }
	void SetMass(float m)                { mass = m               ; }
	void SetAngle(float a)               { angle = a              ; }
	void SetAngularVelocity(float w)     { angularVelocity = w    ; }
	void SetAngularAcceleration(float e) { angularAcceleration = e; }
	void SetTorque(float t) { torque = t; }
	void SetMomentOfInertia(float mi) { momentOfInertia = mi; };


	void AddPosition(glm::vec3 p)     { position.x     += p.x;
	                                    position.y     += p.y;  }
	void AddDirection(glm::vec3 d)    { direction.x    += d.x;
			                            direction.y    += d.y;  }
	void AddVelocity(glm::vec3 v)     { velocity.x     += v.x; 
	                                    velocity.y     += v.y;  }
	void AddAcceleration(glm::vec3 a) { acceleration.x += a.x;
	                                    acceleration.y += a.y;  }
	void AddMassCenter(glm::vec3 mc)  { massCenter.x   += mc.x;
	                                    massCenter.y   += mc.y; }
	void AddForce(glm::vec3 f)        { force.x        += f.x;
	                                    force.y        += f.y;  }
	void AddDensity(float d)             { density += d            ; }
	void AddRestitution(float r)         { restitution += r        ; }
	void AddStaticFriction(float sf)     { staticFriction += sf    ; }
	void AddDynamicFriction(float df)    { dynamicFriction += df   ; }
	void AddMass(float m)                { mass += m               ; }
	void AddAngle(float a)               { angle += a              ; }
	void AddAngularVelocity(float w)     { angularVelocity += w    ; }
	void AddAngularAcceleration(float e) { angularAcceleration += e; }
	void AddTorque(float t) { torque += t; }
	void AddMomentOfInertia(float mi) { momentOfInertia += mi; };
	
protected:

	glm::vec3 CalculateMassCenter();			     // Расчет центра масс
    glm::vec3 CalculateMassCenter(IShape& sh);           // Универсальный расчет центра масс в зависимости от меша
private:

	float mass    = 1.;	                 // Масса в кг
	glm::vec2 position     = { 0., 0. }; // Позиция в м
	glm::vec2 velocity     = { 0., 0. }; // Скорость в м/c
	glm::vec2 acceleration = { 0., 0. }; // Ускорение в м/с^2
	glm::vec2 force        = { 0., 0. }; // Сила force = mass * acceleration

	float momentOfInertia = 1.;          // Момент инерции в кг*м^2
	float angle   = 0.;	                 // Угол поворота тела в рад
	float angularVelocity     = 0.;	     // Угловая скорость в рад/с
	float angularAcceleration = 0.;	     // Угловое ускорение в рад/c^2
	float torque              = 0.;      // Крутящий момент torque = momentOfInertia * angularAcceleration, torque = cross(point.pos, angularVel)
	
	float zIndex = 0.;
	float density = 1.;                  // Плотность кг/м
	float restitution = 0.8;             // Коэффициент упругости
	float staticFriction = 0.663;
	float dynamicFriction = 0.336;
	glm::vec2 direction = { 0., 1. };    // Вектор направления
	glm::vec2 massCenter = { 0., 0. };   // Позиция центра масс https://habr.com/ru/post/579364/

};


#endif // !IRIGIDBODY_H

