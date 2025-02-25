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

	// ���� ��ȸ �� ���� ��û�� ���͸� ó��.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		if (!actor->isExpired)
		{
			newActors.emplace(actor);
		}
	}

	// �߰� ��û�� ���� ó��.
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
	// ������ ���Ե� ���͸� ��ȸ�ϸ鼭 Update �Լ� ȣ��.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		// ���Ͱ� ��Ȱ��ȭ �����̰ų�, ���� ��û�� ��� �ǳʶٱ�.
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		actor->Update(deltaTime);
	}
}

void Level::Draw()
{
	// ������ ���Ե� ���͸� ��ȸ�ϸ鼭 Draw �Լ� ȣ��.
	for (const std::shared_ptr<Actor>& actor : actors)
	{
		// ���Ͱ� ��Ȱ��ȭ �����̰ų�, ���� ��û�� ��� �ǳʶٱ�.
		if (!actor->isActive || actor->isExpired || !actor->isVisible)
		{
			continue;
		}

		actor->Draw();
	}
}