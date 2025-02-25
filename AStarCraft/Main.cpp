#include "Engine/Engine.h"
#include "Level/AStarLevel.h"

int main()
{
	Engine engine;
	engine.LoadLevel(new AStarLevel());
	engine.Run();
}