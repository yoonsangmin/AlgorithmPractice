#include "Player.h"

#include "Engine/Engine.h"
#include "Level/AStarLevel.h"
#include "Actor/Marker.h"

Player::Player(Level* level, float markerDuration, float markerFlickerTime)
	: Actor(level), markerTimer(markerDuration), markerFlickerTimer(markerFlickerTime)
{
	destinationMarker = Engine::Get().AddActor(new Marker(Vector2(), Color::BackgroundBlue, level));
	destinationMarker.lock()->SetActive(false);
	markerTimer.SetTimeOut(true);
}

void Player::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}

    if (Engine::Get().GetKeyDown(VK_LBUTTON))
    {
        lButtonDownMousePosition = Engine::Get().MousePosition();
    }

    if (Engine::Get().GetKeyUp(VK_LBUTTON))
    {
        for (const std::weak_ptr<Unit> unit : selectedUnits)
        {
            std::shared_ptr<Unit> lockedUnit = unit.lock();
            if (lockedUnit)
            {
                lockedUnit->SetSelected(false);
            }
        }

        selectedUnits.clear();
        // 유닛 선택.
        level->FindActors(lButtonDownMousePosition, Engine::Get().MousePosition(), selectedUnits);

        for (const std::weak_ptr<Unit> unit : selectedUnits)
        {
            std::shared_ptr<Unit> lockedUnit = unit.lock();
            if (lockedUnit)
            {
                lockedUnit->SetSelected(true);
            }
        }
    }

	if (Engine::Get().GetKeyDown(VK_MBUTTON))
	{
        if (level->CanWalk(Engine::Get().MousePosition()))
        {
            SpawnUnit(Engine::Get().MousePosition());
        }
	}

	if (Engine::Get().GetKeyDown(VK_RBUTTON))
	{
		Vector2 targetPosition = Engine::Get().MousePosition();

        if (targetPosition.x >= 0 && targetPosition.x < Engine::Get().ScreenSize().x &&
            targetPosition.y >= 0 && targetPosition.y < Engine::Get().ScreenSize().y)
        {
            RemoveExpiredUnits();
            for (std::weak_ptr<Unit> unit : selectedUnits)
            {
                auto locked = unit.lock();
                if (locked != nullptr && locked->IsAcive())
                {
                    locked->RequestPath(targetPosition);
                }
            }

            destinationMarker.lock()->SetPosition(targetPosition);
            destinationMarker.lock()->SetActive(true);
            markerTimer.Reset();
        }
	}

	if (!markerTimer.IsTimeOut())
	{
		markerTimer.Update(deltaTime);
		markerFlickerTimer.Update(deltaTime);

		if (markerFlickerTimer.IsTimeOut())
		{
			markerFlickerTimer.Reset();
			if (destinationMarker.lock()->IsAcive())
			{
				destinationMarker.lock()->SetActive(false);
			}
			else
			{
				destinationMarker.lock()->SetActive(true);
			}
		}
	}
	else
	{
		destinationMarker.lock()->SetActive(false);
	}
}

void Player::Draw()
{
	Super::Draw();

	Engine::Get().Draw(Vector2(0, Engine::Get().ScreenSize().y - 1), "LBtn Drag: Select Unit, MBtn: Spawn Unit, RBtn: Move");

    // 드래그 범위 그리기.
    if (Engine::Get().GetKey(VK_LBUTTON))
    {
        Vector2 currentMousePosition = Engine::Get().MousePosition();
        int startX = lButtonDownMousePosition.x < currentMousePosition.x ? lButtonDownMousePosition.x : currentMousePosition.x;
        int endX = lButtonDownMousePosition.x >= currentMousePosition.x ? lButtonDownMousePosition.x : currentMousePosition.x;
        int startY = lButtonDownMousePosition.y < currentMousePosition.y ? lButtonDownMousePosition.y : currentMousePosition.y;
        int endY = lButtonDownMousePosition.y >= currentMousePosition.y ? lButtonDownMousePosition.y : currentMousePosition.y;

        startX = max(startX, 0);
        startY = max(startY, 0);

        endX = min(endX, Engine::Get().ScreenSize().x - 1);
        endY = min(endY, Engine::Get().ScreenSize().y - 1);

        for (int x = startX; x <= endX; ++x)
        {
            if (x == startX || x == endX)
            {
                for (int y = startY; y <= endY; ++y)
                {
                    Engine::Get().Draw(Vector2(x, y), "#", Color::BrightGreen);
                }
            }
            else
            {
                Engine::Get().Draw(Vector2(x, startY), "#", Color::BrightGreen);
                Engine::Get().Draw(Vector2(x, endY), "#", Color::BrightGreen);
            }
        }
    }
}

void Player::SpawnUnit(const Vector2& position)
{
	std::weak_ptr<Unit> unit = std::static_pointer_cast<Unit>(level->AddActor(new Unit(position, level)));

    level->UpdateMap(unit.lock()->Position(), false);

	units.emplace_back(unit);
}

void Player::RemoveExpiredUnits()
{
	units.erase(std::remove_if(units.begin(), units.end(),
		[](const std::weak_ptr<Unit>& unit) {
			return unit.expired();
		}),
		units.end());

	selectedUnits.erase(std::remove_if(selectedUnits.begin(), selectedUnits.end(),
		[](const std::weak_ptr<Unit>& unit) {
			return unit.expired();
		}),
		selectedUnits.end());
}
