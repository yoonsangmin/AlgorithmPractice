#include "Wall.h"
#include "Engine/Engine.h"

Wall::Wall(const Vector2& position, Level* level)
	: DrawableActor(level, "@", true, true)
{
	color = Color::Magenta;
	this->position = position;	
	priority = 100;
}