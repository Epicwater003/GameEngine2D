#ifndef SQUARE_H
#define SQUARE_H

#include "GameObject.h"
#include "IShape.h"
#include "IBody.h"

class Square :
	public GameObject
{
public:
	Square(float a, float b):GameObject(),width(a), height(b) {
        Shape->SetMesh(*CreateSquareMesh(a, b));
	};
    float width;
    float height;

    void Reshape() {
        Mesh m = *CreateSquareMesh(width, height);
        std::vector<Vertex> v(m.getVertices());
        std::vector<GLuint> i(m.getIndices());

        Shape->SetIndices(i);
        Shape->SetVertices(v);
    };
    std::unique_ptr<Mesh> CreateMesh() { return CreateSquareMesh(width, height); };
    std::unique_ptr<Mesh> CreateSquareMesh(float a, float b) 
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        
        Vertex vert;

        vert = {
            glm::vec3(-a/2.f,-b/2.f,0.),
            glm::vec2(-a / 2.f,-b / 2.f),
            this->Shape->GetColor()
        };
        vertices.push_back(vert);

        vert = {
            glm::vec3(a / 2.f,-b / 2.f,0.),
            glm::vec2(a / 2.f,-b / 2.f),
            this->Shape->GetColor()
        };
        vertices.push_back(vert);

        vert = {
           glm::vec3(a / 2.f,b / 2.f,0.),
           glm::vec2(a / 2.f,b / 2.f),
           this->Shape->GetColor()
        };
        vertices.push_back(vert);

        vert = {
           glm::vec3(-a / 2.f,b / 2.f,0.),
           glm::vec2(-a / 2.f,b / 2.f),
           this->Shape->GetColor()
        };
        vertices.push_back(vert);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        return std::make_unique<Mesh>(vertices, indices);
    }
};


#endif // !SQUARE_H


