#pragma once
#include "SubState.h"
class StatsState :
	public SubState
{
private:



public:
	StatsState(Camera * cam, std::stack<Shape * >* pickingEvent);
	~StatsState();

	void HandlePicking(Shape * pickedObject) override;
	void HandleInput() override;
	void Update(double deltaTime) override;
	void Draw() override;
	void DrawHUD() override;

private:
	void _init() override;
};

