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

		// Преобразуем относительные координаты точек в мировые координаты точек для тела A
		std::vector<glm::vec2> worldVerticesA;
		for (auto& v : verticesA) {
			worldVerticesA.push_back(glm::vec2(modelMatA * glm::vec4(v.position,1)));
		}

		auto verticesB = b.GetVertices();
		auto modelMatB = b.GetModelMatrix();

		// Преобразуем относительные координаты точек в мировые координаты точек для тела B
		std::vector<glm::vec2> worldVerticesB;
		for (auto& v : verticesB) {
			worldVerticesB.push_back(glm::vec2(modelMatB * glm::vec4(v.position, 1)));
		}

		// Находим минимальные проникновения тел
		ProjectionDetails minProjDetA = GetMinimalPenetration(worldVerticesA, worldVerticesB);

		// Если проникновение тел == 0, то тела не сталкиваются, т.к. существует разделяющая ось
		if(!minProjDetA.penetration) {
			return CollisionDetails();
		}
		ProjectionDetails minProjDetB = GetMinimalPenetration(worldVerticesB, worldVerticesA);

		// Если проникновение тел == 0, то тела не сталкиваются, т.к. существует разделяющая ось
		if (!minProjDetB.penetration) {
			return CollisionDetails();
		}

		// Нормализуем оси
		minProjDetA.axis = glm::normalize(minProjDetA.axis);
		minProjDetB.axis = glm::normalize(minProjDetB.axis);

		// Тк не существует разделяющей оси, тела сталкиваются
		collisionProperties.isCollide = 1;

		// Нас интересует минимальное проникновение тел
		if (minProjDetA.penetration < minProjDetB.penetration) {
			collisionProperties.penetration = minProjDetA.penetration;
			// Однозначное определение направления оси
			collisionProperties.Normal = -glm::vec3(minProjDetA.axis, 0);
		}
		else
		{
			collisionProperties.penetration = minProjDetB.penetration;
			// Однозначное определение направления оси
			collisionProperties.Normal = glm::vec3(minProjDetB.axis, 0);
		}
		// 
		//collisionProperties.Normal = glm::normalize(collisionProperties.Normal); Уже нормализована выше по коду
		collisionProperties.ReactionA = -collisionProperties.Normal;
		collisionProperties.ReactionB = collisionProperties.Normal;

		return collisionProperties;
	}

protected:

	ProjectionDetails GetMinimalPenetration(std::vector<glm::vec2> verticesA, std::vector<glm::vec2> verticesB) {

		ProjectionDetails outProjDet;
		// Инициализация максимальным числом, потому что ищем минимальное
		outProjDet.penetration = FLT_MAX;

		// Ищем разделяющую ось
		for (int i = 0, s = verticesA.size(); i < s; i++) {
			glm::vec2 firstPoint = verticesA[i];
			glm::vec2 secondPoint = verticesA[(i + 1) % s];

			// Берем вектор между двумя точками
			glm::vec2 line = secondPoint - firstPoint;
			//glm::vec2 lineCenter = (secondPoint + firstPoint) / 2.f; Будет нужно для вывода красивой отладочной информации )

			// Находим нормаль к вектору - наша ось
			glm::vec2 axis = GetNormalToVector(line);

			// Находим пересечение проекций точек на эту ось
			ProjectionDetails projDet = GetProjectionDetails(axis, verticesA, verticesB);

			// Вычисляем проникновение
			float penetration = projDet.projection.max - projDet.projection.min;

			// Если проникновение <= 0, то дальше нет смысла обрабатывать
			if (penetration <= 0.0) {
				return ProjectionDetails();
			}
			// Нормализуем значения для корректного сравнения(axis может быть разной длины)
			penetration /= glm::length(axis);
			// Записываем значения
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
		// Ищем максимальную проекцию точек тела на прямую (тень объекта)
		ProjectionDetails projDetA = GetAxisProjection(line, verticesA);
		ProjectionDetails projDetB = GetAxisProjection(line, verticesB);
		ProjectionDetails outDetails = { Projection{0, 0} };

		// Если проекции не переекаются, то ось не подходит
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

	glm::vec2 GetNormalToVector(glm::vec2 line) { // TODO: Вынести в класс Math
		return glm::vec2(line.y, -line.x);
	}
	float GetPointProjection(glm::vec2 point, glm::vec2 line) { // TODO: Вынести в класс Math
		return glm::dot(point, line); // Координата точки на прямой line
	}

};


#endif // !SAT2D_H
