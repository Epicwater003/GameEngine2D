#ifndef AABB2D_H
#define AABB2D_H

#include "ICollisionEngine.h"
#include "IShape.h"
#include <glm/glm.hpp>

struct AABoundingBox2D {
	AABoundingBox2D(glm::vec2 min, glm::vec2 max) {
		this->min = min;
		this->max = max;
		
	}
	glm::vec2 min = {0,1};
	glm::vec2 max = {0,0};
};

class AABB2D:
	public ICollisionEngine
{
public:
	
	AABoundingBox2D GenerateBoundingBox(IShape& go) {
		Mesh m = go.GetMesh();
		glm::mat4 modelMat = go.GetModelMatrix();
		std::vector<Vertex> vs = m.getVertices();

		glm::vec4 v = modelMat * glm::vec4(vs[0].position, 1);

		glm::vec2 max = { v.x , v.y };
		glm::vec2 min = { v.x , v.y };
		//std::cout << vs.size() << std::endl;
		
		for (int i = 1, s = vs.size() - 1; i < s; i++) {
			v = modelMat * glm::vec4(vs[i].position, 1);
			if (v.x > max.x)
				max.x = v.x;
			if (v.x < min.x)
				min.x = v.x;
			if (v.y > max.y)
				max.y = v.y;
			if (v.y < min.y)
				min.y = v.y;
		}
		//std::cout << min.x << "" << min.y << std::endl;
		AABoundingBox2D a(min, max);

		//std::cout << "AABB a:" << a.min.x << " " << a.min.y << " " << a.max.x << " " << a.max.y << std::endl;
		return a; 
	};
	bool isPossibleToCollide(IShape& a, IShape& b) {
		AABoundingBox2D aabbA = GenerateBoundingBox(a);
		AABoundingBox2D aabbB = GenerateBoundingBox(b);
		//std::cout << "AABB A:" << aabbA.min.x << " " << aabbA.min.y << " " << aabbA.max.x << " " << aabbA.max.y << std::endl;
		//std::cout << "AABB B:" << aabbB.min.x << " " << aabbB.min.y << " " << aabbB.max.x << " " << aabbB.max.y << std::endl;
		bool answer = (
			(aabbA.max.x >= aabbB.min.x) && (aabbA.min.x <= aabbB.max.x) &&
			(aabbA.max.y >= aabbB.min.y) && (aabbA.min.y <= aabbB.max.y)
			);
		return answer;
	};
};


#endif 