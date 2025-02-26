#pragma once

#include <deque>
#include <vector>
#include "Level/Level.h"

class AStarLevel : public Level
{
	RTTI_DECLARATIONS(AStarLevel, Level)

public:
	AStarLevel(const char* path = "");

private:
	void LoadMap(const char* filePath);
};