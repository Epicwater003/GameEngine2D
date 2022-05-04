#ifndef CIRCLE_H
#define CIRCLE_H

#include "IGameObject.h"
#include "DefaultObject.h"
#include "IBody2D.h"
#include "RigidBody2D.h"
#include "Mesh.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

class Circle :
    public IGameObject,
    public IBody2D
{
public:
    // Конструкторы
    Circle(float radius = 1., float resolution = 7);
    Circle(std::vector<Texture> &textures, float radius = 1., float resolution = 7);
    // Параметры
    float radius = 1.;
    float resolution = 7.;
    // Контракты
    IGameObject* ObjectContract = new DefaultObject();
    IBody2D* BodyContract = new RigidBody2D();
    // Методы
    void Reshape() { ObjectContract->SetMesh(CreateCircleMesh(resolution)); }

    /*========================================*/
    Mesh GetMesh()             { return ObjectContract->GetMesh()       ; }
    glm::mat4 GetModelMatrix() { return ObjectContract->GetModelMatrix(); }
    glm::vec3 GetColor()       { return ObjectContract->GetColor()      ; }

    void SetMesh(Mesh mesh)            { ObjectContract->SetMesh(mesh)      ; }
    void SetModelMatrix(glm::mat4 mat) { ObjectContract->SetModelMatrix(mat); }
    void SetColor(glm::vec3 col)       { ObjectContract->SetColor(col)      ; }

    void Draw(Shader& s, Camera& c) { ObjectContract->Draw(s, c); }
    void Update();
    Mesh CreateMesh() { return CreateCircleMesh(resolution); }
    /*========================================*/
    glm::vec2 GetPosition()        { return BodyContract->GetPosition()           ; }
    glm::vec2 GetDirection()       { return BodyContract->GetDirection()          ; }
    glm::vec2 GetVelocity()        { return BodyContract->GetVelocity()           ; }
    glm::vec2 GetAcceleration()    { return BodyContract->GetAcceleration()       ; }
    glm::vec2 GetMassCenter()      { return BodyContract->GetMassCenter()         ; }
    float GetDensity()             { return BodyContract->GetDensity()            ; }
    float GetMass()                { return BodyContract->GetMass()               ; }
    float GetAngle()               { return BodyContract->GetAngle()              ; }
    float GetAngularVelocity()     { return BodyContract->GetAngularVelocity()    ; }
    float GetAngularAcceleration() { return BodyContract->GetAngularAcceleration(); }

    void SetPosition(glm::vec2 p)        { BodyContract->SetPosition(p)           ; }
    void SetDirection(glm::vec2 d)       { BodyContract->SetDirection(d)          ; }
    void SetVelocity(glm::vec2 v)        { BodyContract->SetVelocity(v)           ; }
    void SetAcceleration(glm::vec2 a)    { BodyContract->SetAcceleration(a)       ; }
    void SetMassCenter(glm::vec2 mc)     { BodyContract->SetMassCenter(mc)        ; }
    void SetDensity(float d)             { BodyContract->SetDensity(d)            ; }
    void SetMass(float m)                { BodyContract->SetMass(m)               ; }
    void SetAngle(float a)               { BodyContract->SetAngle(a)              ; }
    void SetAngularVelocity(float w)     { BodyContract->SetAngularVelocity(w)    ; }
    void SetAngularAcceleration(float e) { BodyContract->SetAngularAcceleration(e); }

    void Move(glm::vec2 position) {
        BodyContract->Move(position);
    }
    void Rotate(float angle) {
        BodyContract->Rotate(angle);
    }
    /*========================================*/
protected:
    IGameObject* Create() { return new Circle(); };
    glm::vec2 CalculateMassCenter();
private:
    Mesh CreateCircleMesh(float resolution);

};

#endif // !CIRCLE_H