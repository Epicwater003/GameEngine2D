#ifndef IBODY_H
#define IBODY_H

#include <glm/glm.hpp>
#include "IShape.h"

class IBody
{
public:
	
	//virtual IBody*    GetBody()         = 0;
	virtual glm::vec3 GetPosition()     = 0;
	virtual glm::vec3 GetDirection()    = 0;
	virtual glm::vec3 GetVelocity()     = 0;
	virtual glm::vec3 GetAcceleration() = 0;
	virtual glm::vec3 GetMassCenter()   = 0;
	virtual glm::vec3 GetForce()        = 0;
	virtual float GetDensity()          = 0;
	virtual float GetRestitution()      = 0;
	virtual float GetStaticFriction()   = 0;
	virtual float GetDynamicFriction()  = 0;
	virtual float GetMass()             = 0;
	virtual float GetAngle()            = 0;
	virtual float GetAngularVelocity()  = 0;
	virtual float GetAngularAcceleration() = 0;
	virtual float GetTorque()           = 0;
	virtual float GetMomentOfInertia()  = 0;

	virtual void SetBody(IBody&)               = 0;
	virtual void SetPosition(glm::vec3)        = 0;
	virtual void SetDirection(glm::vec3)       = 0;
	virtual void SetVelocity(glm::vec3)        = 0;
	virtual void SetAcceleration(glm::vec3)    = 0;
	virtual void SetMassCenter(glm::vec3)      = 0;
	virtual void SetForce(glm::vec3)           = 0;
	virtual void SetDensity(float)             = 0;
	virtual void SetRestitution(float)         = 0;
	virtual void SetStaticFriction(float)      = 0;
	virtual void SetDynamicFriction(float)     = 0;
	virtual void SetMass(float)                = 0;
	virtual void SetAngle(float)               = 0;
	virtual void SetAngularVelocity(float)     = 0;
	virtual void SetAngularAcceleration(float) = 0;
	virtual void SetTorque(float)              = 0;
	virtual void SetMomentOfInertia(float)     = 0;

	virtual void AddPosition(glm::vec3)        = 0;
	virtual void AddDirection(glm::vec3)       = 0;
	virtual void AddVelocity(glm::vec3)        = 0;
	virtual void AddAcceleration(glm::vec3)    = 0;
	virtual void AddMassCenter(glm::vec3)      = 0;
	virtual void AddForce(glm::vec3)		   = 0;
	virtual void AddDensity(float)             = 0;
	virtual void AddRestitution(float)         = 0;
	virtual void AddStaticFriction(float)      = 0;
	virtual void AddDynamicFriction(float)     = 0;
	virtual void AddMass(float)                = 0;
	virtual void AddAngle(float)               = 0;
	virtual void AddAngularVelocity(float)     = 0;
	virtual void AddAngularAcceleration(float) = 0;
	virtual void AddTorque(float)              = 0;
	virtual void AddMomentOfInertia(float)     = 0;

	virtual glm::vec3 CalculateMassCenter() = 0;           // Расчет центра масс
	virtual glm::vec3 CalculateMassCenter(IShape&) = 0;           // Расчет центра масс
};


#endif // IBODY_H
