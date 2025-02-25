#pragma once

#include <queue>
#include <unordered_map>
#include <deque>
#include "math/Vector2.h"

// A* ��ã�� ����� ó���ϴ� Ŭ����.
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

	// ��� �˻� �Լ�.
	void FindPath(const Vector2& start, const Vector2& destination, const std::vector<std::vector<bool>>& map, std::deque<Vector2>* outPath);

private:
	// ���������� �������ؼ� ��� ����.
	void ConstructPath(Node goalNode, const Vector2& start, std::deque<Vector2>* outPath);

	// hCost ��� �Լ�.
	float CalculateHeuristic(const Vector2& position, const Vector2 destination);

	// Ž���Ϸ��� ��ġ�� ��ȿ�� ��ġ���� Ȯ��.
	bool IsValidPosition(const Vector2& position, const std::vector<std::vector<bool>>& map);

private:

	// ���� ����Ʈ.
	std::priority_queue<Node, std::vector<Node>> openList;

	// ���� ����Ʈ.
	std::unordered_map<Vector2, Node> closedList;
};
