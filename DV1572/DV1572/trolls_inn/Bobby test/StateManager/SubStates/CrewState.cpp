#include "CrewState.h"

CrewState::CrewState(Camera * cam, std::stack<Shape*>* pickingEvent)
	: SubState(cam, pickingEvent)
{
	_init();
}

CrewState::~CrewState()
{
}

void CrewState::HandlePicking(Shape * pickedObject)
{
}

void CrewState::HandleInput()
{
}

void CrewState::Update(double deltaTime)
{
}

void CrewState::Draw()
{
}

void CrewState::DrawHUD()
{
	p_HUD.Draw();
}

void CrewState::_init()
{
	p_HUD.LoadHud("trolls_inn/Resources/HUD/CrewHud/CrewHud.txt");
}
