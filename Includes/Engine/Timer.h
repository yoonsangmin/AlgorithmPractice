#pragma once

#include "Core.h"

class ENGINE_API Timer
{
public:
	Timer() = default;
	Timer(float time);

	void Update(float deltaTime);
	void Reset();

	inline void SetTimeOut(bool timeOut) { elapsedTime = timeOut ? setTime : 0; }
	inline bool IsTimeOut() const { return elapsedTime >= setTime; }
	inline void SetTime(float time) { setTime = time; }

private:
	float elapsedTime = 0.0f;
	float setTime = 0.0f;
};