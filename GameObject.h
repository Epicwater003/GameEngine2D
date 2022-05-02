/*
* ����� GameObject �������� ��:
*	* ����� ��������� ���������
*	* ����� ��������� ��������
*   * ������������� ����������� ������� ����� ���:
*		* ����
*		* ���
*/


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "INoPhysical.h"

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class GameObject:
	public INoPhysical
{
public:
	GameObject(); // ������ ������� �������� �������

	glm::mat4 model = glm::mat4(1);	         // ������� ������
	glm::vec3 color = { 0.423,0.411,0.376 }; // ����
	
	
	virtual GameObject* Create() = 0;					 // ����� ��� ���������� ����������� ������� ��������
	virtual void Draw(Shader &s, Camera &c) = 0;		 // ���������
	
	virtual void Update();                               // �������� ����������
	//virtual Mesh CreateMesh() = 0;		             // ������� �����	 | TODO: ���� ������� ������� ����������������� ������� ��� ���� ����� �����
	
protected:

private:
	
};





#endif // !GAMEOBJECT_H
