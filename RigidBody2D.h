#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "IBody2D.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class RigidBody2D: 
	public IBody2D 
{
	glm::vec2 GetPosition()        { return position           ; }
	glm::vec2 GetDirection()       { return direction          ; }
	glm::vec2 GetVelocity()        { return velocity           ; }
	glm::vec2 GetAcceleration()    { return acceleration       ; }
	glm::vec2 GetMassCenter()      { return massCenter         ; }
	float GetDensity()             { return density            ; }
	float GetMass()                { return mass               ; }
	float GetAngle()               { return angle              ; }
	float GetAngularVelocity()     { return angularVelocity    ; }
	float GetAngularAcceleration() { return angularAcceleration; }

	void SetPosition(glm::vec2 p)        { position = p           ; }
	void SetDirection(glm::vec2 d)       { direction = d          ; }
	void SetVelocity(glm::vec2 v)        { velocity = v           ; }
	void SetAcceleration(glm::vec2 a)    { acceleration = a       ; }
	void SetMassCenter(glm::vec2 mc)     { massCenter = mc        ; }
	void SetDensity(float d)             { density = d            ; }
	void SetMass(float m)                { mass = m               ; }
	void SetAngle(float a)               { angle = a              ; }
	void SetAngularVelocity(float w)     { angularVelocity = w    ; }
	void SetAngularAcceleration(float e) { angularAcceleration = e; }

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
	glm::vec2 CalculateMassCenter();         // ������ ������ ����
    glm::vec2 CalculateMassCenter(Mesh m);           // ������������� ������ ������ ���� � ����������� �� ����
};


#endif // !IRIGIDBODY_H

