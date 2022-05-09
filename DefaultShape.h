#ifndef DEFAULTOBJECT_H
#define DEFAULTOBJECT_H

#include <glm/glm.hpp>
#include "IShape.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class DefaultShape:
	public IShape
{
public:
	IShape*   GetShape()       { return this; }
	Mesh      GetMesh()        { return mesh;  }
	glm::mat4 GetModelMatrix() { return model; }
	glm::vec3 GetColor()       { return color; }

	void SetShape(IShape& shape) {  
		mesh = shape.GetMesh();
		color = shape.GetColor();
		model = shape.GetModelMatrix();
	}

	void SetMesh(Mesh mesh)            { this->mesh = mesh; }
	void SetModelMatrix(glm::mat4 mat) { this->model = mat; }
	void SetColor(glm::vec3 col)       { this->color = col; }

	void Draw(Shader& s, Camera& c);
	void Update();
	Mesh CreateMesh();
	IShape* CreateShape() { return new DefaultShape(); }

protected:
	
private:
	glm::mat4 model = glm::mat4(1);	         // Матрица модели
	glm::vec3 color = { 0.423,0.411,0.376 }; // Цвет
	Mesh mesh = CreateMesh();                // Возможно можно убрать инициализацию, если запретить создавать экземпляры класса
	
};





#endif // !GAMEOBJECT_H
