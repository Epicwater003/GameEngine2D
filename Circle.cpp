#include "Circle.h"

Circle::Circle(float radius, float resolution):
	IGameObject(),
	radius(radius),
	resolution(resolution)
{
	ObjectContract->SetMesh(CreateCircleMesh(resolution));
}

Circle::Circle(std::vector<Texture>& textures, float radius, float resolution):
	IGameObject(),
	radius(radius),
	resolution(resolution) 
{
	ObjectContract->SetMesh(CreateCircleMesh(resolution));
	//mesh.AttachTextures(textures);
}

void Circle::Update() {
	float angle = BodyContract->GetAngle();
	glm::vec2 position = BodyContract->GetPosition();
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(position.x,position.y, 0));
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	ObjectContract->SetModelMatrix(model);
}


Mesh Circle::CreateCircleMesh(float resolution) {

	std::vector<Vertex> vertices;
	Vertex vert = {
		glm::vec3(0.,0.,0.),
		glm::vec2(0.5,0.5) ,
		this->ObjectContract->GetColor()
	};

	vertices.push_back(vert);
	for (float i = 0; i < 359; i+=180/resolution) {
		float x = glm::cos(glm::radians(i));
		float y = glm::sin(glm::radians(i));
		float u = 0.5 + (x) / (2 * radius);
		float v = 0.5 + (y) / (2 * radius);
		vert = {
			glm::vec3(x,y,0.),
			glm::vec2(u,v),
			this->ObjectContract->GetColor()
		};
		vertices.push_back(vert);
	}
	

	std::vector<GLuint> indices;
	for (GLuint i = 1, s = vertices.size()-1;  i < s; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);
	}
	indices.push_back(0);
	indices.push_back(vertices.size() - 1);
	indices.push_back(1);
	

	return Mesh(vertices, indices);
}
glm::vec2 Circle::CalculateMassCenter() {
	return glm::vec2(0);
}