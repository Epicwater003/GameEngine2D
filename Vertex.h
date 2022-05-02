#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex{
	glm::vec3 position;
	glm::vec2 texUV;
	glm::vec3 color;
	glm::vec3 normal;
};

#endif // !VERTEX_H

