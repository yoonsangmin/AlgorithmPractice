#include "PreCompiledHeader.h"
#include "Actor.h"

#include "Core.h"
#include "Level/Level.h"

Actor::Actor(class Level* level, int priority)
	: isActive(true), isExpired(false), level(level), priority(priority)
{
}

Actor::~Actor()
{
}

void Actor::Update(float delatTime)
{
}

void Actor::Draw()
{
}

bool Actor::SetPosition(const Vector2& newPosition)
{
    if (isCollisionEnabled && level->CanWalk(newPosition) || !isCollisionEnabled)
    {
        position = newPosition;
        return true;
    }

    return false;
}

inline Vector2 Actor::Position() const
{
	return position;
}
