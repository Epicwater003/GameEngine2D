#ifndef SAT2D_H
#define SAT2D_H

#include <glm/glm.hpp>
#include "ICollisionEngine.h"
#include "IShape.h"
#include "DrawablePrimitive.h"
#include "GameObject.h"
#include "RigidBody2D.h"

struct Edge {
	glm::vec2 point1 = {0,0};
	glm::vec2 point2 = {0,0};
};

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
		


		// ���� ����� �������� � ������� �����������
		// ���� ����� ��������� ��������� �� ������ ����� ��������, �� � ����� ��������(���� ������������ �� ��� �� �����)
		
		// ������� ������� ������ �����(����������� ��������� �� ��� � ����������� �� ����������������, ��. ���������� �������)
		Edge edgeA = GetBestEdge(glm::vec2(collisionProperties.Normal), worldVerticesA);
		Edge edgeB = GetBestEdge(-glm::vec2(collisionProperties.Normal), worldVerticesB);
		glm::vec2 eA = edgeA.point2 - edgeA.point1;
		glm::vec2 eB = edgeB.point2 - edgeB.point1;

		// ����� �����, ������� ����������� ��������������� ���, �� ��������
		Edge refE, incE;
		float flip = 0;
		glm::vec2 refEv, incEv;
		if (abs(glm::dot(eA, glm::vec2(collisionProperties.Normal))) <= abs(glm::dot(eB, glm::vec2(collisionProperties.Normal)))) {
			refEv = eA;
			refE = edgeA;
			incEv = eB;
			incE = edgeB;
		}
		else {
			refEv = eB;
			refE = edgeB;
			incEv = eA;
			incE = edgeA;
			flip = 1; // ���������� ��� ������� ������ ����� ����� �������������
		}

		glm::vec2 refEvn = refEv;
		refEvn = glm::normalize(refEvn);

		float o1 = glm::dot(refEvn, refE.point1);
		// �������� �������������� ���
		std::vector<glm::vec2> cp = Clip(incE.point1, incE.point2, refEvn, o1);

		// ���� ��� 2 �����, �� ������ ��� ������ ������ ����� ��������
		if (cp.size() == 2) {
			float o2 = glm::dot(refEvn,refE.point2);
			std::vector<glm::vec2> cp1 = Clip(cp[0], cp[1], -refEvn, -o2);

			// ���� ��� 2 �����, �� ������ ��� ������ ������ ����� ��������
			if (cp1.size() == 2){
				std::vector<glm::vec2> cp2;
				// ����� ������� � �������� ���
				glm::vec2 refNorm = {-refEvn.y, refEvn.x};
				//refNorm *= flip;
				// ��������� ����� �� ����� �� �������� ����
				float max = glm::dot(refNorm, refE.point2);
				float depthOfp1 = glm::dot(refNorm, cp1[0]) - max;
				float depthOfp2 = glm::dot(refNorm, cp1[1]) - max;
				// ������ �������� �����, ����� ������ ������������ ����� ��������
				if (depthOfp1 >= 0. && abs(collisionProperties.penetration - depthOfp1) <= 0.00001) { // TODO: ������� ������ �������� � ������� isEqual. ��������� ��� ������ �����
					cp2.push_back(cp1[0]);
				}
				if (depthOfp2 >= 0. && abs(collisionProperties.penetration - depthOfp2) <= 0.00001) { // TODO: ������� ������ �������� � ������� isEqual. ��������� ��� ������ �����
					cp2.push_back(cp1[1]);
				}				


				// ����������� �������� ����� ��������
				if(cp2.size() == 2){
					glm::vec3 middle = glm::vec3((cp2[0] + cp2[1]), 0.f) / 2.f;
					glm::vec3 rm = glm::vec3((glm::vec2(middle) + cp2[1]), 0.f) / 2.f;
					glm::vec3 lm = glm::vec3((glm::vec2(middle) + cp2[0]), 0.f) / 2.f;
					//cp2[0] = glm::vec2(middle);
					glm::vec3 pA1, pA2, pB1, pB2;

					if (flip) {
						pA1 = glm::vec3(cp2[0], 0);
						pA2 = glm::vec3(cp2[1], 0);

						pB1 = glm::vec3(cp2[0] - glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
						pB2 = glm::vec3(cp2[1] - glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
					}

					else {
						pB1 = glm::vec3(cp2[0], 0);
						pB2 = glm::vec3(cp2[1], 0);

						pA1 = glm::vec3(cp2[0] + glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
						pA2 = glm::vec3(cp2[1] + glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
					}



					collisionProperties.CollidePointsA.push_back(pA1);
					collisionProperties.CollidePointsA.push_back(pA2);

					collisionProperties.CollidePointsB.push_back(pB1);
					collisionProperties.CollidePointsB.push_back(pB2);


					
				}
				else if(cp2.size() == 1)
				{
					glm::vec3 pA1, pB1;
					if (flip) {
						pA1 = glm::vec3(cp2[0], 0);
						pB1 = glm::vec3(cp2[0] - glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
					}

					else{
						pB1 = glm::vec3(cp2[0], 0);
						pA1 = glm::vec3(cp2[0] + glm::vec2(collisionProperties.Normal * collisionProperties.penetration), 0);
					}
					
					collisionProperties.CollidePointsA.push_back(pA1);
					collisionProperties.CollidePointsB.push_back(pB1);				

				}
			}
		}

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
			if (penetration <= outProjDet.penetration) {
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
	Edge GetBestEdge(glm::vec2 axis, std::vector<glm::vec2>& worldVertices) {
		// ���� �������� �������� �����
		int pointsNum = worldVertices.size();
		float maxDist = -FLT_MAX;
		int index = 0;
		for (int i = 0; i < pointsNum; i++) {
			float k = glm::dot(worldVertices[i], axis);
			if (maxDist < k && abs(maxDist - k) > 0.00001) { // TODO: ������� ������ �������� � ������� isEqual. ��������� ��� ������ �����
				maxDist = k;
				index = i;
			}

		}
		// ������� ��� ����� ���������� �������� �������� �����
		glm::vec2 point = worldVertices[index];
		glm::vec2 prevPoint = worldVertices[((index - 1)==-1)? pointsNum-1 : ((index - 1)% pointsNum)]; // ������ ������� ���������� �� ������ �� ������� �������
		glm::vec2 nextPoint = worldVertices[(index + 1) % pointsNum];

		glm::vec2 fEdge = point - prevPoint;
		glm::vec2 sEdge = point - nextPoint;

		// ����������� ��� ����������� ���������
		fEdge = glm::normalize(fEdge);
		sEdge = glm::normalize(sEdge);

		if (glm::dot(fEdge, axis) <= glm::dot(sEdge, axis)) {
			Edge e = { prevPoint, point };
			return e;
		}
		Edge e = { point, nextPoint };
		return e;
	}
	std::vector<glm::vec2> Clip(glm::vec2 p1, glm::vec2 p2, glm::vec2 axis, float k) {
		std::vector<glm::vec2> clippedPoints;
		// ����� �������� �� ��� � ������� �� �� �������� ���������
		float d1 = glm::dot(p1, axis) - k;
		float d2 = glm::dot(p2, axis) - k;

		if (d1 >= 0.0) clippedPoints.push_back(p1);  // ���� ����� ����� � �� ���������� �������, �� ��������� �
		if (d2 >= 0.0) clippedPoints.push_back(p2);  // ���� ����� ����� � �� ���������� �������, �� ��������� �
		

		if (d1 * d2 < 0.0) {
			// ���� ����� ���� �� ������ ������� �� ���������(�.�. �� ������� 1 �����)
			// �� ����������� ����� �����
			// ������ �� ������� ����� �����
			glm::vec2 e = p2 - p1;
			// ���������� 
			float u = d1 / (d1 - d2);
			e *= u;
			// ����������� � �������
			e += p1;
			clippedPoints.push_back(e);
		}
		return clippedPoints;
	}

};


#endif // !SAT2D_H
