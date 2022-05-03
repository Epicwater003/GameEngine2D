#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody2D.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody2D 
{
	glm::vec2 position     = { 0., 0. }; // �������
	glm::vec2 direction    = { 0., 1. }; // ������ �����������(���� �������)
	glm::vec2 velocity     = { 0., 0. }; // ��������
	glm::vec2 acceleration = { 0., 0. }; // ���������
	glm::vec2 massCenter   = { 0., 0. }; // ������� ������ ���� https://habr.com/ru/post/579364/
	float density = 1.;                  // ���������
	float mass    = 1.;	                 // �����
	float angle   = 0.;	                 // ���� �������� ����
	float angularVelocity     = 0.;	     // ������� ��������
	float angularAcceleration = 0.;	     // ������� ���������
protected:
	void Move(glm::vec2 position)          ;         // ����������� � ������������ � �������� �������� � �������� �����
	void Rotate(float angle)               ;         // ������� ������������ ������ ���� �� �������
	virtual glm::vec2 CalculateMassCenter();         // ������ ������ ����
    glm::vec2 CalculateMassCenter(Mesh m);           // ������������� ������ ������ ���� � ����������� �� ����
};


#endif // !IRIGIDBODY_H

