#include "BuildState.h"



BuildState::BuildState(Camera * cam, std::stack<Shape *>* pickingEvent) : SubState(cam, pickingEvent)
{
	this->_init();
}


BuildState::~BuildState()
{
}

void BuildState::_init()
{
}

void BuildState::Update(double deltaTime)
{
}

void BuildState::Draw()
{
}
