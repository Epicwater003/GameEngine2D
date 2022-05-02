#ifndef CIRCLE_H
#define CIRCLE_H

#include "GameObject.h"
#include "IRigidBody.h"
#include "INoPhysical.h"
#include "Mesh.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

class Circle :
    public GameObject
    
{
public:
    Circle(float radius = 1., float resolution = 7);
    Circle(std::vector<Texture> &textures, float radius = 1., float resolution = 7);
    
    float radius = 1.;
    float resolution = 7.;
    Mesh mesh = CreateCircleMesh(resolution);

    void Reshape() { mesh = CreateCircleMesh(resolution);};
    void Draw(Shader& s, Camera& c);

protected:
    GameObject* Create() { return new Circle(); };
    glm::vec2 CalculateMassCenter();

private:
    Mesh CreateCircleMesh(float resolution);

};

#endif // !CIRCLE_H