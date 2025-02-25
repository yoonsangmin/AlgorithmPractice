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

	// �浹 Ȯ�� �Լ�.
	bool Intersect(const DrawableActor& other);

	// Getter.
	inline int Width() const { return width; }

	// Setter.
	inline void SetColor(Color color) { this->color = color; }

protected:

	// ȭ�鿡 �׸� ���� ��.
	char* image;

	// �ʺ�(���ڿ� ����).
	int width = 0;

	// ���� ��.
	Color color = Color::White;

	// ���� ��ü����.
	bool isStatic = true;

	// �浹 �������� ����.
	bool enableCollision = false;
};