#include "EventsState.h"

EventsState::EventsState(Camera * cam, std::stack<Shape*>* pickingEvent)
	: SubState(cam, pickingEvent)
{
	_init();
}

EventsState::~EventsState()
{
}

void EventsState::HandlePicking(Shape * pickedObject)
{
}

void EventsState::HandleInput()
{
}

void EventsState::Update(double deltaTime)
{
}

void EventsState::Draw()
{
}

void EventsState::DrawHUD()
{
	p_HUD.Draw();
}

void EventsState::_init()
{
	p_HUD.LoadHud("trolls_inn/Resources/HUD/EventsHud/EventsHud.txt");
}
