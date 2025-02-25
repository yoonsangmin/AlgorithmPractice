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
		SpawnUnit(Engine::Get().MousePosition());
	}

	if (Engine::Get().GetKeyDown(VK_RBUTTON))
	{
		Vector2 targetPosition = Engine::Get().MousePosition();
		
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

	Engine::Get().Draw(Vector2(0, Engine::Get().ScreenSize().y - 1), "Left Click: Spawn Unit, Right Click: Move");
}

void Player::SpawnUnit(const Vector2& position)
{
	std::weak_ptr<Unit> unit = std::static_pointer_cast<Unit>(level->AddActor(new Unit(position, level)));

	units.emplace_back(unit);
	selectedUnits.emplace_back(unit);
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
