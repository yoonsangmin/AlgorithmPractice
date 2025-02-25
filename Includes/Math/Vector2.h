#pragma once

#include "Core.h"

class ENGINE_API Vector2
{
public:
	Vector2(int x = 0, int y = 0);
	~Vector2() = default;

	// ������ �����ε�.
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;

public:
	// x��ǥ / y��ǥ.
	int x = 0;
	int y = 0;
};

// Vector2 �ؽ� �Լ�.
namespace std
{
	template<>
	struct hash<Vector2>
	{
		size_t operator()(const Vector2& vector) const
		{
			return hash<int>()(vector.x) ^ hash<int>()(vector.y);
		}
	};
};