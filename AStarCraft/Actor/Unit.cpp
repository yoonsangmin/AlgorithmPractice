#include "Unit.h"
#include "Engine/Engine.h"
#include "Marker.h"
#include "Level/AStarLevel.h"

Unit::Unit(const Vector2& position, Level* level, float moveSpeed, float MarkerSpeed, float markerInterval, int maxMarker)
	: DrawableActor(level, "e", false, true)
{
	color = Color::Green;
	this->position = position;

	moveSpeedPerSecond = moveSpeed;
	moveTimer.SetTime(1.0f / moveSpeedPerSecond);

	this->maxMarker = maxMarker;

	markerMoveTimer.SetTime(1.0f / MarkerSpeed);
	markerIntervalTimer.SetTime(markerInterval);

	for (int ix = 0; ix < maxMarker; ++ix)
	{
		pathMarkers.emplace_back(Engine::Get().AddActor(new Marker(position, Color::BackgroundWhite, level)));
		pathMarkers.back().lock()->SetActive(false);
	}

	markerIterator = path.end();
}

void Unit::Update(float deltaTime)
{
	moveTimer.Update(deltaTime);
	MoveToNextPath();

	if (path.size() != 0)
	{
		DrawPathMarker(deltaTime);
	}
	else
	{
		for (int ix = 0; ix < maxMarker; ++ix)
		{
			pathMarkers[ix].lock()->SetActive(false);
		}
	}
}

void Unit::RequestPath(Vector2 newPosition)
{
	level->As<AStarLevel>()->FindPath(position, newPosition, &path);
	
	if (path.size() == 0)
	{
		return;
	}

	markerMoveTimer.Reset();
	pathMarkers[0].lock()->SetActive(true);
	markerIterator = path.begin() + 1;

	markerIntervalTimer.SetTimeOut(true);
}

void Unit::MoveToNextPath()
{
	if (!path.empty())
	{
		if (moveTimer.IsTimeOut())
		{
			SetPosition(path.front());
			path.pop_front();
			moveTimer.Reset();
		}
	}
}

void Unit::DrawPathMarker(float deltaTime)
{
	if (!markerIntervalTimer.IsTimeOut())
	{
		return;
		//markerIntervalTimer.Update(deltaTime);
		//if (markerIntervalTimer.IsTimeOut())
		//{
		//	markerIterator = path.begin() + 1;
		//	pathMarkers[0].lock()->SetActive(true);
		//}
	}

	if (markerIntervalTimer.IsTimeOut())
	{
		if (pathMarkers[0].lock()->IsAcive())
		{
			markerMoveTimer.Update(deltaTime);
			if (markerMoveTimer.IsTimeOut())
			{
				markerMoveTimer.Reset();
				if (markerIterator != path.end())
				{
					pathMarkers[0].lock()->SetPosition(*markerIterator);
					++markerIterator;
				}
				
				for (int ix = maxMarker - 1; ix > 0; --ix)
				{
					if (pathMarkers[ix - 1].lock()->IsAcive())
					{
						pathMarkers[ix].lock()->SetPosition(pathMarkers[ix - 1].lock()->Position());
						pathMarkers[ix].lock()->SetActive(true);
					}
				}

				if (markerIterator == path.end() && pathMarkers.front().lock()->Position() == pathMarkers.back().lock()->Position())
				{
					markerIntervalTimer.Reset();
					for (int ix = 0; ix < maxMarker; ++ix)
					{
						pathMarkers[ix].lock()->SetActive(false);
					}
				}
			}
		}
	}
}
