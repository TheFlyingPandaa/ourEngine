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

		end.x -= start.x - 1;
		end.y -= start.y - 1;

		m_buildStage = BuildStage::None;
		m_startTile = nullptr;
		m_selectedTile = nullptr;
		m_roomPlaceable = false;
		this->grid->AddRoom(start, end, m_selectedRoomType);
	}
}

void BuildState::Draw()
{
	
}
