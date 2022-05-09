#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

#include <glm/glm.hpp>

#include "Mesh.h"

#include "IBody.h"
#include "IShape.h"
#include "ICollide.h"
#include "RigidBody2D.h"
#include "DefaultShape.h"

class GameObject:
	public IBody,
	public IShape
{
public:
	GameObject() :
		Body(new RigidBody2D()),
		Shape(new DefaultShape())
	{

	}

	IBody*  Body  = nullptr;
	IShape* Shape = nullptr;
	virtual void Reshape(){};

	virtual void Draw(Shader& s, Camera& c) { Shape->Draw(s, c)         ; }
	virtual void Update()                   { Shape->Update()           ; }
	virtual Mesh CreateMesh()               { return Shape->CreateMesh(); }
	IShape* CreateShape()                   { return Shape->CreateShape(); }

	void Move(glm::vec3 position) { Body->Move(position)       ; }
	void Rotate(float angle)      { Body->Rotate(angle)        ; }
	virtual glm::vec3 CalculateMassCenter()    { return Body->CalculateMassCenter(); }

	IShape*   GetShape()       { return Shape->GetShape()      ; }
	Mesh      GetMesh()        { return Shape->GetMesh()       ; }
	glm::mat4 GetModelMatrix() { return Shape->GetModelMatrix(); }
	glm::vec3 GetColor()       { return Shape->GetColor()      ; }

	IBody*    GetBody()         { return Body->GetBody(); }
	glm::vec3 GetPosition()     { return Body->GetPosition()    ; }
	glm::vec3 GetDirection()    { return Body->GetDirection()   ; }
	glm::vec3 GetVelocity()     { return Body->GetVelocity()    ; }
	glm::vec3 GetAcceleration() { return Body->GetAcceleration(); }
	glm::vec3 GetMassCenter()   { return Body->GetMassCenter()  ; }
	float GetDensity()             { return Body->GetDensity()            ; }
	float GetMass()                { return Body->GetMass()               ; }
	float GetAngle()               { return Body->GetAngle()              ; }
	float GetAngularVelocity()     { return Body->GetAngularVelocity()    ; }
	float GetAngularAcceleration() { return Body->GetAngularAcceleration(); }

	void SetShape(IShape& shape)       { Shape = shape.CreateShape(); }
	void SetMesh(Mesh mesh)            { Shape->SetMesh(mesh)              ; }
	void SetModelMatrix(glm::mat4 mat) { Shape->SetModelMatrix(mat)        ; }
	void SetColor(glm::vec3 col)       { Shape->SetColor(col)              ; }

	void SetBody(IBody& body)         { Body->SetBody(body)     ; }
	void SetPosition(glm::vec3 p)     { Body->SetPosition(p)    ; }
	void SetDirection(glm::vec3 d)    { Body->SetDirection(d)   ; }
	void SetVelocity(glm::vec3 v)     { Body->SetVelocity(v)    ; }
	void SetAcceleration(glm::vec3 a) { Body->SetAcceleration(a); }
	void SetMassCenter(glm::vec3 mc)  { Body->SetMassCenter(mc) ; }
	void SetDensity(float d)             { Body->SetDensity(d)            ; }
	void SetMass(float m)                { Body->SetMass(m)               ; }
	void SetAngle(float a)               { Body->SetAngle(a)              ; }
	void SetAngularVelocity(float w)     { Body->SetAngularVelocity(w)    ; }
	void SetAngularAcceleration(float e) { Body->SetAngularAcceleration(e); }

protected:
private:
};


#endif // !GAMEOBJECT2D
