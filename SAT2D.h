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
		


		// Ищем точки контакта в мировых координатах
		// Этот метод позволяет вычислять не только точку контакта, но и пятно контакта(хотя использовать мы это не будем)
		
		// Сначала находим лучшие ребра(максимально удаленные по оси и максимально ей перпендикулярные, см. реализацию функции)
		Edge edgeA = GetBestEdge(glm::vec2(collisionProperties.Normal), worldVerticesA);
		Edge edgeB = GetBestEdge(-glm::vec2(collisionProperties.Normal), worldVerticesB);
		glm::vec2 eA = edgeA.point2 - edgeA.point1;
		glm::vec2 eB = edgeB.point2 - edgeB.point1;

		// Берем ребро, которое максимально перпендикулярно оси, за основное
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
			flip = 1; // Запоминаем что решение задачи нужно будет инвертировать
		}

		glm::vec2 refEvn = refEv;
		refEvn = glm::normalize(refEvn);

		float o1 = glm::dot(refEvn, refE.point1);
		// Обрезаем дополнительную ось
		std::vector<glm::vec2> cp = Clip(incE.point1, incE.point2, refEvn, o1);

		// если нет 2 точек, то дальше нет смысла искать пятно контакта
		if (cp.size() == 2) {
			float o2 = glm::dot(refEvn,refE.point2);
			std::vector<glm::vec2> cp1 = Clip(cp[0], cp[1], -refEvn, -o2);

			// если нет 2 точек, то дальше нет смысла искать пятно контакта
			if (cp1.size() == 2){
				std::vector<glm::vec2> cp2;
				// берем нормаль к основной оси
				glm::vec2 refNorm = {-refEvn.y, refEvn.x};
				//refNorm *= flip;
				// Проверяем лежат ли точки за основной осью
				float max = glm::dot(refNorm, refE.point2);
				float depthOfp1 = glm::dot(refNorm, cp1[0]) - max;
				float depthOfp2 = glm::dot(refNorm, cp1[1]) - max;
				// Вторая проверка нужна, чтобы убрать сомнительные пятна контакта
				if (depthOfp1 >= 0. && abs(collisionProperties.penetration - depthOfp1) <= 0.00001) { // TODO: Вынести вторую проверку в функцию isEqual. Проверить все схожие места
					cp2.push_back(cp1[0]);
				}
				if (depthOfp2 >= 0. && abs(collisionProperties.penetration - depthOfp2) <= 0.00001) { // TODO: Вынести вторую проверку в функцию isEqual. Проверить все схожие места
					cp2.push_back(cp1[1]);
				}				


				// Расчитываем конечные точки контакта
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
	Edge GetBestEdge(glm::vec2 axis, std::vector<glm::vec2>& worldVertices) {
		// Ищем наиболее удаленую точку
		int pointsNum = worldVertices.size();
		float maxDist = -FLT_MAX;
		int index = 0;
		for (int i = 0; i < pointsNum; i++) {
			float k = glm::dot(worldVertices[i], axis);
			if (maxDist < k && abs(maxDist - k) > 0.00001) { // TODO: Вынести вторую проверку в функцию isEqual. Проверить все схожие места
				maxDist = k;
				index = i;
			}

		}
		// Находим все ребра содержащие наиболее удаленую точку
		glm::vec2 point = worldVertices[index];
		glm::vec2 prevPoint = worldVertices[((index - 1)==-1)? pointsNum-1 : ((index - 1)% pointsNum)]; // Святой костыль защищающий от выхода за границы массива
		glm::vec2 nextPoint = worldVertices[(index + 1) % pointsNum];

		glm::vec2 fEdge = point - prevPoint;
		glm::vec2 sEdge = point - nextPoint;

		// Нормализуем для корректного сравнения
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
		// Берем проекции на ось и смещаем их на величину обрезания
		float d1 = glm::dot(p1, axis) - k;
		float d2 = glm::dot(p2, axis) - k;

		if (d1 >= 0.0) clippedPoints.push_back(p1);  // Если точка лежит в не отсеченной области, то оставляем её
		if (d2 >= 0.0) clippedPoints.push_back(p2);  // Если точка лежит в не отсеченной области, то оставляем её
		

		if (d1 * d2 < 0.0) {
			// Если точки были по разные стороны от отсечения(т.е. мы отсекли 1 точку)
			// то расчитываем новую точку
			// Вектор на котором точка лежит
			glm::vec2 e = p2 - p1;
			// Расстояние 
			float u = d1 / (d1 - d2);
			e *= u;
			// Перемещение в позицию
			e += p1;
			clippedPoints.push_back(e);
		}
		return clippedPoints;
	}

};


#endif // !SAT2D_H
