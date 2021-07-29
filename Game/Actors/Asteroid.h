#pragma once

#include "Base/Actor.h"

class Asteroid : public nc::Actor
{
public:
	Asteroid(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape } {}

	void Update(float dt) override;

	void OnCollision(Actor* actor) override;

private:
	float speed = 100;
	nc::Vector2 velocity;
};