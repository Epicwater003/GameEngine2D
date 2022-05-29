#ifndef IMPULSERESPONSE_H
#define IMPULSERESPONSE_H

#include "IResponseEngine.h"
#include "IBody.h"
#include "IShape.h"

class ImpulseResponse
{
public:
protected:

	virtual glm::vec3 CalculateImpulse(glm::vec3 relativeVelocity, float inverseMass, float e =1.f) { // Вычисляем вектор импульса
		glm::vec3 impulse = -(e + 1.f) * relativeVelocity;
		impulse /= inverseMass;
		return impulse;
	}

	virtual glm::vec3 CalculateImpactImpulse(glm::vec3 impulse, glm::vec3 impulseDirection) { 		// Компонента вектора импульса, которая расталкивает объекты
		return glm::dot(impulse, impulseDirection) * impulseDirection;
	}

	virtual glm::vec3 CalculateFrictionImpulse(glm::vec3 impulse, glm::vec3 frictionImpulseDirection, glm::vec3 impactImpulse, float staticFriction = .663f, float dynamicFriction = .336f) {
		frictionImpulseDirection = glm::normalize(frictionImpulseDirection);

		// Компонента вектора импульса, которая преобразуется в трение
		glm::vec3 impulseComponentT = glm::dot(impulse, frictionImpulseDirection) * frictionImpulseDirection;

		// Исходя из закона Амона-Кулона расчитываем импульс трения
		glm::vec3 frictionImpulse;
		if (glm::length(impulseComponentT) < glm::length(impactImpulse) * staticFriction) {
			return impulseComponentT * staticFriction;
		}
		return impulseComponentT * dynamicFriction;
	}

private:
};


class ImpulseResponseNFNR : // NO FRICTION NO ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {

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
		glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);

		// Компонента вектора импульса, которая расталкивает объекты
		glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

		// Прикладываем импульс ( impulse = mass * velocity)
		a.AddVelocity(1.f / a.GetMass() * impactImpulse);
		b.AddVelocity(1.f / b.GetMass() * -impactImpulse);

		// Раздвигаем объекты на минимальное расстояние, достаточное, чтобы объекты перестали пересекаться
		a.AddPosition(-collisionProps.Normal * collisionProps.penetration / 2.f);
		b.AddPosition(collisionProps.Normal * collisionProps.penetration / 2.f);

	}
protected:
private:
};


class ImpulseResponseWFNR: // WITH FRICTION NO ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {
		if(collisionProps.isCollide){
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
			glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);

			// Компонента вектора импульса, которая расталкивает объекты
			glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

			// Прикладываем импульс ( impulse = mass * velocity)
			a.AddVelocity(1.f / a.GetMass() * impactImpulse);
			b.AddVelocity(1.f / b.GetMass() * -impactImpulse);
		
			// Вычисляем трение
			// Вычисляем вектор касательный к компоненте импульса расталкивающего объекты
			glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection; 
			tangent = glm::vec3(impulseDirection.y, -impulseDirection.x, 0.f);
			// Если длина этого вектора = 0, то весь импульс уходит в отталкивание объектов, т.е. трения нет
			if (!glm::length(tangent)) {
				return;
			}

			glm::vec3 frictionImpulse = CalculateFrictionImpulse(impulse, tangent, impactImpulse);

			a.AddVelocity(1.f / a.GetMass() * frictionImpulse);
			b.AddVelocity(1.f / b.GetMass() * -frictionImpulse);

			// Position correction
			const float percent = 0.8f; // обычно от 20% до 80%
			const float slop = 0.01f; // обычно от 0.01 до 0.1
			glm::vec3 correction = -std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
			a.AddPosition(float(1. / a.GetMass()) * -correction);
			b.AddPosition(float(1. / b.GetMass()) * correction);


			// Раздвигаем объекты на минимальное расстояние, достаточное, чтобы объекты перестали пересекаться
			//a.AddPosition(-collisionProps.Normal * collisionProps.penetration / 2.f);
			//b.AddPosition(collisionProps.Normal * collisionProps.penetration / 2.f);
		}
	}
protected:
private:
	DrawablePrimitive::Line l;
};

#endif // !IMPULSERESPONSEEFNR_H
