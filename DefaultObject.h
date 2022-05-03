#ifndef DEFAULTOBJECT_H
#define DEFAULTOBJECT_H

#include <glm/glm.hpp>

#include "IGameObject.h"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class DefaultObject:
	public IGameObject
{
public:

	glm::mat4 model = glm::mat4(1);	         // Матрица модели
	glm::vec3 color = { 0.423,0.411,0.376 }; // Цвет
	Mesh mesh = CreateMesh();                // Возможно можно убрать инициализацию, если запретить создавать экземпляры класса
	
	Mesh GetMesh()             { return mesh;  }
	glm::mat4 GetModelMatrix() { return model; }
	glm::vec3 GetColor()       { return color; }

	void SetMesh(Mesh mesh)            { this->mesh = mesh; }
	void SetModelMatrix(glm::mat4 mat) { this->model = mat; }
	void SetColor(glm::vec3 col)       { this->color = col; }

	void Draw(Shader& s, Camera& c);
	void Update();
	Mesh CreateMesh();

protected:

private:
	
};





#endif // !GAMEOBJECT_H
