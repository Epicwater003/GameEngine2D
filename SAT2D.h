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



class SAT2D:
	public INarrowPhase
{

public:

	CollisionDetails GetCollisionProperties(IShape& a, IShape& b) {
		CollisionDetails collisionProperties = { false, FLT_MAX };
		
		auto verticesA = a.GetVertices();
		auto modelMatA = a.GetModelMatrix();

		// ����������� ������������� ���������� ����� � ������� ���������� ����� ��� ���� A
		std::vector<glm::vec2> worldVerticesA;
		for (auto& v : verticesA) {
			worldVerticesA.push_back(glm::vec2(modelMatA * glm::vec4(v.position,1)));
		}

		auto verticesB = b.GetVertices();
		auto modelMatB = b.GetModelMatrix();

		// ����������� ������������� ���������� ����� � ������� ���������� ����� ��� ���� B
		std::vector<glm::vec2> worldVerticesB;
		for (auto& v : verticesB) {
			worldVerticesB.push_back(glm::vec2(modelMatB * glm::vec4(v.position, 1)));
		}

		// ������� ����������� ������������� ���
		ProjectionDetails minProjDetA = GetMinimalPenetration(worldVerticesA, worldVerticesB);

		// ���� ������������� ��� == 0, �� ���� �� ������������, �.�. ���������� ����������� ���
		if(!minProjDetA.penetration) {
			return CollisionDetails();
		}
		ProjectionDetails minProjDetB = GetMinimalPenetration(worldVerticesB, worldVerticesA);

		// ���� ������������� ��� == 0, �� ���� �� ������������, �.�. ���������� ����������� ���
		if (!minProjDetB.penetration) {
			return CollisionDetails();
		}

		// ����������� ���
		minProjDetA.axis = glm::normalize(minProjDetA.axis);
		minProjDetB.axis = glm::normalize(minProjDetB.axis);

		// �� �� ���������� ����������� ���, ���� ������������
		collisionProperties.isCollide = 1;

		// ��� ���������� ����������� ������������� ���
		if (minProjDetA.penetration < minProjDetB.penetration) {
			collisionProperties.penetration = minProjDetA.penetration;
			// ����������� ����������� ����������� ���
			collisionProperties.Normal = -glm::vec3(minProjDetA.axis, 0);
		}
		else
		{
			collisionProperties.penetration = minProjDetB.penetration;
			// ����������� ����������� ����������� ���
			collisionProperties.Normal = glm::vec3(minProjDetB.axis, 0);
		}
		// 
		//collisionProperties.Normal = glm::normalize(collisionProperties.Normal); ��� ������������� ���� �� ����
		collisionProperties.ReactionA = -collisionProperties.Normal;
		collisionProperties.ReactionB = collisionProperties.Normal;

		return collisionProperties;
	}

protected:

	ProjectionDetails GetMinimalPenetration(std::vector<glm::vec2> verticesA, std::vector<glm::vec2> verticesB) {

		ProjectionDetails outProjDet;
		// ������������� ������������ ������, ������ ��� ���� �����������
		outProjDet.penetration = FLT_MAX;

		// ���� ����������� ���
		for (int i = 0, s = verticesA.size(); i < s; i++) {
			glm::vec2 firstPoint = verticesA[i];
			glm::vec2 secondPoint = verticesA[(i + 1) % s];

			// ����� ������ ����� ����� �������
			glm::vec2 line = secondPoint - firstPoint;
			//glm::vec2 lineCenter = (secondPoint + firstPoint) / 2.f; ����� ����� ��� ������ �������� ���������� ���������� )

			// ������� ������� � ������� - ���� ���
			glm::vec2 axis = GetNormalToVector(line);

			// ������� ����������� �������� ����� �� ��� ���
			ProjectionDetails projDet = GetProjectionDetails(axis, verticesA, verticesB);

			// ��������� �������������
			float penetration = projDet.projection.max - projDet.projection.min;

			// ���� ������������� <= 0, �� ������ ��� ������ ������������
			if (penetration <= 0.0) {
				return ProjectionDetails();
			}
			// ����������� �������� ��� ����������� ���������(axis ����� ���� ������ �����)
			penetration /= glm::length(axis);
			// ���������� ��������
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

	ProjectionDetails GetProjectionDetails(glm::vec2 line, std::vector<glm::vec2> verticesA, std::vector<glm::vec2> verticesB) {
		// ���� ������������ �������� ����� ���� �� ������ (���� �������)
		ProjectionDetails projDetA = GetAxisProjection(line, verticesA);
		ProjectionDetails projDetB = GetAxisProjection(line, verticesB);
		ProjectionDetails outDetails = { Projection{0, 0} };

		// ���� �������� �� �����������, �� ��� �� ��������
		if (projDetA.projection.max < projDetB.projection.min || projDetB.projection.max < projDetA.projection.min)
			return outDetails;

		outDetails.projection = { projDetA.projection.min, projDetB.projection.max };
		outDetails.minPoint = projDetA.minPoint;
		outDetails.maxPoint = projDetB.maxPoint;

		return outDetails;
	}

	ProjectionDetails GetAxisProjection(glm::vec2& line, std::vector<glm::vec2> vertices) {
		Projection proj = { FLT_MAX, -FLT_MAX };
		ProjectionDetails projDet;

		for (auto& v : vertices) {
			float projection = GetPointProjection(v, line);

			if (projection < proj.min) {
				proj.min = projection;
				projDet.minPoint = { v.x, v.y };
			}
			if (projection > proj.max) {
				proj.max = projection;
				projDet.maxPoint = { v.x, v.y };
			}
		}
		projDet.projection = proj;
		return projDet;
	}

private:
	DrawablePrimitive::Point p;
	DrawablePrimitive::Line l;

	glm::vec2 GetNormalToVector(glm::vec2 line) { // TODO: ������� � ����� Math
		return glm::vec2(line.y, -line.x);
	}
	float GetPointProjection(glm::vec2 point, glm::vec2 line) { // TODO: ������� � ����� Math
		return glm::dot(point, line); // ���������� ����� �� ������ line
	}

};


#endif // !SAT2D_H
