/*
* Класс GameObject отвечает за:
*	* Общий интерфейс отрисовки
*	* Общий интерфейс создания
*   * Отличительные особенности объекта такие как:
*		* Цвет
*		* Тип
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
	GameObject(); // Запрет прямого создания объекта

	glm::mat4 model = glm::mat4(1);	         // Матрица модели
	glm::vec3 color = { 0.423,0.411,0.376 }; // Цвет
	
	
	virtual GameObject* Create() = 0;					 // Нужно для реализации абстрактной фабрики объектов
	virtual void Draw(Shader &s, Camera &c) = 0;		 // Отрисовка
	
	virtual void Update();                               // Пересчет параметров
	//virtual Mesh CreateMesh() = 0;		             // Создает сетку	 | TODO: Надо какимто образом стандартизировать функцию для всех типов сеток
	
protected:

private:
	
};





#endif // !GAMEOBJECT_H
