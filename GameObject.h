#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

#include <memory>

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
		Body(std::make_unique<RigidBody2D>()),
		Shape(std::make_unique<DefaultShape>())
	{}
	
	std::unique_ptr<IBody>  Body = std::make_unique<IBody>();
	std::unique_ptr<IShape> Shape = std::make_unique<IShape>();

	virtual void Reshape(){};

	virtual void Draw(Shader& s, Camera& c) { Shape->Draw(s, c)         ; }
	virtual void Update(float dt);                           // { Shape->Update(); }
	virtual std::unique_ptr<Mesh> CreateMesh()               { return Shape->CreateMesh(); }
	std::unique_ptr<IShape> CreateShape()                   { return Shape->CreateShape(); }

	virtual glm::vec3 CalculateMassCenter()    { return Body->CalculateMassCenter(*Shape); }
	virtual glm::vec3 CalculateMassCenter(IShape& sh) { return Body->CalculateMassCenter(sh); }

	//IShape   GetShape()       { return Shape->GetShape()      ; }
	Mesh     GetMesh()        { return Shape->GetMesh()       ; }
	glm::mat4 GetModelMatrix() { return Shape->GetModelMatrix(); }
	glm::vec3 GetColor()       { return Shape->GetColor()      ; }

	std::vector<Vertex> GetVertices() { return Shape->GetVertices(); }
	std::vector<GLuint> GetIndices() { return Shape->GetIndices(); }
	std::vector<Texture> GetTextures() { return Shape->GetTextures(); }

	//IBody*    GetBody()         { return Body->GetBody(); }
	glm::vec3 GetPosition()     { return Body->GetPosition()    ; }
	glm::vec3 GetDirection()    { return Body->GetDirection()   ; }
	glm::vec3 GetVelocity()     { return Body->GetVelocity()    ; }
	glm::vec3 GetAcceleration() { return Body->GetAcceleration(); }
	glm::vec3 GetMassCenter()   { return Body->GetMassCenter()  ; }
	glm::vec3 GetForce() { return Body->GetForce(); }
	float GetDensity()             { return Body->GetDensity()            ; }
	float GetRestitution()         { return Body->GetRestitution()        ; }
	float GetMass()                { return Body->GetMass()               ; }
	float GetAngle()               { return Body->GetAngle()              ; }
	float GetAngularVelocity()     { return Body->GetAngularVelocity()    ; }
	float GetAngularAcceleration() { return Body->GetAngularAcceleration(); }
	float GetTorque() { return Body->GetTorque(); }
	float GetMomentOfInertia() { return Body->GetMomentOfInertia(); }

	void SetShape(IShape& shape)       { Shape = shape.CreateShape(); }
	void SetMesh(Mesh& mesh)            { Shape->SetMesh(mesh)              ; }
	void SetModelMatrix(glm::mat4 mat) { Shape->SetModelMatrix(mat)        ; }
	void SetColor(glm::vec3 col)       { Shape->SetColor(col)              ; }

	void SetVertices(std::vector<Vertex>& v) { Shape->GetMesh().setVertices(v); }
	void SetIndices(std::vector<GLuint>& i) { Shape->SetIndices(i); }
	void SetTextures(std::vector<Texture>& t) { Shape->SetTextures(t); }

	void SetBody(IBody& body)         { Body->SetBody(body)     ; }
	void SetPosition(glm::vec3 p)     { Body->SetPosition(p)    ; }
	void SetDirection(glm::vec3 d)    { Body->SetDirection(d)   ; }
	void SetVelocity(glm::vec3 v)     { Body->SetVelocity(v)    ; }
	void SetAcceleration(glm::vec3 a) { Body->SetAcceleration(a); }
	void SetMassCenter(glm::vec3 mc)  { Body->SetMassCenter(mc) ; }
	void SetForce(glm::vec3 f) { Body->SetForce(f); }
	void SetDensity(float d)             { Body->SetDensity(d)            ; }
	void SetRestitution(float r)         { Body->SetRestitution(r)        ; }
	void SetMass(float m)                { Body->SetMass(m)               ; }
	void SetAngle(float a)               { Body->SetAngle(a)              ; }
	void SetAngularVelocity(float w)     { Body->SetAngularVelocity(w)    ; }
	void SetAngularAcceleration(float e) { Body->SetAngularAcceleration(e); }
	void SetTorque(float t) { Body->SetTorque(t); }
	void SetMomentOfInertia(float mi) { Body->SetMomentOfInertia(mi); }

	void AddPosition(glm::vec3 p)     { Body->AddPosition(p)    ; }
	void AddDirection(glm::vec3 d)    { Body->AddDirection(d)   ; }
	void AddVelocity(glm::vec3 v)     { Body->AddVelocity(v)    ; }
	void AddAcceleration(glm::vec3 a) { Body->AddAcceleration(a); }
	void AddMassCenter(glm::vec3 mc)  { Body->AddMassCenter(mc) ; }
	void AddForce(glm::vec3 f) { Body->AddForce(f); }
	void AddDensity(float d)             { Body->AddDensity(d)            ; }
	void AddRestitution(float r)         { Body->AddRestitution(r)        ; }
	void AddMass(float m)                { Body->AddMass(m)               ; }
	void AddAngle(float a)               { Body->AddAngle(a)              ; }
	void AddAngularVelocity(float w)     { Body->AddAngularVelocity(w)    ; }
	void AddAngularAcceleration(float e) { Body->AddAngularAcceleration(e); }
	void AddTorque(float t) { Body->AddTorque(t); }
	void AddMomentOfInertia(float mi) { Body->AddMomentOfInertia(mi); }

protected:
private:
};


#endif // !GAMEOBJECT2D
