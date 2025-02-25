#pragma once

#include <vector>
#include <set>
#include "Core.h"
#include "RTTI.h"
#include "Container/List.h"
#include "Actor/Actor.h"

struct ActorComparator
{
	bool operator()(const std::shared_ptr<Actor>& lhs, const std::shared_ptr<Actor>& rhs) const
	{
		// Actor Ŭ������ �� �����ڸ� ����Ͽ� ��
		return *lhs > *rhs;
	}
};

class ENGINE_API Level : public RTTI
{
	// RTTI ����.
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// ���� �߰� �Լ�.
	template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
	std::shared_ptr<T> AddActor(T* newActor)
	{
		std::shared_ptr<T> sharedActor(newActor);
		return AddActor(sharedActor);
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
	std::shared_ptr<T> AddActor(std::shared_ptr<T> newActor)
	{
		addRequestedActors.emplace_back(newActor);
		return newActor;
	}

	// ���� ��û�� �� ���͸� �����ϴ� �Լ�.
	void ProcessAddedAndDestroyedActor();

	// ���� ó�� �Լ�.
	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	// ���� ������ ��ġ�Ǵ� ��ü(����) �迭.
	std::multiset<std::shared_ptr<Actor>, ActorComparator> actors;

	// �߰� ��û�� ����.
	std::vector<std::shared_ptr<Actor>> addRequestedActors;
};
