#include "StatsState.h"

StatsState::StatsState(Camera * cam, std::stack<Shape*>* pickingEvent)
	: SubState(cam, pickingEvent)
{
	_init();
}

StatsState::~StatsState()
{
}

void StatsState::HandlePicking(Shape * pickedObject)
{
}

void StatsState::HandleInput()
{
}

void StatsState::Update(double deltaTime)
{
}

void StatsState::Draw()
{
}

void StatsState::DrawHUD()
{
	p_HUD.Draw();
}

void StatsState::_init()
{
	p_HUD.LoadHud("trolls_inn/Resources/HUD/StatsHud/StatsHud.txt");
}
