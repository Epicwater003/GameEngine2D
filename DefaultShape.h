#ifndef DEFAULTOBJECT_H
#define DEFAULTOBJECT_H

#include <memory>
#include <glm/glm.hpp>
#include "IShape.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class DefaultShape:
	public IShape
{
public:
	//IShape   GetShape()       { return *this; }
	Mesh      GetMesh()        { return *mesh; }
	glm::mat4 GetModelMatrix() { return model; }
	glm::vec3 GetColor()       { return color; }

	void SetShape(IShape& shape) {  
		mesh =  std::make_unique<Mesh>(shape.GetMesh());
		color = shape.GetColor();
		model = shape.GetModelMatrix();
	}
	void SetVertices(std::vector<Vertex>& v) { mesh->setVertices(v); }
	void SetIndices(std::vector<GLuint>& i) { mesh->setIndices(i); }
	void SetTextures(std::vector<Texture>& t) { mesh->setTextures(t); }
	std::vector<Vertex> GetVertices() { return mesh->getVertices(); }
	std::vector<GLuint> GetIndices() { return mesh->getIndices(); }
	std::vector<Texture> GetTextures() { return mesh->getTextures(); }

	void SetMesh(Mesh& mesh)            { this->mesh = std::make_unique<Mesh>(mesh); }
	void SetModelMatrix(glm::mat4 mat)  { this->model = mat; }
	void SetColor(glm::vec3 col)        { this->color = col; }

	void Draw(Shader& s, Camera& c);
	void Update(float dt);
	std::unique_ptr<Mesh> CreateMesh();
	std::unique_ptr<IShape> CreateShape() { return std::make_unique<DefaultShape>(); }

protected:
	
private:
	glm::mat4 model = glm::mat4(1);	         // Матрица модели
	glm::vec3 color = { 1., 0.54, 0.41 };    // Цвет
	std::unique_ptr<Mesh> mesh = CreateMesh();               
};





#endif // !GAMEOBJECT_H
