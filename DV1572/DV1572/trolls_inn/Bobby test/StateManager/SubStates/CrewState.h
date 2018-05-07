#pragma once
#include "SubState.h"
#include "../../Room/Grid.h"
class CrewState :
	public SubState
{
private:



public:
	CrewState(Camera * cam, std::stack<Shape * >* pickingEvent);
	~CrewState();

	void HandlePicking(Shape * pickedObject) override;
	void HandleInput() override;
	void Update(double deltaTime) override;
	void Draw() override;
	void DrawHUD() override;

private:
	void _init() override;
};

