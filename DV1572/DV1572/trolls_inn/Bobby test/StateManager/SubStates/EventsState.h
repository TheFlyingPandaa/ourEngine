#pragma once
#include "SubState.h"
class EventsState :
	public SubState
{
private:



public:
	EventsState(Camera * cam, std::stack<Shape * >* pickingEvent);
	~EventsState();

	void HandlePicking(Shape * pickedObject) override;
	void HandleInput() override;
	void Update(double deltaTime) override;
	void Draw() override;
	void DrawHUD() override;

private:
	void _init() override;
};

