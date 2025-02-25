#include "PreCompiledHeader.h"
#include "Level.h"

Level::Level()
{
}

Level::~Level()
{
}

void Level::ProcessAddedAndDestroyedActor()
{
	std::multiset<std::shared_ptr<Actor>, ActorComparator> newActors;

	// 액터 순회 후 삭제 요청된 액터를 처리.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		if (!actor->isExpired)
		{
			newActors.emplace(actor);
		}
	}

	// 추가 요청된 액터 처리.
	if (addRequestedActors.size() > 0)
	{
		for (std::shared_ptr<Actor>& newActor : addRequestedActors)
		{
			newActors.emplace(newActor);
		}
		
		addRequestedActors.clear();
	}

	actors.clear();
	actors.swap(newActors);
}

void Level::Update(float deltaTime)
{
	// 레벨에 포함된 액터를 순회하면서 Update 함수 호출.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		// 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		actor->Update(deltaTime);
	}
}

void Level::Draw()
{
	// 레벨에 포함된 액터를 순회하면서 Draw 함수 호출.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		// 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
		if (!actor->isActive || actor->isExpired || !actor->isVisible)
		{
			continue;
		}

		actor->Draw();
	}
}