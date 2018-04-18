#include "BuildState.h"

void BuildState::_handleBuildRoom(Shape * pickedShape)
{
	switch (m_buildStage)
	{
	case BuildStage::Start:
		if (pickedShape)
		{
			m_startTile = pickedShape;
			m_buildStage = BuildStage::Selection;
		}
		else
			m_buildStage = BuildStage::None;
		break;
	case BuildStage::Selection:
		if (pickedShape)
			m_selectedTile = pickedShape;
		break;
	}
}

void BuildState::_buildInput()
{

	if (Input::isMouseLeftPressed())
	{
		if (m_buildStage == BuildStage::None)
		{
			m_buildStage = BuildStage::Start;
			this->grid->PickTiles();
		}
		else if (m_buildStage == BuildStage::Selection)
		{
			this->grid->PickTiles(m_selectedTile);
			if (m_startTile && m_selectedTile)
			{
				XMFLOAT3 s = m_startTile->getPosition();
				XMFLOAT3 e = m_selectedTile->getPosition();

				XMINT2 start;
				start.x = static_cast<int>(s.x + 0.5f);
				start.y = static_cast<int>(s.z + 0.5f);
				XMINT2 end;
				end.x = static_cast<int>(e.x + 0.5f);
				end.y = static_cast<int>(e.z + 0.5f);

				m_roomPlaceable = this->grid->CheckAndMarkTiles(start, end);
			}
		}
	}
	else if (m_buildStage == BuildStage::Selection && !Input::isMouseLeftPressed())
	{
		m_buildStage = BuildStage::End;
	}
	else if (m_buildStage == BuildStage::End && m_doorBuild) {
		XMFLOAT3 s = m_startTile->getPosition();
		XMFLOAT3 e = m_selectedTile->getPosition();

		XMINT2 start;
		start.x = static_cast<int>(s.x + 0.5f);
		start.y = static_cast<int>(s.z + 0.5f);
		XMINT2 end;
		end.x = static_cast<int>(e.x + 0.5f);
		end.y = static_cast<int>(e.z + 0.5f);

		if (start.x > end.x)
			std::swap(start.x, end.x);
		if (start.y > end.y)
			std::swap(start.y, end.y);
		this->grid->ResetTileColor(start, end);

		XMINT2 size = end;
		size.x -= start.x - 1;
		size.y -= start.y - 1;

		m_buildStage = BuildStage::None;
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		m_roomPlaceable = false;

		//If debugging is needed you got the size
		this->grid->AddDoor(start, end, size);


	}
	else if (m_buildStage == BuildStage::End)
	{
		XMFLOAT3 s = m_startTile->getPosition();
		XMFLOAT3 e = m_selectedTile->getPosition();

		XMINT2 start;
		start.x = static_cast<int>(s.x + 0.5f);
		start.y = static_cast<int>(s.z + 0.5f);
		XMINT2 end;
		end.x = static_cast<int>(e.x + 0.5f);
		end.y = static_cast<int>(e.z + 0.5f);

		if (start.x > end.x)
			std::swap(start.x, end.x);
		if (start.y > end.y)
			std::swap(start.y, end.y);
		this->grid->ResetTileColor(start, end);

		//This makes it a size.
		//this will change the end point
		end.x -= start.x - 1;
		end.y -= start.y - 1;

		m_buildStage = BuildStage::None;
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		m_roomPlaceable = false;
		this->grid->AddRoom(start, end, m_selectedRoomType);
	}

}

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
	if (Input::isKeyPressed('V'))
	{ 
		m_doorBuild = true;
	}
	else
	{
		m_doorBuild = false;
	}

	if (!this->p_pickingEvent->empty()) {
		Shape * obj = this->p_pickingEvent->top();
		this->p_pickingEvent->pop();

		if (m_buildStage != BuildStage::None)
			_handleBuildRoom(obj);
	}
	if (Input::isKeyPressed('E'))
		this->_exit();
	
	_buildInput();

}

void BuildState::Draw()
{
	
}

void BuildState::DrawHUD()
{
}
