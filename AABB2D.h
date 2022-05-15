#ifndef AABB2D_H
#define AABB2D_H

#include "ICollisionEngine.h"
#include "IShape.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"
#include <glad/glad.h>


#include "Shader.h"
#include "Buffer.h"
#include "Vertex.h"

class AABB2D:
	public ICollisionEngine
{
public:
	
	AxisAligned2D GenerateBoundingBox(IShape& go) {
		model = go.GetModelMatrix();
		std::vector<Vertex> vs = go.GetVertices();

		glm::vec4 v = model * glm::vec4(vs[0].position, 1);

		glm::vec2 max = { v.x , v.y };
		glm::vec2 min = { v.x , v.y };

		
		for (int i = 1, s = vs.size(); i < s; i++) {
			v = model * glm::vec4(vs[i].position, 1);

			if (v.x >= max.x)
				max.x = v.x;
			if (v.x <= min.x)
				min.x = v.x;
			if (v.y >= max.y)
				max.y = v.y;
			if (v.y <= min.y)
				min.y = v.y;
		}

		AxisAligned2D a(min, max);
		return a; 
	};
	bool isCollide(IShape& a, IShape& b) {
		AxisAligned2D aabbA = GenerateBoundingBox(a);
		AxisAligned2D aabbB = GenerateBoundingBox(b);
		CreateMeshA(aabbA);
		CreateMeshB(aabbB);
		bool answer = ( // просто сделать 2 if конструкции с return
			(aabbA.max.x >= aabbB.min.x) && (aabbA.min.x <= aabbB.max.x) &&
			(aabbA.max.y >= aabbB.min.y) && (aabbA.min.y <= aabbB.max.y)
			);
		
		return answer;
	};

	void Draw(Shader& s, Camera& c) {
		if(mesha)
			mesha->Draw(s, c, model, true);
		if(meshb)
			meshb->Draw(s, c, model, true);
	}
private:
	void CreateMeshA(AxisAligned2D a) {
		std::vector<Vertex> vs;
		std::vector<GLuint> inds = { 0,1,2,0,2,3 };
		Vertex v;
		v.position = glm::vec3(a.min.x, a.min.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.max.x, a.min.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.max.x, a.max.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.min.x, a.max.y, 0);
		vs.push_back(v);
		mesha = std::make_unique<Mesh>(vs, inds);
	}
	void CreateMeshB(AxisAligned2D a) {
		std::vector<Vertex> vs;
		std::vector<GLuint> inds = { 0,1,2,0,2,3 };
		Vertex v;
		v.position = glm::vec3(a.min.x, a.min.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.max.x, a.min.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.max.x, a.max.y, 0);
		vs.push_back(v);
		v.position = glm::vec3(a.min.x, a.max.y, 0);
		vs.push_back(v);
		meshb = std::make_unique<Mesh>(vs, inds);
	}
	std::unique_ptr<Mesh> mesha = nullptr;
	std::unique_ptr<Mesh> meshb = nullptr;
	glm::mat4 model;
};


#endif 