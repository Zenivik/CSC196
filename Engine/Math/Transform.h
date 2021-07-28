#pragma once

#include "Matrix22.h"
#include "Matrix33.h"

namespace nc
{
	struct Transform
	{
		Vector2 position;
		float rotation;
		Vector2 scale;

		Vector2 localPosition;
		float localRotation ;
		Vector2 localScale ;

		Matrix33 matrix;

		Transform() {}
		Transform(const Vector2& position, float rotation, float scale) : position{ position }, rotation{ rotation }, scale{ scale } {}

		void Update();
		void Update(const Matrix33& mx);
	};
}