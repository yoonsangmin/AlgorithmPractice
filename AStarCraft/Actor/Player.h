#pragma once

#include <deque>
#include <vector>
#include "Actor/Actor.h"
#include "Engine/Timer.h"
#include "Unit.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(class Level* level, float markerDuration = 1.0f, float markerFlickerTime = 0.1f);

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	void SpawnUnit(const Vector2& position);

	void RemoveExpiredUnits();

private:
	std::weak_ptr<class Marker> destinationMarker;
	Timer markerFlickerTimer;
	Timer markerTimer;

	std::vector<std::weak_ptr<Unit>> units;
	std::vector<std::weak_ptr<Unit>> selectedUnits;

    Vector2 lButtonDownMousePosition;
};