#include "AStarLevel.h"
#include "Actor/Player.h"
#include "Actor/Wall.h"

AStarLevel::AStarLevel(const char* path)
{
	AddActor(new Player(this));

	LoadMap("../Assets/Map.txt");
}

void AStarLevel::LoadMap(const char* filePath)
{
	FILE* file = nullptr;

	fopen_s(&file, filePath, "r");

	if (file == nullptr)
	{
		OutputDebugStringA("File loading failed\n");
		return;
	}

	char buffer[256];
	int line = 0;
	// 한 줄씩 읽기.
	while (fgets(buffer, 256, file) != 0)
	{
		// 문자열 길이 구하기.
		int length = (int)strlen(buffer);

		std::vector<bool> lineData(length, true);

		// 문자열을 한 글자씩 읽어서 액터 생성.
		for (int ix = 0; ix < length; ++ix)
		{
			// 벽 생성.
			if (buffer[ix] == '1')
			{
				AddActor(new Wall(Vector2(ix, line), this));
				lineData[ix] = false;
			}
		}

		naviMap.emplace_back(lineData);
		// 줄 수 증가.
		++line;
	}

	fclose(file);
}
