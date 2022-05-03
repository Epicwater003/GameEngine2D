#ifndef IBODY2D_H
#define IBODY2D_H

#include <glm/glm.hpp>

class IBody2D
{
public:
	// TODO: ���� ������ �������� �� ������� � �������
	virtual void Move(glm::vec2 position)   = 0;		   // ����������� � ������������ � �������� �������� � �������� �����
	virtual void Rotate(float angle)        = 0;  		   // ������� ������������ ������ ���� �� �������
	virtual glm::vec2 CalculateMassCenter() = 0;           // ������ ������ ����
};


#endif // IBODY_H

