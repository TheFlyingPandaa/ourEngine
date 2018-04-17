#include "BuildState.h"



BuildState::BuildState(Camera * cam, 
	std::stack<Shape *>* pickingEvent,
	Grid * grid) : SubState(cam, pickingEvent)
{
	this->grid = grid;
	this->_init();
	m_buildStage = BuildStage::None;
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
