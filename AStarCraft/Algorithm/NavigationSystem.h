#pragma once

#include <queue>
#include <unordered_map>
#include <deque>
#include "math/Vector2.h"

// A* 길찾기 기능을 처리하는 클래스.
class NavigationSystem
{
private:
	struct Node
	{
		Node() = default;
		Node(const Vector2& position, const Vector2& previousPosition)
			: position(position), previousPosition(previousPosition)
		{
		}

		bool operator<(const Node& other) const
		{
			return fCost > other.fCost;
		}

		bool operator==(const Node& other) const
		{
			return position == other.position;
		}

		Vector2 position;
		Vector2 previousPosition;
		float gCost = 0.0f;
		float hCost = 0.0f;
		float fCost = 0.0f;;
	};

	Vector2 directions[4] =
	{
		Vector2(0, 1),
		Vector2(1, 0),
		Vector2(0, -1),
		Vector2(-1, 0)
	};

public:
	NavigationSystem() = default;
	~NavigationSystem() = default;

	// 경로 검색 함수.
	void FindPath(const Vector2& start, const Vector2& destination, const std::vector<std::vector<bool>>& map, std::deque<Vector2>* outPath);

private:
	// 목적지에서 역추적해서 경로 생성.
	void ConstructPath(Node goalNode, const Vector2& start, std::deque<Vector2>* outPath);

	// hCost 계산 함수.
	float CalculateHeuristic(const Vector2& position, const Vector2 destination);

	// 탐색하려는 위치가 유효한 위치인지 확인.
	bool IsValidPosition(const Vector2& position, const std::vector<std::vector<bool>>& map);

private:

	// 열린 리스트.
	std::priority_queue<Node, std::vector<Node>> openList;

	// 닫힌 리스트.
	std::unordered_map<Vector2, Node> closedList;
};
