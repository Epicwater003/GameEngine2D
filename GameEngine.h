#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>

class ICollisionEngine;
class IResponseEngine;
class IShape;

class GameEngine {
public:
	ICollisionEngine* collisionEngine;
	IResponseEngine*  responseEngine;

	std::vector<IShape *> gameObjects;

	IShape* AddObject(IShape& obj);
	IShape* AddObject(IShape& obj, glm::vec2 position);
	void doStep(float deltaTime) {

		/*for body in gameObjects{
			bodies_that_can_collide.add() = collisionEngine.isCanCollide(IGameObject a, IGameObject b);
		}
		for body in bodies_that_can_collade{
			collisions.add() = collisionEngine.checkColision(IGameObject a, IGameObject b);
		};
		for collision in collisions{
			responseEngine.doResponse(collision colision);
		}
		for gameobject in gameObjects{
			gameObjects.draw();
		}*/

	};
	void Render();
	void Clear();

};



#endif // !GAMEENGINE_H

