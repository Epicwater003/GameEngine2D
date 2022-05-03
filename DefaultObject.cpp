#include "DefaultObject.h"

void DefaultObject::Draw(Shader& s, Camera & c) {
	
	mesh.Draw(s, c, this->model);
}

void DefaultObject::Update() {
			 // TODO: �������� ���� ���-�� :D
}


Mesh DefaultObject::CreateMesh() {
	std::vector<Vertex> v;
	std::vector<GLuint> i;
	i.push_back(0);
	i.push_back(1);
	i.push_back(2);
	Vertex a = {
		glm::vec3(0.0,0.0,0.0),
		glm::vec2(0.0,0.0),
		GetColor()
	};
	v.push_back(a);
	a = {
		glm::vec3(0.5,1.0,0.0),
		glm::vec2(0.5,1.0),
		GetColor()
	};
	v.push_back(a);
	a = {
		glm::vec3(1.0,0.0,0.0),
		glm::vec2(1.0,0.0),
		GetColor()
	};
	v.push_back(a);
	
	return Mesh(v,i);
}