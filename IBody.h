#ifndef IBODY_H
#define IBODY_H

#include <glm/glm.hpp>

class IBody
{
public:
	
	//virtual IBody*    GetBody()         = 0;
	virtual glm::vec3 GetPosition()     = 0;
	virtual glm::vec3 GetDirection()    = 0;
	virtual glm::vec3 GetVelocity()     = 0;
	virtual glm::vec3 GetAcceleration() = 0;
	virtual glm::vec3 GetMassCenter()   = 0;
	virtual float GetDensity()          = 0;
	virtual float GetMass()             = 0;
	virtual float GetAngle()            = 0;
	virtual float GetAngularVelocity()  = 0;
	virtual float GetAngularAcceleration() = 0;

	virtual void SetBody(IBody&)               = 0;
	virtual void SetPosition(glm::vec3)        = 0;
	virtual void SetDirection(glm::vec3)       = 0;
	virtual void SetVelocity(glm::vec3)        = 0;
	virtual void SetAcceleration(glm::vec3)    = 0;
	virtual void SetMassCenter(glm::vec3)      = 0;
	virtual void SetDensity(float)             = 0;
	virtual void SetMass(float)                = 0;
	virtual void SetAngle(float)               = 0;
	virtual void SetAngularVelocity(float)     = 0;
	virtual void SetAngularAcceleration(float) = 0;

	virtual void Move(glm::vec3 position)   = 0;		   // Перемещение в точку
	virtual void Rotate(float angle)        = 0;  		   // Поворот относительно центра
	virtual glm::vec3 CalculateMassCenter() = 0;           // Расчет центра масс
};


#endif // IBODY_H
// Для простоты разработки реализаций контракта
//glm::vec2 GetPosition() { return position; }
//glm::vec2 GetDirection() { return direction; }
//glm::vec2 GetVelocity() { return velocity; }
//glm::vec2 GetAcceleration() { return acceleration; }
//glm::vec2 GetMassCenter() { return massCenter; }
//float GetDensity() { return density; }
//float GetMass() { return mass; }
//float GetAngle() { return angle; }
//float GetAngularVelocity() { return angularVelocity; }
//float GetAngularAcceleration() { return angularAcceleration; }
//
//void SetPosition(glm::vec2 p) { position = p; }
//void SetDirection(glm::vec2 d) { direction = d; }
//void SetVelocity(glm::vec2 v) { velocity = v; }
//void SetAcceleration(glm::vec2 a) { acceleration = a; }
//void SetMassCenter(glm::vec2 mc) { massCenter = mc; }
//void SetDensity(float d) { density = d; }
//void SetMass(float m) { mass = m; }
//void SetAngle(float a) { angle = a; }
//void SetAngularVelocity(float w) { angularVelocity = w; }
//void SetAngularAcceleration(float e) { angularAcceleration = e; }

// Для простоты разработки объекта использующего реализацию
// IBody2D* BodyContract = new RigidBody2D();
//glm::vec2 GetPosition() { return BodyContract->GetPosition(); }
//glm::vec2 GetDirection() { return BodyContract->GetDirection(); }
//glm::vec2 GetVelocity() { return BodyContract->GetVelocity(); }
//glm::vec2 GetAcceleration() { return BodyContract->GetAcceleration(); }
//glm::vec2 GetMassCenter() { return BodyContract->GetMassCenter(); }
//float GetDensity() { return BodyContract->GetDensity(); }
//float GetMass() { return BodyContract->GetMass(); }
//float GetAngle() { return BodyContract->GetAngle(); }
//float GetAngularVelocity() { return BodyContract->GetAngularVelocity(); }
//float GetAngularAcceleration() { return BodyContract->GetAngularAcceleration(); }
//
//void SetPosition(glm::vec2 p) { BodyContract->SetPosition(p); }
//void SetDirection(glm::vec2 d) { BodyContract->SetDirection(d); }
//void SetVelocity(glm::vec2 v) { BodyContract->SetVelocity(v); }
//void SetAcceleration(glm::vec2 a) { BodyContract->SetAcceleration(a); }
//void SetMassCenter(glm::vec2 mc) { BodyContract->SetMassCenter(mc); }
//void SetDensity(float d) { BodyContract->SetDensity(d); }
//void SetMass(float m) { BodyContract->SetMass(m); }
//void SetAngle(float a) { BodyContract->SetAngle(a); }
//void SetAngularVelocity(float w) { BodyContract->SetAngularVelocity(w); }
//void SetAngularAcceleration(float e) { BodyContract->SetAngularAcceleration(e); }
//
//void Move(glm::vec2 position) {
//    BodyContract->Move(position);
//}
//void Rotate(float angle) {
//    BodyContract->Rotate(angle);
//}