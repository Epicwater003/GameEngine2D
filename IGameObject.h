/*
* Класс GameObject отвечает за:
*	* Общий интерфейс отрисовки
*	* Общий интерфейс создания
*   * Отличительные особенности объекта такие как:
*		* Цвет
*		* Тип
*       * Модель(сетка треугольников, меш)
*/


#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


class IGameObject
{
public:
	//GameObject(); // Запрет прямого создания объекта
	
	virtual Mesh GetMesh()             = 0;
	virtual glm::mat4 GetModelMatrix() = 0;
	virtual glm::vec3 GetColor()       = 0;

	virtual void SetMesh(Mesh mesh)            = 0;
	virtual void SetModelMatrix(glm::mat4 mat) = 0;
	virtual void SetColor(glm::vec3 col)       = 0;

	
	virtual void Draw(Shader& s, Camera& c) = 0;	 // Отрисовка
	virtual void Update()     = 0;                   // Перерасчет параметров													 
    virtual Mesh CreateMesh() = 0;		             // Создает сетку	 | TODO: Надо каким-то образом стандартизировать функцию для всех типов сеток
	//virtual GameObject* Create() = 0;			     // Нужно для реализации абстрактной фабрики объектов
protected:

private:

};





#endif // !GAMEOBJECT_H
