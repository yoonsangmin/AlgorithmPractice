#include "DemoLevel.h"
#include "Actor/PlayerOld.h"
#include "Actor/Start.h"

DemoLevel::DemoLevel()
{
	AddActor(new Start());
	AddActor(new PlayerOld());
}