#include "AStarLevel.h"
#include "Actor/Player.h"
#include "Actor/Wall.h"

AStarLevel::AStarLevel(const char* path)
{
	AddActor(new Player(this));

	LoadMap("../Assets/Map.txt");
}
	
void AStarLevel::FindPath(const Vector2& start, const Vector2& destination, std::deque<Vector2>* outPath)
{
	if (map[destination.y][destination.x])
	{
		navigationSystem.FindPath(start, destination, map, outPath);
	}
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
	// �� �پ� �б�.
	while (fgets(buffer, 256, file) != 0)
	{
		// ���ڿ� ���� ���ϱ�.
		int length = (int)strlen(buffer);

		std::vector<bool> lineData(length, true);

		// ���ڿ��� �� ���ھ� �о ���� ����.
		for (int ix = 0; ix < length; ++ix)
		{
			// �� ����.
			if (buffer[ix] == '1')
			{
				AddActor(new Wall(Vector2(ix, line), this));
				lineData[ix] = false;
			}
		}

		map.emplace_back(lineData);
		// �� �� ����.
		++line;
	}

	fclose(file);
}
