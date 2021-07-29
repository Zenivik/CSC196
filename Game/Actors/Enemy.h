#pragma once
#include "Base/Actor.h"

class Enemy : public nc::Actor
{
public:
	Enemy(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape } {}

	void Update(float dt) override;

	void OnCollision(Actor* actor) override;

private:
	float speed = 100;
	float fireTimer = 0;
	float fireRate = 2;
};