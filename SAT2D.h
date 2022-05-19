#ifndef SAT2D_H
#define SAT2D_H

#include <glm/glm.hpp>
#include "ICollisionEngine.h"
#include "IShape.h"
#include "DrawablePrimitive.h"

struct Projection {
	float min = FLT_MAX;
	float max = -FLT_MAX;
};

struct ProjectionDetails {
	Projection projection = {FLT_MAX, -FLT_MAX};
	glm::vec2 minPoint = {0.f, 0.f};
	glm::vec2 maxPoint = {0.f, 0.f};
	glm::vec2 axis = { 0.f,0.f };
	float penetration = 0.;
};



class SAT2D
	
{

public:

	CollisionProperties2D isCollide(IShape& a, IShape& b, glm::mat4 m) {
		CollisionProperties2D collisionProperties = { false, FLT_MAX };
		
		auto verticesA = a.GetVertices();
		auto modelMatA = a.GetModelMatrix();

		std::vector<glm::vec2> worldVerticesA;
		for (auto& v : verticesA) {
			worldVerticesA.push_back(glm::vec2(modelMatA * glm::vec4(v.position,1)));
		}

		auto verticesB = b.GetVertices();
		auto modelMatB = b.GetModelMatrix();

		std::vector<glm::vec2> worldVerticesB;
		for (auto& v : verticesB) {
			worldVerticesB.push_back(glm::vec2(modelMatB * glm::vec4(v.position, 1)));
		}

		ProjectionDetails minProjDetA = GetMinimalPenetration(worldVerticesA, modelMatA, worldVerticesB, modelMatB, true, m,glm::vec3(0.,1.,0.));
		ProjectionDetails minProjDetB = GetMinimalPenetration(worldVerticesB, modelMatB, worldVerticesA, modelMatA, true, m, glm::vec3(1., 0., 0.));

		if (!minProjDetA.penetration || !minProjDetB.penetration) {
			return CollisionProperties2D();
		}


		minProjDetA.axis = glm::normalize(minProjDetA.axis);
		minProjDetB.axis = glm::normalize(minProjDetB.axis);

		collisionProperties.isCollide = 1;

		if (minProjDetA.penetration < minProjDetB.penetration) {
			collisionProperties.penetration = minProjDetA.penetration;
			collisionProperties.Normal = glm::normalize(minProjDetA.axis);

			l.setColor(glm::vec3(0., 1., 1.));
			l.setPoints(glm::vec3(0.), -glm::vec3(collisionProperties.Normal, 0) * collisionProperties.penetration);
			l.Draw(m * glm::translate(glm::mat4(1), glm::vec3(minProjDetA.maxPoint, 0)));

			l.setPoints(glm::vec3(minProjDetA.minPoint, 0), glm::vec3(minProjDetA.maxPoint, 0));
			l.setColor(glm::vec3(0., 1., 0.));
			l.Draw(m);
		}
		else if(minProjDetB.penetration < minProjDetA.penetration)
		{
			collisionProperties.penetration = minProjDetB.penetration;
			collisionProperties.Normal = glm::normalize(minProjDetB.axis);

			l.setColor(glm::vec3(0., 1., 1.));
			l.setPoints(glm::vec3(0.), -glm::vec3(collisionProperties.Normal, 0) * collisionProperties.penetration);
			l.Draw(m * glm::translate(glm::mat4(1), glm::vec3(minProjDetB.maxPoint, 0)));

			l.setPoints(glm::vec3(minProjDetB.minPoint, 0), glm::vec3(minProjDetB.maxPoint, 0));
			l.setColor(glm::vec3(1., 0., 0.));
			l.Draw(m);
		}
		else {

			collisionProperties.penetration = minProjDetB.penetration;
			collisionProperties.Normal = glm::normalize(minProjDetB.axis);

			l.setColor(glm::vec3(1., 1., 1.));
			l.setPoints(glm::vec3(0.), -glm::vec3(collisionProperties.Normal, 0) * collisionProperties.penetration);
			l.Draw(m * glm::translate(glm::mat4(1), glm::vec3(minProjDetB.maxPoint, 0)));

			l.setPoints(glm::vec3(minProjDetB.minPoint, 0), glm::vec3(minProjDetB.maxPoint, 0));
			l.setColor(glm::vec3(.5, .5, .5));
			l.Draw(m);
		}

		return collisionProperties;



	}
	glm::vec2 GetNormalToVector(glm::vec2 line) {
		return glm::vec2(line.y, -line.x);
	}
	float GetPointProjection(glm::vec2 point, glm::vec2 line) {
		return glm::dot(point, line); // Координата точки на прямой line
	}
	ProjectionDetails GetAxisProjection(glm::vec2& line, IShape& shape){ // Переписать IShape на vector<vertex> и glm::mat4
		Projection proj = { FLT_MAX, -FLT_MAX };
		ProjectionDetails projDet;
		auto vs = shape.GetVertices();
		auto model = shape.GetModelMatrix();
		for (auto& v : vs) {
			glm::vec2 point = glm::vec2(model * glm::vec4(v.position, 1));
			float projection = GetPointProjection(point, line);

			if (projection <= proj.min){
				proj.min = projection;
				projDet.minPoint = { point.x, point.y};
			}
			if (projection >= proj.max){
				proj.max = projection;
				projDet.maxPoint = { point.x, point.y};
			}
		}
		projDet.projection = proj;
		return projDet;
	}
	ProjectionDetails GetAxisProjection(glm::vec2& line, std::vector<glm::vec2> vertices, glm::mat4 modelMat) { // Переписать IShape на vector<vertex> и glm::mat4
		Projection proj = { FLT_MAX, -FLT_MAX };
		ProjectionDetails projDet;
		
		for (auto& v : vertices) {
			float projection = GetPointProjection(v, line);

			if (projection <= proj.min) {
				proj.min = projection;
				projDet.minPoint = { v.x, v.y };
			}
			if (projection >= proj.max) {
				proj.max = projection;
				projDet.maxPoint = { v.x, v.y };
			}
		}
		projDet.projection = proj;
		return projDet;
	}

	ProjectionDetails GetProjectionDetails(glm::vec2 line, IShape& a, IShape& b) {// Переписать IShape на vector<vertex> и glm::mat4
		ProjectionDetails projDetA = GetAxisProjection(line, a);
		ProjectionDetails projDetB = GetAxisProjection(line, b);
		ProjectionDetails outDetails = { Projection{0, 0} };

		if (projDetA.projection.max < projDetB.projection.min || projDetB.projection.max < projDetA.projection.min)
			return outDetails;

		outDetails.projection = { projDetA.projection.min, projDetB.projection.max };
		outDetails.minPoint = projDetA.minPoint;
		outDetails.maxPoint = projDetB.maxPoint;

		return outDetails;
	}
	ProjectionDetails GetProjectionDetails(glm::vec2 line, std::vector<glm::vec2> verticesA, glm::mat4 modelMatA, std::vector<glm::vec2> verticesB, glm::mat4 modelMatB) {
		ProjectionDetails projDetA = GetAxisProjection(line, verticesA, modelMatA);
		ProjectionDetails projDetB = GetAxisProjection(line, verticesB, modelMatB);
		ProjectionDetails outDetails = { Projection{0, 0} };

		if (projDetA.projection.max < projDetB.projection.min || projDetB.projection.max < projDetA.projection.min)
			return outDetails;

		outDetails.projection = { projDetA.projection.min, projDetB.projection.max };
		outDetails.minPoint = projDetA.minPoint;
		outDetails.maxPoint = projDetB.maxPoint;

		return outDetails;
	}
	ProjectionDetails GetMinimalPenetration(std::vector<glm::vec2> verticesA, glm::mat4 modelMatA, std::vector<glm::vec2> verticesB, glm::mat4 modelMatB, bool debugDraw = false, glm::mat4 toWorldMat = glm::mat4(1), glm::vec3 color = {1,0,0}) {

		ProjectionDetails outProjDet;
		outProjDet.penetration = FLT_MAX;

		for (int i = 0, s = verticesA.size(); i < s; i++) {
			glm::vec2 firstPoint = verticesA[i];
			glm::vec2 secondPoint = verticesA[(i + 1) % s];

			glm::vec2 line = secondPoint - firstPoint;
			glm::vec2 lineCenter = (secondPoint + firstPoint) / 2.f;

			glm::vec2 axis = GetNormalToVector(line);

			ProjectionDetails projDet = GetProjectionDetails(axis, verticesA, modelMatA, verticesB, modelMatB);

			float penetration = projDet.projection.max - projDet.projection.min;

			if (penetration <= 0.0) {
				return ProjectionDetails();
			}
			penetration /= glm::length(axis);

			if(debugDraw){
				l.setPoints(firstPoint, secondPoint);
				l.setColor(color);
				l.Draw(toWorldMat);

				l.setPoints(glm::vec3(0.), 0.1f * glm::vec3(axis, 0) / glm::length(axis));
				l.setColor(color);
				l.Draw(toWorldMat * glm::translate(glm::mat4(1), glm::vec3(lineCenter, 0.f)));
			}
			if (penetration < outProjDet.penetration) {
				outProjDet.projection = projDet.projection; 
				outProjDet.minPoint = projDet.minPoint;
				outProjDet.maxPoint = projDet.maxPoint;
				outProjDet.penetration = penetration;
				outProjDet.axis = axis;
			}
		}
		return outProjDet;
	}

protected:

private:
	DrawablePrimitive::Line l;

};


#endif // !SAT2D_H
