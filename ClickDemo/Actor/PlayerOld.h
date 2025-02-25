#pragma once

#include "Actor/DrawableActor.h"

class PlayerOld : public DrawableActor
{
	RTTI_DECLARATIONS(PlayerOld, DrawableActor)

public:
	PlayerOld();

	virtual void Update(float deltaTime) override;
};