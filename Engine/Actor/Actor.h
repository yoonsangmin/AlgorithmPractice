#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Math/Vector2.h"

// 레벨의 기본 물체.
class ENGINE_API Actor : public RTTI
{
	// RTTI 선언.
	RTTI_DECLARATIONS(Actor, RTTI)

	// Level 클래스를 friend로 선언.
	// private에 접근이 가능하도록.
	friend class Level;

public:
	Actor(class Level* level, int priority = 0);
	virtual ~Actor();

	// 루프 처리 함수.
	virtual void Update(float delatTime);
	virtual void Draw();

	// Getter/Setter.
	virtual bool SetPosition(const Vector2& newPosition);
	inline Vector2 Position() const;

	inline void SetPriority(int newPirority) { priority = newPirority; }
	inline bool IsAcive() const { return isActive && !isExpired; }
    inline void SetActive(bool active) { isActive = active; }

	inline void Destroy() { isExpired = true; }
    inline bool IsVisible() const { return isVisible; }
    inline void SetVisibility(bool visible) { isVisible = visible; }
    inline bool IsStatic() const { return isStatic; }

	// 연산자 오버로딩.
	bool operator>(const Actor& other) const
	{
		return priority > other.priority;
	}

protected:
	// 소유한 레벨.
	class Level* level = nullptr;

	// 액터의 위치.
	Vector2 position;

	// Update, Draw 호출 우선 순위.
	int priority;

	// ID(해시) / 이름 값.
	// 활성화 상태인지를 나타내는 변수.
	bool isActive;

	// 액터의 제거 요청이 됐는지 여부를 나타내는 변수.
	bool isExpired;

	// 화면에 그릴지.
	bool isVisible = true;

    // 정적 액터인지.
    bool isStatic = true;

    // 충돌 가능한지 여부.
    bool isCollisionEnabled = false;
};