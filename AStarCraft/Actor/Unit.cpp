#include "Unit.h"
#include "Engine/Engine.h"
#include "Marker.h"
#include "Level/AStarLevel.h"

Unit::Unit(const Vector2& position, Level* level, float moveSpeed, float MarkerSpeed, float markerInterval, int maxMarker)
	: DrawableActor(level, "M", false, true)
{
	color = Color::Yellow;
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

	markerIndex = 0;
    SetSelected(false);

    targetPosition = position;

    attempt = 0;
    maxAttempt = 5;
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

void Unit::RequestPath(Vector2 newPosition, bool shouldKeepTargetPosition)
{
    level->UpdateMap(position, true);
	level->As<AStarLevel>()->FindPath(position, newPosition, &path);
    level->UpdateMap(position, false);

    if (!shouldKeepTargetPosition)
    {
        targetPosition = newPosition;
        attempt = 0;
    }

	if (path.size() == 0)
	{
		return;
	}

	markerMoveTimer.Reset();
	pathMarkers[0].lock()->SetActive(true);
	markerIndex = 1;
    markerPositions.assign(path.begin(), path.end());

	markerIntervalTimer.SetTimeOut(true);
}

bool Unit::SetPosition(const Vector2& newPosition)
{
    level->UpdateMap(position, true);
    bool result = Super::SetPosition(newPosition);
    level->UpdateMap(position, false);
    
    return result;
}

void Unit::MoveToNextPath()
{
	if (!path.empty())
	{
		if (moveTimer.IsTimeOut())
		{
            if (SetPosition(path.front()))
            {
                path.pop_front();
                moveTimer.Reset();
            }
            else if (attempt < maxAttempt)
            {
                ++attempt;
                RequestPath(targetPosition, true);
                moveTimer.Reset();
            }
		}
	}

    if (moveTimer.IsTimeOut())
    {
        if (path.size() == 0 && position != targetPosition && attempt < maxAttempt)
        {
            ++attempt;
            RequestPath(targetPosition, true);
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

                for (int ix = maxMarker - 1; ix > 0; --ix)
                {
                    if (pathMarkers[ix - 1].lock()->IsAcive())
                    {
                        pathMarkers[ix].lock()->SetPosition(pathMarkers[ix - 1].lock()->Position());
                        pathMarkers[ix].lock()->SetActive(true);
                    }
                }

				if (markerIndex < markerPositions.size())
				{
					pathMarkers[0].lock()->SetPosition(markerPositions[markerIndex]);
					++markerIndex;
				}

				if (markerIndex == markerPositions.size() && pathMarkers.front().lock()->Position() == pathMarkers.back().lock()->Position())
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
