#pragma once

#include <deque>
#include <vector>
#include "Actor/DrawableActor.h"
#include "Engine/Timer.h"

class Unit : public DrawableActor
{
	RTTI_DECLARATIONS(Unit, DrawableActor)
	
public:
	Unit(const Vector2& position, class Level* level, float moveSpeed = 10.0f, float markerSpeed = 50.0f, float markerInterval = 1.0f, int maxMarker = 5);

	virtual void Update(float deltaTime) override;

	void RequestPath(Vector2 newPosition);

private:
	void MoveToNextPath();
	void DrawPathMarker(float deltaTime);

private:
	std::deque<Vector2> path;
	float moveSpeedPerSecond;
	Timer moveTimer;

	Timer markerMoveTimer;
	Timer markerIntervalTimer;

	//std::weak_ptr<class Marker> pathMarker;
	int maxMarker = 0;
	std::vector<std::weak_ptr<class Marker>> pathMarkers;
	std::deque<Vector2>::iterator markerIterator;
};
