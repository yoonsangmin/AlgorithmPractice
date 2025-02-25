#include "NavigationSystem.h"

void NavigationSystem::FindPath(const Vector2& start, const Vector2& destination, const std::vector<std::vector<bool>>& map, std::deque<Vector2>* outPath)
{
    outPath->clear();
	openList.push(Node(start, start));

	while (!openList.empty())
	{
		// 가장 작은 fCost를 가진 노드를 가져온다.
		Node currentNode = openList.top();
		openList.pop();

		// ClosedList에 들어있는 경우 무시한다.
		if (closedList.find(currentNode.position) != closedList.end())
		{
			continue;
		}

		// 현재 노드를 ClosedList에 추가한다.
		closedList[currentNode.position] = currentNode;

		// 목적지에 도착했으면 경로를 생성하고 종료한다.
		if (currentNode.position == destination)
		{
			ConstructPath(currentNode, start, outPath);
			break;
		}

		for (int ix = 0; ix < 4; ++ix)
		{
			Vector2 nextPosition = currentNode.position + directions[ix];

			// 다음 노드가 유효한지 확인.
			if (!IsValidPosition(nextPosition, map))
			{
				continue;
			}

			// 이미 ClosedList에 있는 경우 무시.
			if (closedList.find(nextPosition) != closedList.end())
			{
				continue;
			}

			// 다음 노드 값 설정.
			Node newNode(nextPosition, currentNode.position);
			newNode.gCost = currentNode.gCost + 1;
			newNode.hCost = CalculateHeuristic(nextPosition, destination);
			newNode.fCost = newNode.gCost + newNode.hCost;

			// 오픈 리스트에 추가.
			openList.push(newNode);
		}
	}

    // 길을 못 찾았을 경우.
    if (outPath->empty())
    {
        SetAlternativeDestination(start, destination, outPath);
    }

	// 리스트 비우기.
	while (!openList.empty()) 
	{
		openList.pop();
	}
	closedList.clear();
}

void NavigationSystem::ConstructPath(Node goalNode, const Vector2& start, std::deque<Vector2>* outPath)
{
	Vector2 currentPosition = goalNode.position;

	while (currentPosition != start)
	{
		outPath->push_front(currentPosition);
		currentPosition = closedList[currentPosition].previousPosition;
	}
}

void NavigationSystem::SetAlternativeDestination(const Vector2& start, const Vector2& destination, std::deque<Vector2>* outPath)
{
    // closedList에 있는 노드 중 목적지와 가장 가까운 노드를 찾는다.
    float minGCost = FLT_MAX;
    float minHCost = FLT_MAX;
    Vector2 alternativeDestination;
    for (auto& node : closedList)
    {
        if (node.second.position == start)
        {
            continue;
        }

        float hCost = node.second.hCost;
        float gCost = node.second.gCost;
        if (hCost < minHCost || hCost == minHCost && gCost < minGCost)
        {
            minHCost = hCost;
            minGCost = gCost;
            alternativeDestination = node.first;
        }
    }

    // 목적지로 설정.
    ConstructPath(closedList[alternativeDestination], start, outPath);
}

float NavigationSystem::CalculateHeuristic(const Vector2& position, const Vector2 destination)
{
	// 직선 거리 계산.
	return sqrtf(powf((float)destination.x - position.x, 2) + powf((float)destination.y - position.y, 2));
}

bool NavigationSystem::IsValidPosition(const Vector2& position, const std::vector<std::vector<bool>>& map)
{
	return position.x >= 0 && position.x < map[0].size()
		&& position.y >= 0 && position.y < map.size() && map[position.y][position.x];
}