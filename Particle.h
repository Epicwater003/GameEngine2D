#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameObject.h"
#include "RigidBody2D.h"
#include "DefaultShape.h"

class Particle:
	public GameObject
{
	Particle():
		GameObject()
	{

	}
	GameObject* p = new Particle();
};

#endif // !PARTICLE_H
