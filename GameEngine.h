#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>

#include "IBody.h"
#include "IShape.h"
#include "GameObject.h"
#include "ICollisionEngine.h"
#include "IResponseEngine.h"
#include "SAT2D.h"
#include "ImpulseResponse.h"



class GameEngine {
public:
	std::unique_ptr<IBroadPhase> broadPhase;
	std::unique_ptr<INarrowPhase> narrowPhase = std::make_unique<SAT2D>();
	std::unique_ptr<IResponseEngine> responsePhase = std::make_unique<ImpulseResponseWFWR>();

	
	std::vector<GameObject> gameObjects;
	std::vector<bool> staticObjects;
	std::vector<glm::vec3> forces = {glm::vec3(0,-0.3,0)};

	void CreateCircleObject(float radius, unsigned int resolution, std::vector<Texture>& tex, glm::vec3 pos, glm::vec3 vel, glm::vec3 color = glm::vec3(1., 0.54, 0.41), float angle = 3.14f / 4.f, float mass = 10., bool isStatic = false) {
		if (radius <= 0) {
			radius = 10.f;
		}
		if (resolution <= 1) {
			resolution = 2;
		}

		Circle c(radius, resolution);
		c.CalculateMassCenter();
		c.SetMass(mass);
		c.SetMomentOfInertia((1. / 12.) * (c.radius * c.radius * c.GetMass()));
		c.SetRestitution(0.89);
		c.SetPosition(pos);
		c.SetVelocity(vel);
		c.SetAngle(angle);
		c.SetColor(color);
		c.SetTextures(tex);
		c.Reshape();
		c.Update(0);
		staticObjects.push_back(isStatic);
		gameObjects.push_back(std::move(c));
		
	};
	void CreateSquareObject(float a, float b, glm::vec3 pos, glm::vec3 vel, glm::vec3 color = glm::vec3(1., 0.54, 0.41), float angle = 3.14f / 4.f, float mass = 10., bool isStatic = false) {
		if (a < 0) {
			a = -a;
		}
		if (a == 0) {
			a = 0.5;
		}
		if (b < 0) {
			b = -b;
		}
		if (b == 0) {
			b = 0.5;
		}
		Square c(a, b);
		c.CalculateMassCenter();
		c.SetMass(mass);
		c.SetMomentOfInertia((1. / 3.) * (a*a + b*b) * (c.GetMass()));
		c.SetRestitution(0.89);
		c.SetPosition(pos);
		c.SetVelocity(vel);
		c.SetAngle(angle);
		c.SetColor(color);
		c.SetRestitution(0.6);
		c.Reshape();
		c.Update(0);
		staticObjects.push_back(isStatic);
		gameObjects.push_back(std::move(c));
		
	}


	void doStep(float deltaTime) {

		for (int i = 0, s = gameObjects.size(); i < s; i++) {
			for (int j = i; j < s; j++) {
				if(i!=j){
					CollisionDetails cd = narrowPhase->GetCollisionProperties(gameObjects[i], gameObjects[j]);
					if (cd.isCollide) {
						responsePhase->SolveCollision(gameObjects[i], gameObjects[j], cd);
					}
				}
			}
		}
		for (int i = 0, s = gameObjects.size(); i < s; i++) {
			
			gameObjects[i].Update(deltaTime);
			if (!staticObjects[i]) {
				for (int j = 0, z = forces.size(); j < z; j++) {
					gameObjects[i].AddVelocity(forces[j]);
				}
			}
		}

		
		

	};
	void Render(Shader& sh, Camera& cm) {
		for (int i = 0, s = gameObjects.size(); i < s; i++) {
			gameObjects[i].Draw(sh, cm);
		}
	};
	void Clear() {
		assert(staticObjects.size() == gameObjects.size());
		std::vector<GameObject> gameObjectsT;
		std::vector<bool> staticObjectsT;
		for (int i = 0, s = staticObjects.size(); i < s; i++) {
			if (staticObjects[i]) {
				staticObjectsT.push_back(true);
				gameObjectsT.push_back(std::move(gameObjects[i]));
			}
		}
		gameObjects.clear();
		staticObjects.clear();
		staticObjects = staticObjectsT;
		for (int i = 0, s = staticObjectsT.size(); i < s;i++) {
			gameObjects.push_back( std::move(gameObjectsT[i]));
		}
		
	};

};



#endif // !GAMEENGINE_H

