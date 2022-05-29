#ifndef IMPULSERESPONSE_H
#define IMPULSERESPONSE_H

#include "IResponseEngine.h"
#include "IBody.h"
#include "IShape.h"

class ImpulseResponse
{
public:
protected:

	virtual glm::vec3 CalculateImpulse(glm::vec3 relativeVelocity, float inverseMass, float e =1.f) { // ��������� ������ ��������
		glm::vec3 impulse = -(e + 1.f) * relativeVelocity;
		impulse /= inverseMass;
		return impulse;
	}

	virtual glm::vec3 CalculateImpactImpulse(glm::vec3 impulse, glm::vec3 impulseDirection) { 		// ���������� ������� ��������, ������� ������������ �������
		return glm::dot(impulse, impulseDirection) * impulseDirection;
	}

	virtual glm::vec3 CalculateFrictionImpulse(glm::vec3 impulse, glm::vec3 frictionImpulseDirection, glm::vec3 impactImpulse, float staticFriction = .663f, float dynamicFriction = .336f) {
		frictionImpulseDirection = glm::normalize(frictionImpulseDirection);

		// ���������� ������� ��������, ������� ������������� � ������
		glm::vec3 impulseComponentT = glm::dot(impulse, frictionImpulseDirection) * frictionImpulseDirection;

		// ������ �� ������ �����-������ ����������� ������� ������
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

		// ��������� ������������� ������ �������� � ������������� ������ �������
		glm::vec3 Vr = a.GetVelocity() - b.GetVelocity();
		glm::vec3 Cr = b.GetPosition() - a.GetPosition();

		glm::vec3 impulseDirection = collisionProps.Normal;
		float velAlongNormal = glm::dot(Vr, impulseDirection); // �������� ����������� ������������� �������� �� ������������� ������� �������

		// ���� ������������� �������� ����������� ���������� ����� ���������, �� �� ��������� ���� ��� ������������
		if (velAlongNormal < 0) {
			return;
		}

		// �������� ������� ����������� ���������
		float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;
		float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass());
		
		// ��������� ������ �������� ��� ������� ���� (��� ������� ����� -impulse)
		glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);

		// ���������� ������� ��������, ������� ������������ �������
		glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

		// ������������ ������� ( impulse = mass * velocity)
		a.AddVelocity(1.f / a.GetMass() * impactImpulse);
		b.AddVelocity(1.f / b.GetMass() * -impactImpulse);

		// ���������� ������� �� ����������� ����������, �����������, ����� ������� ��������� ������������
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
			// ��������� ������������� ������ �������� � ������������� ������ �������
			glm::vec3 Vr = a.GetVelocity() - b.GetVelocity();
			glm::vec3 Cr = b.GetPosition() - a.GetPosition();

			glm::vec3 impulseDirection = collisionProps.Normal;
			float velAlongNormal = glm::dot(Vr, impulseDirection); // �������� ����������� ������������� �������� �� ������������� ������� �������

			// ���� ������������� �������� ����������� ���������� ����� ���������, �� �� ��������� ���� ��� ������������
			if (velAlongNormal < 0) {
				return;
			}

			// �������� ������� ����������� ���������
			float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;
			float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass());

			// ��������� ������ �������� ��� ������� ���� (��� ������� ����� -impulse)
			glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);

			// ���������� ������� ��������, ������� ������������ �������
			glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

			// ������������ ������� ( impulse = mass * velocity)
			a.AddVelocity(1.f / a.GetMass() * impactImpulse);
			b.AddVelocity(1.f / b.GetMass() * -impactImpulse);
		
			// ��������� ������
			// ��������� ������ ����������� � ���������� �������� ��������������� �������
			glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection; 
			tangent = glm::vec3(impulseDirection.y, -impulseDirection.x, 0.f);
			// ���� ����� ����� ������� = 0, �� ���� ������� ������ � ������������ ��������, �.�. ������ ���
			if (!glm::length(tangent)) {
				return;
			}

			glm::vec3 frictionImpulse = CalculateFrictionImpulse(impulse, tangent, impactImpulse);

			a.AddVelocity(1.f / a.GetMass() * frictionImpulse);
			b.AddVelocity(1.f / b.GetMass() * -frictionImpulse);

			// Position correction
			const float percent = 0.8f; // ������ �� 20% �� 80%
			const float slop = 0.01f; // ������ �� 0.01 �� 0.1
			glm::vec3 correction = -std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
			a.AddPosition(float(1. / a.GetMass()) * -correction);
			b.AddPosition(float(1. / b.GetMass()) * correction);


			// ���������� ������� �� ����������� ����������, �����������, ����� ������� ��������� ������������
			//a.AddPosition(-collisionProps.Normal * collisionProps.penetration / 2.f);
			//b.AddPosition(collisionProps.Normal * collisionProps.penetration / 2.f);
		}
	}
protected:
private:
	DrawablePrimitive::Line l;
};

#endif // !IMPULSERESPONSEEFNR_H
