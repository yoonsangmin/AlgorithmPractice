#pragma once

#include <deque>
#include <vector>
#include "Level/Level.h"
#include "Algorithm/NavigationSystem.h"

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const char* path = "");

	void FindPath(const Vector2& start, const Vector2& destination, std::deque<Vector2>* outPath);

private:
	void LoadMap(const char* filePath);

private:
	std::vector<std::vector<bool>> map;
	NavigationSystem navigationSystem;
};