#include "Marker.h"

Marker::Marker(const Vector2& position, const Color color, Level* level)
	: DrawableActor(level, " ")
{
	this->color = color;
	this->position = position;
	priority = 100;
}

void Marker::Update(float deltaTime)
{
	if (isActive)
	{
		//Flicker();
	}
}

void Marker::Flicker()
{
	if (isVisible)
	{
		isVisible = false;
	}
	else
	{
		isVisible = true;
	}
}
