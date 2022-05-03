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
    Circle(float radius = 1., float resolution = 7);
    Circle(std::vector<Texture> &textures, float radius = 1., float resolution = 7);

    float radius = 1.;
    float resolution = 7.;

    IGameObject* ObjectContract = new DefaultObject();
    IBody2D* BodyContract = new RigidBody2D();

    
    //Mesh mesh = CreateCircleMesh(resolution);

    void Reshape() { ObjectContract->SetMesh(CreateCircleMesh(resolution)); };

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