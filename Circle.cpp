#include "Circle.h"

Circle::Circle(float radius, float resolution):
	GameObject(),
	radius(radius),
	resolution(resolution)
{
	Shape->SetMesh(*CreateCircleMesh(resolution));
}

Circle::Circle(std::vector<Texture>& textures, float radius, float resolution):
	GameObject(),
	radius(radius),
	resolution(resolution) 
{
	Shape->SetMesh(*CreateCircleMesh(resolution));
}


std::unique_ptr<Mesh> Circle::CreateCircleMesh(float resolution) {
	
	std::vector<Vertex> vertices;
	Vertex vert;

	for (float i = 0; i < 360; i+=180/resolution) {
		float x = glm::cos(glm::radians(i))*radius/2.;
		float y = glm::sin(glm::radians(i))*radius/2.;
		float u = 0.5 + (x) / (2 * radius);
		float v = 0.5 + (y) / (2 * radius);
		vert = {
			glm::vec3(x,y,0.),
			glm::vec2(u,v),
			this->Shape->GetColor()
		};
		vertices.push_back(vert);
	}
	

	std::vector<GLuint> indices;
	for (GLuint i = 1, s = vertices.size()-1;  i < s; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);
	}
	return std::make_unique<Mesh>(vertices, indices);
}
