#ifndef SAT2D_H
#define SAT2D_H

#include <glm/glm.hpp>
#include "IShape.h"
#include "DrawablePrimitive.h"

class SAT2D
	
{
	DrawablePrimitive::Line l;
public:
	

	bool isCollide(IShape& a, IShape& b, glm::mat4 m) {
		auto indicesA = a.GetIndices();
		auto indicesB = b.GetIndices();
		auto verticesA = a.GetVertices();
		auto verticesB = b.GetVertices();
		
		glm::vec3 firstPoint;
		glm::vec3 firstPointM;
		glm::vec3 secondPoint;
		glm::vec3 secondPointM;
		glm::vec3 line;
		glm::vec2 normToLine;
		glm::vec3 lineM;
		glm::vec2 normToLineM;
		bool answer = 1;
		for (int i = 1, s = indicesA.size(); i < s; i++) {
			firstPoint = verticesA[indicesA[i - 1]].position;
			firstPointM = glm::vec3(a.GetModelMatrix() * glm::vec4(firstPoint, 1));
			secondPoint = verticesA[indicesA[i]].position;
			secondPointM = glm::vec3(a.GetModelMatrix() * glm::vec4(secondPoint, 1));

			line = firstPoint - secondPoint;
			normToLine = GetNormalToVector(glm::vec2(line));
			lineM = firstPointM - secondPointM;
			normToLineM = GetNormalToVector(glm::vec2(lineM));
			
			// В алгоритме SAT используются нормали к граням, но для чего - не понятно
			/*if (ProjectionOverlaping(normToLineM, a, b)) {
				l.setPoints(firstPointM, secondPointM);
				l.setColor(glm::vec3(0., 1., 0.));
				l.Draw(m);
			}

			if (!ProjectionOverlaping(normToLineM, a, b))
				answer = 0;*/

			// При использовании самих граней как разделяющих осей, все работает нормально
			if (ProjectionOverlaping(glm::vec2(lineM.x, lineM.y), a, b)) {
				l.setPoints(firstPointM, secondPointM);
				l.setColor(glm::vec3(0., 1., 0.));
				l.Draw(m);
			}

			if (!ProjectionOverlaping(glm::vec2(lineM.x, lineM.y), a, b))
				answer = 0;

		}
		for (int i = 1, s = indicesB.size(); i < s; i++) {



			firstPoint = verticesB[indicesB[i - 1]].position;
			firstPointM = glm::vec3(b.GetModelMatrix() * glm::vec4(firstPoint, 1));
			secondPoint = verticesB[indicesB[i]].position;
			secondPointM = glm::vec3(b.GetModelMatrix() * glm::vec4(secondPoint, 1));

			line = firstPoint - secondPoint;
			normToLine = GetNormalToVector(glm::vec2(line));
			lineM = firstPointM - secondPointM;
			normToLineM = GetNormalToVector(glm::vec2(lineM));

			/*if (ProjectionOverlaping(normToLineM, a, b)) {
				l.setPoints(firstPoint, secondPoint);
				l.setColor(glm::vec3(1., 0., 0.));
				l.Draw(m * b.GetModelMatrix());
			}


			if (!ProjectionOverlaping(normToLineM, a, b))
				answer = 0;*/
			
			if(ProjectionOverlaping(glm::vec2(lineM.x, lineM.y), a, b)){
				l.setPoints(firstPoint, secondPoint);
				l.setColor(glm::vec3(1., 0., 0.));
				l.Draw(m * b.GetModelMatrix());
			}


			if (!ProjectionOverlaping(glm::vec2(lineM.x, lineM.y), a, b))
				answer = 0;

		}
		return answer;


	}
	glm::vec2 GetNormalToVector(glm::vec2 line) {
		return glm::vec2(-line.y, line.x);
	}
	glm::vec2 GetPointProjection(glm::vec2 point, glm::vec2 line) {
		glm::vec2 projection;
		float t = glm::dot(point, line) / glm::length(line); // Координата точки на прямой line
		projection.x = t;// *line.x;
		projection.y = t;// *line.y;
		return projection;
	}
	glm::vec2 GetAxisProjection(glm::vec2 line, IShape& shape){ // Переписать IShape на vector<vertex> и glm::mat4
		glm::vec2 proj = { FLT_MAX, -FLT_MAX };
		auto vs = shape.GetVertices();
		auto model = shape.GetModelMatrix();
		for (auto& v : vs) {
			glm::vec4 point = model * glm::vec4(v.position, 1);
			glm::vec2 projection = GetPointProjection(glm::vec2(point.x, point.y), line);
			float dist = projection.x;// * projection.x;
			/*if (projection.x < 0)
				dist = -dist;*/
			if (proj.x > dist)
				proj.x = dist;
			if (proj.y < dist)
				proj.y = dist;
		}
		return proj;
	}

	float ProjectionOverlaping(glm::vec2 line, IShape& a, IShape& b) {// Переписать IShape на vector<vertex> и glm::mat4
		glm::vec2 projA = GetAxisProjection(line, a);
		glm::vec2 projB = GetAxisProjection(line, b);
		
		if (projA.y < projB.x || projB.y < projA.x)
			return false;
		return true;
	}

	float checkCollision(IShape& a, IShape& b) {
		auto indices = a.GetIndices();
		auto vertices = a.GetVertices();

		float answer = 0;

		glm::vec3 firstPoint;
		glm::vec3 firstPointM;
		glm::vec3 secondPoint;
		glm::vec3 secondPointM;
		glm::vec3 line;
		glm::vec3 lineM;

		
		for (int i = 1, s = indices.size(); i < s; i++) {
			firstPoint = vertices[indices[i - 1]].position;
			firstPointM = glm::vec3(a.GetModelMatrix() * glm::vec4(firstPoint, 1));
			secondPoint = vertices[indices[i]].position;
			secondPointM = glm::vec3(a.GetModelMatrix() * glm::vec4(secondPoint, 1));

			line = firstPoint - secondPoint;
			lineM = firstPointM - secondPointM;

			answer = ProjectionOverlaping(glm::vec2(line.x, line.y), a, b);

			if (answer){
				return answer;
			}

		}
		return answer;
	}

};


#endif // !SAT2D_H
