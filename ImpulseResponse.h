#ifndef IMPULSERESPONSE_H
#define IMPULSERESPONSE_H

#include "IResponseEngine.h"
#include "IBody.h"
#include "IShape.h"

class ImpulseResponse
{
public:
protected:
	glm::vec3 Cross(glm::vec3 v, float a)
	{
		return glm::vec3(a * v.y, -a * v.x, 0.f);
	}

	glm::vec3 Cross(float a, glm::vec3 v)
	{
		return glm::vec3(-a * v.y, a * v.x, 0.f);
	}

	float Cross(glm::vec3 a, glm::vec3 b)
	{
		return a.x * b.y - a.y * b.x;
	}
	virtual glm::vec3 CalculateImpulse(glm::vec3 relativeVelocity, float inverseMass, float collidePoints, float e = 1.f) { // Вычисляем вектор импульса
		glm::vec3 impulse = -(1.f + e) * relativeVelocity;
		impulse /= inverseMass;
		impulse /= collidePoints;
		return impulse;
	}

	virtual glm::vec3 CalculateImpactImpulse(glm::vec3 impulse, glm::vec3 impulseDirection) { 		// Компонента вектора импульса, которая расталкивает объекты
		return glm::dot(impulse, impulseDirection) * impulseDirection;
	}

	virtual glm::vec3 CalculateFrictionImpulse(glm::vec3 impulse, glm::vec3 frictionImpulseDirection, glm::vec3 impactImpulse, float staticFriction = .663f, float dynamicFriction = .336f) {

		// Компонента вектора импульса, которая преобразуется в трение
		glm::vec3 impulseComponentT = glm::dot(impulse, frictionImpulseDirection) * frictionImpulseDirection;

		// Исходя из закона Амона-Кулона расчитываем импульс трения
		if (glm::length(impulseComponentT) < glm::length(impactImpulse) * staticFriction) {
			impulseComponentT *= (float)staticFriction;
		}
		else impulseComponentT *= (float)dynamicFriction;
		return impulseComponentT;
	}

private:
};


class ImpulseResponseNFNR : // NO FRICTION NO ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {
		if (collisionProps.isCollide) {
			assert(collisionProps.CollidePointsA.size() == collisionProps.CollidePointsB.size());
			for (int i = 0; i < collisionProps.CollidePointsA.size(); i++) {

				// Раздвигаем объекты предотвращая проваливание
				const float percent = 0.4f; // обычно от 20% до 80%
				const float slop = 0.05f; // обычно от 0.01 до 0.1
				glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
				a.AddPosition(float(1. / a.GetMass()) * -correction);
				b.AddPosition(float(1. / b.GetMass()) * correction);

				// Вычисляем относительный вектор скорости и относительный вектор позиции
				glm::vec3 Vr = a.GetVelocity() - b.GetVelocity();
				glm::vec3 Cr = b.GetPosition() - a.GetPosition();

				glm::vec3 impulseDirection = collisionProps.Normal;
				float velAlongNormal = glm::dot(Vr, impulseDirection); // Вычислем направление относительной скорости на относительном векторе позиции

				// Если относительная скорость увеличивает расстояние между объектами, то не разрешаем этот тип столкновений
				if (velAlongNormal < 0) {
					return;
				}

				// Получаем средний коэффициент упругости
				float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;
				float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass());

				// Вычисляем вектор импульса для первого тела (для второго будет -impulse)
				glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, collisionProps.CollidePointsA.size(), e);

				// Компонента вектора импульса, которая расталкивает объекты
				glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

				// Прикладываем импульс ( impulse = mass * velocity)
				a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
				b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);
			}
		}


	}
protected:
private:
};


class ImpulseResponseWFNR : // WITH FRICTION NO ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {
		if (collisionProps.isCollide) {
			assert(collisionProps.CollidePointsA.size() == collisionProps.CollidePointsB.size());
			for (int i = 0; i < collisionProps.CollidePointsA.size(); i++) {

				// Раздвигаем объекты предотвращая проваливание
				const float percent = 0.01f; // обычно от 20% до 80%
				const float slop = 0.1f; // обычно от 0.01 до 0.1
				glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (1.f/a.GetMass() + 1.f/b.GetMass()) * percent * collisionProps.Normal;
				a.AddPosition(float(1. / a.GetMass()) * -correction);
				b.AddPosition(float(1. / b.GetMass()) * correction);

				// Вычисляем относительный вектор скорости и относительный вектор позиции
				glm::vec3 Vr = a.GetVelocity() - b.GetVelocity();


				glm::vec3 impulseDirection = collisionProps.Normal;
				float velAlongNormal = glm::dot(Vr, impulseDirection); // Вычислем направление относительной скорости на относительном векторе позиции

				// Если относительная скорость увеличивает расстояние между объектами, то не разрешаем этот тип столкновений
				if (velAlongNormal < 0) {
					return;
				}

				// Получаем средний коэффициент упругости
				float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;

				// Костыль, называемый ограничение
				if (glm::length(Vr) < 1.3) {
					e = 0.0;
				}

				float massA = a.GetMass();
				float massB = b.GetMass();



				float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass());

				// Вычисляем вектор импульса для первого тела (для второго будет -impulse)
				glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, collisionProps.CollidePointsA.size(), e);

				// Компонента вектора импульса, которая расталкивает объекты
				glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

				// Прикладываем импульс ( impulse = mass * velocity)
				a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
				b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);

				// Вычисляем трение
				// Вычисляем вектор касательный к компоненте импульса расталкивающего объекты
				glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection;

				// Если длина этого вектора = 0, то весь импульс уходит в отталкивание объектов, т.е. трения нет
				if (!glm::length(tangent)) {
					return;
				}
				tangent = glm::normalize(tangent);
				glm::vec3 frictionImpulse = CalculateFrictionImpulse(impulse, tangent, impactImpulse, 0.1, 0.07);
				frictionImpulse /= collisionProps.CollidePointsA.size();

				a.AddVelocity(1.f / (float)a.GetMass() * frictionImpulse);
				b.AddVelocity(1.f / (float)b.GetMass() * -frictionImpulse);
				

			}
		}
	}
protected:
private:
};

class ImpulseResponseWFWR : // WITH FRICTION WITH ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {
		if (collisionProps.isCollide) {
			// Раздвигаем объекты предотвращая проваливание
			const float percent = 0.2f; // обычно от 20% до 80%
			const float slop = 0.05f; // обычно от 0.01 до 0.1
			glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
			a.AddPosition(float(1. / a.GetMass()) * -correction);
			b.AddPosition(float(1. / b.GetMass()) * correction);

			assert(collisionProps.CollidePointsA.size() == collisionProps.CollidePointsB.size());
			for (int i = 0; i < collisionProps.CollidePointsA.size(); i++) {

				// Вычисляем относительные точки столкновения
				glm::vec3 contactPointA = collisionProps.CollidePointsA[i] - a.GetPosition();
				glm::vec3 contactPointB = collisionProps.CollidePointsB[i] - b.GetPosition();

				// Относительно центра масс
				glm::vec3 rA = contactPointA - a.GetMassCenter();
				glm::vec3 rB = contactPointB - b.GetMassCenter();

				// Вычисляем относительный вектор скорости и относительный вектор позиции
				glm::vec3 Vr = -b.GetVelocity() + a.GetVelocity() - Cross(b.GetAngularVelocity(), rB) + Cross(a.GetAngularVelocity(), rA);
				glm::vec3 Wr = -Cross(b.GetAngularVelocity(), rB) + Cross(a.GetAngularVelocity(), rA);
				glm::vec3 impulseDirection = collisionProps.Normal;

				float velAlongNormal = glm::dot(Vr, impulseDirection); // Вычислем направление относительной скорости на относительном векторе позиции

				// Если относительная скорость увеличивает расстояние между объектами, то не разрешаем этот тип столкновений
				if (velAlongNormal < 0)
					return;

				// Получаем средний коэффициент упругости
				float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;

				float massA = a.GetMass();
				float massB = b.GetMass();


				// Костыль, называемый ограничение
				if (glm::length(Vr - Wr) < 3.5) {
					e = 0.0;
				}

				float handleA = Cross(rA, impulseDirection);
				float handleB = Cross(rB, impulseDirection);
				float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass()) + handleA * handleA * (1.f / a.GetMomentOfInertia()) + handleB * handleB * (1.f / b.GetMomentOfInertia());
				

				// Вычисляем вектор импульса для первого тела (для второго будет -impulse)
				glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, collisionProps.CollidePointsA.size(), e);

				// Компонента вектора импульса, которая расталкивает объекты
				glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);


				float rotationImpulseA = Cross(rA, impactImpulse);
				float rotationImpulseB = Cross(rB, -impactImpulse);

				// Прикладываем импульс ( impulse = mass * velocity)
				a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
				b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);
				a.AddAngularVelocity((1.f / a.GetMomentOfInertia()) * rotationImpulseA);
				b.AddAngularVelocity((1.f / b.GetMomentOfInertia()) * rotationImpulseB);

				// Вычисляем трение
				// Вычисляем вектор касательный к компоненте импульса расталкивающего объекты

				Vr = -b.GetVelocity() + a.GetVelocity() - Cross(b.GetAngularVelocity(), rB) + Cross(a.GetAngularVelocity(), rA);

				glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection;

				// Если длина этого вектора = 0, то весь импульс уходит в отталкивание объектов, т.е. трения нет
				if (!glm::length(tangent))
					return;

				tangent = glm::normalize(tangent);
				float jt = -glm::dot(Vr, tangent);
				if (abs(jt) <= 0.01)
					return;

				float staticFriction = sqrtf(a.GetStaticFriction() * b.GetStaticFriction());
				float dynamicFriction = sqrtf(a.GetDynamicFriction() * b.GetStaticFriction());
				
					
				glm::vec3 frictionImpulse = CalculateFrictionImpulse(impulse, tangent, impactImpulse, 0.1, 0.07);

				rotationImpulseA = Cross(rA, frictionImpulse);
				rotationImpulseB = Cross(rB, -frictionImpulse);

				a.AddVelocity(1.f / (float)a.GetMass() * frictionImpulse);
				b.AddVelocity(1.f / (float)b.GetMass() * -frictionImpulse);
				a.AddAngularVelocity((1.f / a.GetMomentOfInertia()) * rotationImpulseA);


			}

		}
	}
protected:
private:
	DrawablePrimitive::Line l;
	DrawablePrimitive::Point p;
};

#endif // !IMPULSERESPONSEEFNR_H
