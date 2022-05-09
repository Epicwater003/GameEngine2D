#ifndef CIRCLE_H
#define CIRCLE_H

#include "GameObject.h"

#include "IShape.h"
#include "DefaultShape.h"
#include "IBody.h"
#include "RigidBody2D.h"


class Circle :
    public GameObject
{
public:
    // ������������
    Circle(float radius = 1., float resolution = 7);
    Circle(std::vector<Texture> &textures, float radius = 1., float resolution = 7);
    // ���������
    float radius = 1.;
    float resolution = 7.;
    // ���������
    
    // ������
    void Reshape() { Shape->SetMesh(CreateCircleMesh(resolution)); }


    void Update();
    Mesh CreateMesh() { return CreateCircleMesh(resolution); }

protected:
    GameObject* Create() { return new Circle(); };
    glm::vec3 CalculateMassCenter();
private:
    Mesh CreateCircleMesh(float resolution);

};

#endif // !CIRCLE_H