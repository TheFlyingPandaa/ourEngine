#include "OurEngine.h"
#include "EventHandler.h"

EventHandler::EventHandler(Inn * inn, RoomCtrl * roomctl, std::vector<std::shared_ptr<Node>> firstPath, std::vector<std::shared_ptr<Node>> secondPath)
{
	this->m_inn = inn;
	this->m_roomCtrl = roomctl;
	this->m_pathToInn = firstPath;
	this->m_pathOutInn = secondPath;

	m_mailMan.setModel(MeshHandler::getBox());
	m_mailMan.setPosition(0.5f, 0.5f);
}

EventHandler::~EventHandler()
{
	
}

void EventHandler::_queueMailManPath(const bool inOrOut)
{
	if (inOrOut)
	{
		XMFLOAT3 oldPos = { 0.0f,0.0f, 0.0f };

		if (m_pathToInn.size() != 0)
		{
			//m_justMoved = false;

			m_mailMan.Move(m_mailMan.getDirectionFromPoint(oldPos, m_pathToInn[0]->tile->getQuad().getPosition()));

			for (int i = 0; i < m_pathToInn.size() - 1; i++)
			{
				//float lol = 255 * (float(i) / float(m_pathToInn.size()));
				//m_pathToInn[i + 1]->tile->getQuad().setColor(0, 0, lol);
				m_mailMan.Move(m_mailMan.getDirectionFromPoint(m_pathToInn[i]->tile->getQuad().getPosition(), m_pathToInn[i + 1]->tile->getQuad().getPosition()));
			}
		}
	}
	else
	{
		XMFLOAT3 oldPos = { (float)m_inn->getReceptionPos().x  ,0.0f, (float)m_inn->getReceptionPos().y };

		if (m_pathOutInn.size() != 0)
		{
			//m_justMoved = false;

			m_mailMan.Move(m_mailMan.getDirectionFromPoint(oldPos, m_pathOutInn[0]->tile->getQuad().getPosition()));

			for (int i = 0; i < m_pathOutInn.size() - 1; i++)
			{
				//float lol = 255 * (float(i) / float(m_pathToInn.size()));
				//m_pathToInn[i + 1]->tile->getQuad().setColor(0, 0, lol);
				m_mailMan.Move(m_mailMan.getDirectionFromPoint(m_pathOutInn[i]->tile->getQuad().getPosition(), m_pathOutInn[i + 1]->tile->getQuad().getPosition()));
			}
		}
	}

}

void EventHandler::Update()
{
	m_mailMan.Update();
	if ((int)m_mailMan.getPosition().x == m_inn->getReceptionPos().x && (int)m_mailMan.getPosition().y == m_inn->getReceptionPos().y && m_eventStart == true)
	{
		std::cout << "event Actualy started" << std::endl;
		m_currentCollectEvents.push(EventCollection(0, 3, 100, m_inn->getMoney()));
		m_eventStart = false;

		_queueMailManPath(false);

	}
	if (!m_currentCollectEvents.empty())
	{
		if (m_currentCollectEvents.top().getEventType() == 0)
		{
			m_currentCollectEvents.top().Update(m_inn->getMoney());
		}
		else if(m_currentCollectEvents.top().getEventType() == 1)
		{
			int amountOfObjects = 0;
			for (size_t i = 0; i < m_roomCtrl->getAllTheRooms().size(); i++)
			{
				amountOfObjects += m_roomCtrl->getAllTheRooms().at(i)->getAmountOfObjects();
			}
			m_currentCollectEvents.top().Update(amountOfObjects);
		}
	}
	if (!m_currentCollectEvents.empty())
	{
		if (m_currentCollectEvents.top().exitState())
		{
			Reward wop = m_currentCollectEvents.top().getReward();
			m_inn->Deposit(wop.getGoldReward());
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		}
	}

//#include "../../InGameConsole.h"
//	using namespace std::chrono_literals;
//
//	if (m_inn->getAngryCustomers() >= m_inn->getAngryCustomersCap())
//	{
//		InGameConsole::pushString("Paris hilton did not like your preformance");
//	}
//
}

void EventHandler::StartCollectEvent()
{
	//TODO: Spawn a mailman or quest giver
	
	if (m_eventStart == false)
	{
		m_eventStart = true;
		//This is temp, it will be changed later
		int amountOfObjects = 0;
		for (size_t i = 0; i < m_roomCtrl->getAllTheRooms().size(); i++)
		{
			amountOfObjects += m_roomCtrl->getAllTheRooms().at(i)->getAmountOfObjects();
		}

		//TODO: Spawn a mailman
		m_mailMan.setPosition(0.5f, 0.5f);
		_queueMailManPath(true);

		//m_currentCollectEvents.push(EventCollection(1, 3, 100, amountOfObjects));
	}
	
}

void EventHandler::EndEvent()
{
	if (!m_currentCollectEvents.empty())
	{
		
			m_inn->getMoney();
			Reward wop = m_currentCollectEvents.top().getReward();
			m_inn->Deposit(wop.getGoldReward());
			m_inn->getMoney();
			m_currentCollectEvents.pop();
			std::cout << "EventEnded" << std::endl;
		
	}
}

void EventHandler::Draw()
{
	m_mailMan.Draw();
}
