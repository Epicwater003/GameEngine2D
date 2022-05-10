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

    Circle(float radius = 1., float resolution = 7);
    Circle(std::vector<Texture> &textures, float radius = 1., float resolution = 7);

    float radius = 1.;
    float resolution = 7.;

    void Reshape() { 
        Mesh m = *CreateCircleMesh(resolution);
        std::vector<Vertex> v(m.getVertices());
        std::vector<GLuint> i(m.getIndices());
        
        Shape->SetIndices(i);
        Shape->SetVertices(v);
       
        //Shape->SetMesh(m);
        //Shape->GetMesh().setIndices(i);
        //Shape->GetMesh().ReloadData();
    }

    std::unique_ptr<Mesh> CreateMesh() { return CreateCircleMesh(resolution); }

protected:
    GameObject* Create() { return new Circle(); };
    glm::vec3 CalculateMassCenter();
private:
    std::unique_ptr<Mesh> CreateCircleMesh(float resolution);

};

#endif // !CIRCLE_H