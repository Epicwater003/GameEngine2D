/*
* ����� INoPhysical �������� ��:
*	����������� ������� ��� ����� ������
*	�������� �� ������������(���� �� �������� ���-�� ����� ������ ICanCollide)
*
* 
*/


#ifndef NOPHYSICAL_H
#define NOPHYSICAL_H

#include "IBaseInterface.h"
#include <glm/glm.hpp>

class INoPhysical:
	public IBaseInterface
{
public:
	virtual void Move(glm::vec2 position);				 // ����������� � ��������� �����
	virtual void Move(glm::vec2 direction, float scalar);// ����������� �� ��������� ������� �� �������� ����������
	virtual void Rotation(float angle);					 // ��������� ���� �������
	virtual void Rotate(float angle);                    // ������� ������������ ������ ���� �� �������� ����
	
protected:
	glm::vec2 position = { 0., 0. };                     // �������
	float angle = 0.;	                                 // ���� �������� ����
private:
	
};

#endif // !NOPHYSICAL_H

