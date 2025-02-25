#pragma once

#include "Actor/DrawableActor.h"

class Marker : public DrawableActor
{
	RTTI_DECLARATIONS(Marker, DrawableActor)
	
public:
	Marker(const Vector2& position, const Color color, Level* level);

	virtual void Update(float deltaTime) override;

private:
	void Flicker();
};