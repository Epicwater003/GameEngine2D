#ifndef IRIGIDBODY_H
#define IRIGIDBODY_H

#include "IBaseInterface.h"
#include <glm/glm.hpp>

class IRigidBody: 
	public IBaseInterface 
{
	glm::vec2 position = { 0., 0. };     // �������
	glm::vec2 direction = { 0., 1. };    // ������ �����������(���� �������)
	glm::vec2 velocity = { 0., 0. };     // ��������
	glm::vec2 acceleration = { 0., 0. }; // ���������
	glm::vec2 massCenter = { 0., 0. };   // ������� ������ ���� https://habr.com/ru/post/579364/
	float density = 1.;                  // ���������
	float mass = 1.;	                 // �����
	float angle = 0.;	                 // ���� �������� ����
	float angularVelocity = 0.;	         // ������� ��������
	float angularAcceleration = 0.;	     // ������� ���������
protected:
	virtual void Move(float deltaTime);					 // ����������� � ������������ � �������� �������� � �������� �����
	virtual void Rotate(double deltaTime);  		     // ������� ������������ ������ ���� �� �������
	virtual glm::vec2 CalculateMassCenter() = 0;         // ������ ������ ����
};


#endif // !IRIGIDBODY_H

