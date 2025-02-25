#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Math/Vector2.h"

// ������ �⺻ ��ü.
class ENGINE_API Actor : public RTTI
{
	// RTTI ����.
	RTTI_DECLARATIONS(Actor, RTTI)

	// Level Ŭ������ friend�� ����.
	// private�� ������ �����ϵ���.
	friend class Level;

public:
	Actor(class Level* level, int priority = 0);
	virtual ~Actor();

	// ���� ó�� �Լ�.
	virtual void Update(float delatTime);
	virtual void Draw();

	// Getter/Setter.
	virtual void SetPosition(const Vector2& newPosition);
	inline Vector2 Position() const;

	inline void SetPriority(int newPirority) { priority = newPirority; }
	inline bool IsAcive() const { return isActive && !isExpired; }
	inline void SetActive(bool active) { isActive = active; }
	inline void Destroy() { isExpired = true; }
	inline bool IsVisible() { return isVisible; }
	inline void SetVisibility(bool visible) { this->isVisible = visible; }

	// ������ �����ε�.
	bool operator>(const Actor& other) const
	{
		return priority > other.priority;
	}

protected:
	// ������ ����.
	class Level* level = nullptr;

	// ������ ��ġ.
	Vector2 position;

	// Update, Draw ȣ�� �켱 ����.
	int priority;

	// ID(�ؽ�) / �̸� ��.
	// Ȱ��ȭ ���������� ��Ÿ���� ����.
	bool isActive;

	// ������ ���� ��û�� �ƴ��� ���θ� ��Ÿ���� ����.
	bool isExpired;

	// ȭ�鿡 �׸���.
	bool isVisible = true;
};