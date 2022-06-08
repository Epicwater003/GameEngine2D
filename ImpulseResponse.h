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
	virtual glm::vec3 CalculateImpulse(glm::vec3 relativeVelocity, float inverseMass, float e = 1.f) { // ��������� ������ ��������
		glm::vec3 impulse = -(float)(e + 1.f) * relativeVelocity;
		impulse /= inverseMass;
		return impulse;
	}

	virtual glm::vec3 CalculateImpactImpulse(glm::vec3 impulse, glm::vec3 impulseDirection) { 		// ���������� ������� ��������, ������� ������������ �������
		return glm::dot(impulse, impulseDirection) * impulseDirection;
	}

	virtual glm::vec3 CalculateFrictionImpulse(glm::vec3 impulse, glm::vec3 frictionImpulseDirection, glm::vec3 impactImpulse, float staticFriction = .663f, float dynamicFriction = .336f) {

		// ���������� ������� ��������, ������� ������������� � ������
		glm::vec3 impulseComponentT = glm::dot(impulse, frictionImpulseDirection) * frictionImpulseDirection;

		// ������ �� ������ �����-������ ����������� ������� ������
		if (glm::length(impulseComponentT) < glm::length(impactImpulse) * staticFriction) {
			return impulseComponentT * (float)staticFriction;
		}
		return impulseComponentT * (float)dynamicFriction;
	}

private:
};


class ImpulseResponseNFNR : // NO FRICTION NO ROTATION
	public IResponseEngine,
	public ImpulseResponse
{
public:
	void SolveCollision(IBody& a, IBody& b, CollisionDetails& collisionProps) {
		// ���������� ������� ������������ ������������
		const float percent = 0.4f; // ������ �� 20% �� 80%
		const float slop = 0.05f; // ������ �� 0.01 �� 0.1
		glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
		a.AddPosition(float(1. / a.GetMass()) * -correction);
		b.AddPosition(float(1. / b.GetMass()) * correction);

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
		a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
		b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);


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


			// ���������� ������� ������������ ������������
			const float percent = 0.4f; // ������ �� 20% �� 80%
			const float slop = 0.05f; // ������ �� 0.01 �� 0.1
			glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
			a.AddPosition(float(1. / a.GetMass()) * -correction);
			b.AddPosition(float(1. / b.GetMass()) * correction);


			// ��������� ������������� ������ �������� � ������������� ������ �������
			glm::vec3 Vr = a.GetVelocity() - b.GetVelocity();


			glm::vec3 impulseDirection = collisionProps.Normal;
			float velAlongNormal = glm::dot(Vr, impulseDirection); // �������� ����������� ������������� �������� �� ������������� ������� �������

			// ���� ������������� �������� ����������� ���������� ����� ���������, �� �� ��������� ���� ��� ������������
			if (velAlongNormal < 0) {
				return;
			}

			// �������� ������� ����������� ���������
			float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;
			float massA = a.GetMass();
			float massB = b.GetMass();

			float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass());

			// ��������� ������ �������� ��� ������� ���� (��� ������� ����� -impulse)
			glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);

			// ���������� ������� ��������, ������� ������������ �������
			glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);

			// ������������ ������� ( impulse = mass * velocity)
			a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
			b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);

			// ��������� ������
			// ��������� ������ ����������� � ���������� �������� ��������������� �������
			glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection;
			tangent = glm::vec3(impulseDirection.y, -impulseDirection.x, 0.f);
			// ���� ����� ����� ������� = 0, �� ���� ������� ������ � ������������ ��������, �.�. ������ ���
			if (!glm::length(tangent)) {
				return;
			}

			glm::vec3 frictionImpulse = CalculateFrictionImpulse(impulse, tangent, impactImpulse);

			a.AddVelocity(1.f / (float)a.GetMass() * frictionImpulse);
			b.AddVelocity(1.f / (float)b.GetMass() * -frictionImpulse);

			
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
			
			
			assert(collisionProps.CollidePointsA.size() == collisionProps.CollidePointsB.size());
			for (int i = 0; i < collisionProps.CollidePointsA.size(); i++) {

				// ��������� ������������� ����� ������������
				glm::vec3 contactPointA = collisionProps.CollidePointsA[i] - a.GetPosition();
				glm::vec3 contactPointB = collisionProps.CollidePointsB[i] - b.GetPosition();

				// ������������ ������ ����
				glm::vec3 rA = contactPointA -a.GetMassCenter();
				glm::vec3 rB = contactPointB -b.GetMassCenter();

				// ��������� ������������� ������ �������� � ������������� ������ �������
				glm::vec3 Vr = -b.GetVelocity() + a.GetVelocity() - Cross(b.GetAngularVelocity(), rB) + Cross(a.GetAngularVelocity(), rA);

				glm::vec3 impulseDirection = collisionProps.Normal;

				float velAlongNormal = glm::dot(Vr, impulseDirection); // �������� ����������� ������������� �������� �� ������������� ������� �������

				// ���� ������������� �������� ����������� ���������� ����� ���������, �� �� ��������� ���� ��� ������������
				if (!(velAlongNormal < 0)) {

					// �������� ������� ����������� ���������
					float e = (a.GetRestitution() + b.GetRestitution()) / 2.f;
					
					float massA = a.GetMass();
					float massB = b.GetMass();


					float handleA = Cross(rA, impulseDirection);
					float handleB = Cross(rB, impulseDirection);
					float inverseMass = (1.f / a.GetMass() + 1.f / b.GetMass()) + handleA * handleA * (1.f / a.GetMomentOfInertia()) + handleB * handleB * (1.f / b.GetMomentOfInertia());
					//float inverseMomentOfInertia =  handleA * handleA * (1.f / a.GetMomentOfInertia()) + handleB * handleB * (1.f / b.GetMomentOfInertia());

					// ��������� ������ �������� ��� ������� ���� (��� ������� ����� -impulse)
					glm::vec3 impulse = CalculateImpulse(Vr, inverseMass, e);
					impulse /= (float)collisionProps.CollidePointsA.size();


					// ���������� ������� ��������, ������� ������������ �������
					glm::vec3 impactImpulse = CalculateImpactImpulse(impulse, impulseDirection);


					float rotationImpulseA = Cross(rA, impactImpulse);
					float rotationImpulseB = Cross(rB, -impactImpulse);

					// ������������ ������� ( impulse = mass * velocity)
					a.AddVelocity(1.f / (float)a.GetMass() * impactImpulse);
					b.AddVelocity(1.f / (float)b.GetMass() * -impactImpulse);
					a.AddAngularVelocity((1.f / a.GetMomentOfInertia()) * rotationImpulseA);
					b.AddAngularVelocity((1.f / b.GetMomentOfInertia()) * rotationImpulseB);

					// ��������� ������
					// ��������� ������ ����������� � ���������� �������� ��������������� �������
					
					Vr = -b.GetVelocity() + a.GetVelocity() - Cross(b.GetAngularVelocity(), rB) + Cross(a.GetAngularVelocity(), rA);

					glm::vec3 tangent = Vr - glm::dot(Vr, impulseDirection) * impulseDirection;

					// ���� ����� ����� ������� = 0, �� ���� ������� ������ � ������������ ��������, �.�. ������ ���
					if (glm::length(tangent)) {
						tangent = glm::normalize(tangent);
						float jt = -glm::dot(Vr, tangent);
						if (!(abs(jt) <= 0.001)) {

							float staticFriction =  sqrtf(a.GetStaticFriction() * b.GetStaticFriction());
							float dynamicFriction =  sqrtf(a.GetDynamicFriction() * b.GetStaticFriction());
							float j = impactImpulse.length();
							glm::vec3 tangentImpulse;
							if (abs(jt) < j * staticFriction)
								tangentImpulse = tangent * (float)jt;
							else
								tangentImpulse = tangent * (float)-j * (float)dynamicFriction;

							glm::vec3 frictionImpulse = tangentImpulse;

							frictionImpulse /= (float)collisionProps.CollidePointsA.size();


							rotationImpulseA = Cross(rA, frictionImpulse);
							rotationImpulseB = Cross(rB, -frictionImpulse);

							a.AddVelocity(1.f / (float)a.GetMass() * frictionImpulse);
							b.AddVelocity(1.f / (float)b.GetMass() * -frictionImpulse);
							a.AddAngularVelocity((1.f / a.GetMomentOfInertia()) * rotationImpulseA);
							b.AddAngularVelocity((1.f / b.GetMomentOfInertia()) * rotationImpulseB);


						}
					}
				}
				// ���������� ������� ������������ ������������
				const float percent = 0.2f; // ������ �� 20% �� 80%
				const float slop = 0.05f; // ������ �� 0.01 �� 0.1
				glm::vec3 correction = std::max(collisionProps.penetration - slop, 0.f) / (float(1. / a.GetMass()) + float(1. / b.GetMass())) * percent * collisionProps.Normal;
				a.AddPosition(float(1. / a.GetMass()) * -correction);
				b.AddPosition(float(1. / b.GetMass()) * correction);

			}

		}
	}
protected:
private:
	DrawablePrimitive::Line l;
	DrawablePrimitive::Point p;
};

#endif // !IMPULSERESPONSEEFNR_H