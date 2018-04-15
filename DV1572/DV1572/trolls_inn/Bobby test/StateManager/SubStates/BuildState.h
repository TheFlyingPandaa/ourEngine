#pragma once
#include "SubState.h"
class BuildState :
	public SubState
{
public:

	BuildState(Camera * cam, std::stack<Shape *>* pickingEvent);
	~BuildState();

	// Inherited via SubState
	virtual void _init() override;
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
};

