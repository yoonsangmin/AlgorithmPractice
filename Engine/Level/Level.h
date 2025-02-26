#pragma once

#include <vector>
#include <set>
#include "Core.h"
#include "RTTI.h"
#include "Container/List.h"
#include "Actor/Actor.h"
#include "Algorithm/NavigationSystem.h"

struct ActorComparator
{
	bool operator()(const std::shared_ptr<Actor>& lhs, const std::shared_ptr<Actor>& rhs) const
	{
		// Actor 클래스의 비교 연산자를 사용하여 비교
		return *lhs > *rhs;
	}
};

class ENGINE_API Level : public RTTI
{
	// RTTI 정의.
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// 액터 추가 함수.
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

    template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
    void FindActors(const Vector2& boundStart, const Vector2& boundEnd, std::vector<std::weak_ptr<T>>& outActors)
    {
        outActors.clear();
        for (const std::shared_ptr<Actor>& actor : actors)
        {
            if (actor->As<T>())
            {
                if (actor->position.x >= boundStart.x && actor->position.x <= boundEnd.x &&
                    actor->position.y >= boundStart.y && actor->position.y <= boundEnd.y)
                    if (auto castedActor = std::dynamic_pointer_cast<T>(actor))
                    {
                        outActors.emplace_back(castedActor);
                    }
            }
        }
    }

	// 삭제 요청이 된 액터를 정리하는 함수.
	void ProcessAddedAndDestroyedActor();

	// 루프 처리 함수.
	virtual void Update(float deltaTime);
	virtual void Draw();

    // 길 찾기 요청 함수.
    void FindPath(const Vector2& start, const Vector2& destination, std::deque<Vector2>* outPath);

    void UpdateMap(const Vector2& position, bool canWalk);
    bool CanWalk(const Vector2& position);

protected:
	// 게임 공간에 배치되는 물체(액터) 배열.
	std::multiset<std::shared_ptr<Actor>, ActorComparator> actors;

	// 추가 요청된 액터.
	std::vector<std::shared_ptr<Actor>> addRequestedActors;

    std::vector<std::vector<bool>> naviMap;
    std::vector<std::vector<bool>> collisonMap;
    NavigationSystem navigationSystem;
};
