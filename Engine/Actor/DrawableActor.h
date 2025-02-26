#pragma once

#include "Actor.h"

class ENGINE_API DrawableActor : public Actor
{
	// RTTI.
	RTTI_DECLARATIONS(DrawableActor, Actor)

public:
	DrawableActor(class Level* level, const char* image = "", bool isStatic = true, bool enableCollision = false, int priority = 0);
	//virtual ~DrawableActor() = default;
	virtual ~DrawableActor();

	virtual void Draw() override;

	// 충돌 확인 함수.
	bool Intersect(const DrawableActor& other);

	// Getter.
	inline int Width() const { return width; }

	// Setter.
	inline void SetColor(Color color) { this->color = color; }

protected:

	// 화면에 그릴 문자 값.
	char* image;

	// 너비(문자열 길이).
	int width = 0;

	// 색상 값.
	Color color = Color::White;

	// 충돌 가능한지 여부.
	bool enableCollision = false;
};