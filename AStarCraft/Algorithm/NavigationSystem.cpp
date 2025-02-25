#include "NavigationSystem.h"

void NavigationSystem::FindPath(const Vector2& start, const Vector2& destination, const std::vector<std::vector<bool>>& map, std::deque<Vector2>* outPath)
{
	openList.push(Node(start, start));

	while (!openList.empty())
	{
		// ���� ���� fCost�� ���� ��带 �����´�.
		Node currentNode = openList.top();
		openList.pop();

		// ClosedList�� ����ִ� ��� �����Ѵ�.
		if (closedList.find(currentNode.position) != closedList.end())
		{
			continue;
		}

		// ���� ��带 ClosedList�� �߰��Ѵ�.
		closedList[currentNode.position] = currentNode;

		// �������� ���������� ��θ� �����ϰ� �����Ѵ�.
		if (currentNode.position == destination)
		{
			ConstructPath(currentNode, start, outPath);
			break;
		}

		for (int ix = 0; ix < 4; ++ix)
		{
			Vector2 nextPosition = currentNode.position + directions[ix];

			// ���� ��尡 ��ȿ���� Ȯ��.
			if (!IsValidPosition(nextPosition, map))
			{
				continue;
			}

			// �̹� ClosedList�� �ִ� ��� ����.
			if (closedList.find(nextPosition) != closedList.end())
			{
				continue;
			}

			// ���� ��� �� ����.
			Node newNode(nextPosition, currentNode.position);
			newNode.gCost = currentNode.gCost + 1;
			newNode.hCost = CalculateHeuristic(nextPosition, destination);
			newNode.fCost = newNode.gCost + newNode.hCost;

			// ���� ����Ʈ�� �߰�.
			openList.push(newNode);
		}
	}

	// ����Ʈ ����.
	while (!openList.empty()) 
	{
		openList.pop();
	}
	closedList.clear();
}

void NavigationSystem::ConstructPath(Node goalNode, const Vector2& start, std::deque<Vector2>* outPath)
{
	outPath->clear();
	Vector2 currentPosition = goalNode.position;

	while (currentPosition != start)
	{
		outPath->push_front(currentPosition);
		currentPosition = closedList[currentPosition].previousPosition;
	}
}

float NavigationSystem::CalculateHeuristic(const Vector2& position, const Vector2 destination)
{
	// ���� �Ÿ� ���.
	return sqrtf(powf((float)destination.x - position.x, 2) + powf((float)destination.y - position.y, 2));
}

bool NavigationSystem::IsValidPosition(const Vector2& position, const std::vector<std::vector<bool>>& map)
{
	return position.x >= 0 && position.x < map[0].size()
		&& position.y >= 0 && position.y < map.size() && map[position.y][position.x];
}