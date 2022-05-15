#ifndef IDRAWABLE_H
#define IDRAWABLE_H

#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>

class IDrawable {
public:

	virtual void Draw() = 0; 
	virtual void Draw(glm::mat4 MWPmat) = 0;
	virtual void Draw(glm::mat4 MWPmat, glm::vec3 color) = 0;
	virtual void Draw(Camera& c) = 0;
	virtual void Draw(Camera& c, glm::vec3 color) = 0;
	virtual void Draw(Shader& s, Camera& c) = 0;
};




#endif // !IDRAWABLE_H
