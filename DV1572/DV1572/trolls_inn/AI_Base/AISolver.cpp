﻿#include "AISolver.h"
#define CAST(t,v) static_cast<t>(v)
// round float to n decimals precision
float round_n2(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}

void AISolver::_checkSpotInRoom(Inn* inn, Customer& customer)
{
	XMINT2 spotAvailable = XMINT2(0,0);

	if (m_roomctrl != nullptr)
	{
		if (customer.GetState() == Sleeping ||
			customer.GetState() == Eating ||
			customer.GetState() == Drinking)
		{
			spotAvailable = customer.findNearestRoom(m_roomctrl, customer.GetState());
		}
		else if (customer.GetState() == Waiting)
			spotAvailable = customer.findNearestRoom(m_roomctrl, customer.GetWaitingToDoState());
	}
	if (spotAvailable.x != -1 && spotAvailable.y != -1)
	{
		CustomerState state = customer.GetState();
		int price = 0;
		std::stringstream ss;

		if (Waiting == state)
		{
			customer.PopToNextState();
			state = customer.GetState();
		}
		// Get the seat/bed ID to lock it to the customer
		switch (state)
		{
		case Drinking:
			price = inn->GetDrinkPrice();
			ss << "A customer bought a drink. \nYou earned " << std::to_string(price) << " gold!" << std::endl;
			break;
		case Eating:
			price = inn->GetFoodPrice();
			ss << "A customer bought a snack. \nYou earned " << std::to_string(price) << " gold!" << std::endl;
			break;
		case Sleeping:
			price = inn->GetSleepPrice();
			ss << "A customer rented a room. \nYou earned " << std::to_string(price) << " gold!" << std::endl;
			break;
		}
		InGameConsole::pushString(ss.str());
		customer.GetEconomy().Withdraw(price);
		inn->Deposit(price);
		customer.SetAvailableSpotFound(true);
	}
	else if (!customer.GetWaitingForSpot())
	{
		customer.SetAvailableSpotFound(false);
		customer.SetWaitingForSpot(true);
		customer.SetWaiting();
	}
}

void AISolver::_doWaiting(Customer& customer, Inn* inn)
{
	if ((this->m_rNG.GenerateRandomNumber(1, 5) * customer.GetWaitingForSpotMultiplier()) > (WAITING_FOR_SPOT_TIME_LIMIT * customer.GetPatience()))
	{
		std::stringstream ss;
		if (customer.GetRace() == Elf)
		{
			ss << "An Elf waited for too long to\nget service and is now leaving.\nYou lost 50 gold!" << std::endl;
			inn->GetRefund(50);
			customer.GetEconomy().GetCashback(50);
			customer.GetAttributes().AddStat(-0.15f);
		}
		else
		{
			ss << "A Dwarf waited for too long to\nget service and is now leaving.\nYou lost 70 gold!" << std::endl;
			inn->GetRefund(70);
			customer.GetEconomy().GetCashback(70);
			customer.GetAttributes().AddStat(0.15f);
		}
		InGameConsole::pushString(ss.str());
		customer.SetWaitingForSpot(false);
		// Do angry face emote
		// Leave inn (?)
		customer.RestartClock();
		customer.setThoughtBubble(Character::ANGRY);
		customer.PopStateQueue();
	}
	else
	{
		customer.SetWaitingForSpotMultiplier(customer.GetWaitingForSpotMultiplier() + 1);
	}
}

std::vector<std::shared_ptr<Node>> AISolver::GetPathAndSmokeGrass(XMINT2 startPosition, XMINT2 targetPosition)
{
	Tile* startTile = m_grid->getTile(startPosition.x, startPosition.y);
	Tile* targetTile = m_grid->getTile(targetPosition.x, targetPosition.y);

	std::vector<std::shared_ptr<Node>> path;
	// Outside -> OutSide
	if (startTile && targetTile)
	{
		path = m_grid->findPath(startTile, targetTile);
	}
	// Outside -> inside
	else if (startTile && 0 == targetTile)
	{
		XMINT2 pos = targetPosition;
		int index = m_roomctrl->_intersect(pos);
		Room* targetRoom = m_roomctrl->getRoomAt(index);

		// If the target room has no connecting doors
		if (m_roomctrl->getRoomConnections(index) == false)
		{
			std::cout << "Has no connected rooms!\n";
			return std::vector<std::shared_ptr<Node>>();
		}
	
		auto entranceDoors = m_roomctrl->getAllEntranceDoors();
		Room* entranceRoom = nullptr; 
		int thisindex = -1;
		int highestCon = 1;
		// Test all the entrance doors if we can get to the target
		for (int i = 0; i < entranceDoors.size(); i++)
		{
			int eIndex = m_roomctrl->_intersect(entranceDoors[i].two);
			Room* eRoom = m_roomctrl->getRoomAt(eIndex);
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(eRoom->getTile(entranceDoors[i].two.x, entranceDoors[i].two.y), targetRoom->getTile(pos.x, pos.y));
			if (roomIndexes.size() > highestCon)
			{
				entranceRoom = eRoom;
				thisindex = i;
				highestCon = CAST(int,roomIndexes.size());
			}
			else // This is probably the same room
			{
				// If the target room is located in the entrance room we found it!
				if (*eRoom == *targetRoom)
				{
					entranceRoom = targetRoom;
					thisindex = i;
				}
			}
		}
		if (thisindex == -1) // This means that we didnt find any entrance doors!
		{
			std::cout << "We didnt find any entrance doors\n";
			return std::vector<std::shared_ptr<Node>>();
		}

		// Now do we wanna walk to the entrance
		path = m_grid->findPath(startTile, m_grid->getTile(entranceDoors[thisindex].one.x, entranceDoors[thisindex].one.y));

		//Lets not talk about this one(This is so we walk straight through the door...)
		std::vector<std::shared_ptr<Node>> lol1;
		std::shared_ptr<Node> current(new Node(entranceRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y), nullptr, 0, 0));
		lol1.push_back(current);
		path.insert(path.end(), lol1.begin(), lol1.end());

		// Easy check, if its the main room we are wanting then we only need to perform pathfinding inside that room
		// and not start the higher level room path finding
		if (*targetRoom == *entranceRoom)
		{
			auto doorToEndTile = targetRoom->findPath(targetRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y), targetRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		// We want to be advanced :P
		else
		{

			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(entranceRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y), targetRoom->getTile(pos.x, pos.y));

			Room* startRoom = entranceRoom;
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = entranceDoors[thisindex].two;
			for (int i = 0; i < roomIndexes.size() - 1; i++)
			{
				// Between this rooms leave door and other rooms enter door
				RoomCtrl::DoorPassage dp = m_roomctrl->getDoorPassage(roomIndexes[i], roomIndexes[i + 1]);

				auto toOtherRoom = startRoom->findPath(startRoom->getTile(startPosition.x, startPosition.y), startRoom->getTile(dp.one.x, dp.one.y));
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = dp.two;
				startRoom = m_roomctrl->getRoomAt(dp.roomIndexes[1]);

				std::vector<std::shared_ptr<Node>> lol;
				std::shared_ptr<Node> current(new Node(startRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), nullptr, 0, 0));
				lol.push_back(current);
				path.insert(path.end(), lol.begin(), lol.end());
				startPosition = DoorLeavePos;

			}


			auto toTarget = startRoom->findPath(startRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), startRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), toTarget.begin(), toTarget.end());

		}

	}
	// Inside -> Inside
	else if (0 == startTile && 0 == targetTile)
	{
		XMINT2 pos = targetPosition;
		int index = m_roomctrl->_intersect(pos);
		Room* targetRoom = m_roomctrl->getRoomAt(index);

		XMINT2 tPos = startPosition;
		int indexForTarget = m_roomctrl->_intersect(tPos);
		Room* startRoom = m_roomctrl->getRoomAt(indexForTarget);

		if (0 == m_roomctrl->getRoomConnections(index))
		{
			std::cout << "Has no connected rooms\n";
			return std::vector<std::shared_ptr<Node>>();
		}

		if (*targetRoom == *startRoom)
		{
			auto doorToEndTile = targetRoom->findPath(targetRoom->getTile(tPos.x, tPos.y), targetRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		else
		{
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(startRoom->getTile(tPos.x, tPos.y), targetRoom->getTile(pos.x, pos.y));
			// if this is equal to zero then the rooms are not connected.
			if (roomIndexes.size() == 0)
			{
				auto entranceDoors = m_roomctrl->getAllEntranceDoors();
				Room* entranceRoom = nullptr;
				int thisindex = -1;
				int highestCon = 1;
				// Test all the entrance doors if we can get to the target
				for (int i = 0; i < entranceDoors.size(); i++)
				{
					int eIndex = m_roomctrl->_intersect(entranceDoors[i].two);
					Room* eRoom = m_roomctrl->getRoomAt(eIndex);
					std::vector<int> roomIndexes = m_roomctrl->roomTraversal(startRoom->getTile(tPos.x, tPos.y), eRoom->getTile(entranceDoors[i].two.x, entranceDoors[i].two.y));
					if (roomIndexes.size() > highestCon)
					{
						entranceRoom = eRoom;
						thisindex = i;
						highestCon = roomIndexes.size();
					}
					else // This is probably the same room
					{
						// If the target room is located in the entrance room we found it!
						if (*eRoom == *startRoom)
						{
							entranceRoom = eRoom;
							thisindex = i;
						}
					}
				}
				if (thisindex == -1) // This means that we didnt find any entrance doors!
				{
					std::cout << "We didnt find any entrance doors\n";
					return std::vector<std::shared_ptr<Node>>();
				}

		
				path = GetPathAndSmokeGrass(tPos, entranceDoors[thisindex].two);
				Room* cRoom = m_roomctrl->getRoomAt(entranceDoors[thisindex].roomIndexes[1]);

				std::vector<std::shared_ptr<Node>> lol;
				
				std::shared_ptr<Node> current(new Node(m_grid->getTile(entranceDoors[thisindex].one.x, entranceDoors[thisindex].one.y), nullptr, 0, 0));
				lol.push_back(current);
				path.insert(path.end(), lol.begin(), lol.end());
				auto newPath = GetPathAndSmokeGrass(entranceDoors[thisindex].one, targetPosition);
				path.insert(path.end(), newPath.begin(), newPath.end());
				return path;
			}

			Room* cRoom = startRoom;
			XMINT2 DoorLeavePos;
			XMINT2 sp2 = startPosition;
			for (int i = 0; i < roomIndexes.size() - 1; i++)
			{
				RoomCtrl::DoorPassage dp = m_roomctrl->getDoorPassage(roomIndexes[i], roomIndexes[i + 1]);

				auto toOtherRoom = cRoom->findPath(cRoom->getTile(sp2.x, sp2.y), cRoom->getTile(dp.one.x, dp.one.y));
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = dp.two;
				cRoom = m_roomctrl->getRoomAt(dp.roomIndexes[1]);

				std::vector<std::shared_ptr<Node>> lol;
				std::shared_ptr<Node> current(new Node(cRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), nullptr, 0, 0));
				lol.push_back(current);
				path.insert(path.end(), lol.begin(), lol.end());
				sp2 = DoorLeavePos;

			}
			auto toTarget = cRoom->findPath(cRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), cRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), toTarget.begin(), toTarget.end());
		}

	}
	//Inside to outside
	else
	{
		XMINT2 pos = startPosition;
		int index = m_roomctrl->_intersect(pos);
		Room* targetRoom = m_roomctrl->getRoomAt(index);

		auto entranceDoors = m_roomctrl->getAllEntranceDoors();
		Room* entranceRoom = nullptr;
		int thisindex = -1;
		// Test all the entrance doors if we can get to the target
		for (int i = 0; i < entranceDoors.size(); i++)
		{
			int eIndex = m_roomctrl->_intersect(entranceDoors[i].two);
			Room* eRoom = m_roomctrl->getRoomAt(eIndex);
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(eRoom->getTile(entranceDoors[i].two.x, entranceDoors[i].two.y), targetRoom->getTile(pos.x, pos.y));
			if (roomIndexes.size())
			{
				entranceRoom = eRoom;
				thisindex = i;
			}
			else // This is probably the same room
			{
				// If the target room is located in the entrance room we found it!
				if (*eRoom == *targetRoom)
				{
					entranceRoom = targetRoom;
					thisindex = i;
				}
			}
		}

		XMINT2 tPos = targetPosition;
		
		// Somehow the entranceroom went nullptr
		if (!entranceRoom || !targetRoom)
		{
			std::cout << "entranceRoom or targetRoom was nullptr\n";
			return std::vector<std::shared_ptr<Node>>();
		}

		if (*entranceRoom == *targetRoom)
		{

			XMINT2 pos = startPosition;
			int index = m_roomctrl->_intersect(pos);
			Room* targetRoom = m_roomctrl->getRoomAt(index);

			// Path to door
			auto toDoor = targetRoom->findPath(targetRoom->getTile(startPosition.x, startPosition.y), targetRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y));
			path.insert(path.end(), toDoor.begin(), toDoor.end());

			//Lets not talk about this one(This is so we walk straight through the door...)
			auto walkThroughDoor = m_grid->findPath(targetRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y), m_grid->getTile(entranceDoors[thisindex].one.x, entranceDoors[thisindex].one.y));
			path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

			auto walkToTarget = m_grid->findPath(m_grid->getTile(entranceDoors[thisindex].one.x, entranceDoors[thisindex].one.y), targetTile);
			path.insert(path.end(), walkToTarget.begin(), walkToTarget.end());
		}
		else
		{
			// We need to find the entrace from inside then we path to the destination
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(targetRoom->getTile(startPosition.x, startPosition.y), entranceRoom->getTile(entranceDoors[thisindex].two.x, entranceDoors[thisindex].two.y));

			Room* startRoom = targetRoom;
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = pos;
			for (int i = 0; i < roomIndexes.size() - 1; i++)
			{
				// Between this rooms leave door and other rooms enter door
				RoomCtrl::DoorPassage dp = m_roomctrl->getDoorPassage(roomIndexes[i], roomIndexes[i + 1]);

				auto toOtherRoom = startRoom->findPath(startRoom->getTile(startPosition.x, startPosition.y), startRoom->getTile(dp.one.x, dp.one.y));
				path.insert(path.end(), toOtherRoom.begin(), toOtherRoom.end());

				// Smooth Entering
				DoorLeavePos = dp.two;
				startRoom = m_roomctrl->getRoomAt(dp.roomIndexes[1]);

				std::vector<std::shared_ptr<Node>> lol;
				std::shared_ptr<Node> current(new Node(startRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), nullptr, 0, 0));
				lol.push_back(current);
				path.insert(path.end(), lol.begin(), lol.end());
				startPosition = DoorLeavePos;

			}

			RoomCtrl::DoorPassage entranceDoor = m_roomctrl->getClosestEntranceDoor(startPosition);
			Room* entranceRoom = m_roomctrl->getRoomAt(entranceDoor.roomIndexes[1]);

			auto toMainLeave = startRoom->findPath(startRoom->getTile(DoorLeavePos.x, DoorLeavePos.y), entranceRoom->getTile(entranceDoor.two.x, entranceDoor.two.y));
			path.insert(path.end(), toMainLeave.begin(), toMainLeave.end());

			std::vector<std::shared_ptr<Node>> lol3;
			std::shared_ptr<Node> current(new Node(m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y), nullptr, 0, 0));
			lol3.push_back(current);
			path.insert(path.end(), lol3.begin(), lol3.end());

			auto finallyTarget = m_grid->findPath(m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y), targetTile);
			path.insert(path.end(), finallyTarget.begin(), finallyTarget.end());

		}



	}
	return path;
}

AISolver::AISolver(RoomCtrl *roomctrl, Grid* grid)
{
	this->m_grid = grid;
	this->m_roomctrl = roomctrl;
	this->m_start = this->m_clock.now();
}

AISolver::~AISolver()
{
	
}

std::chrono::duration<double> AISolver::getTimeSpan() const
{
	return this->m_time_span;
}

void AISolver::RestartClock()
{
	this->m_start = this->m_clock.now();
}

void AISolver::Update(Customer& customer, Inn* inn)
{
	// Get the elapsed time
	this->m_now = this->m_clock.now();
	this->m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(this->m_now - this->m_start);

	CustomerState currentState = customer.GetState();
	customer.Update();

	
	if (currentState == WalkingToInn)
	{
		if (customer.walkQueueDone())
		{
			if (customer.getPosition().y < 0)
			{
				RandomNumberGenerator gen;

				int depth = gen.GenerateRandomNumber(2, 4);
				customer.setPosition(0.5f, -depth + 0.5f);
				// Walk along the catwalk then upwards towards the gridsystem where the rooms are located
				customer.clearWalkingQueue();
				for (int i = 0; i < 16; ++i)
					customer.Move(Character::WalkDirection::RIGHT);
				for (int i = 0; i < depth + 1; ++i)
					customer.Move(Character::WalkDirection::UP);

			}
			else
			{
				// if reception thread is finished, then we pop this pussy
				int status = RequestPath(customer, RoomType::reception);
				if (status == 1)
				{
					customer.PopToNextState();
					customer.SetAction(WalkAction);
					currentState = customer.GetState();

				}
				else if (status == -1)
				{

					customer.RestartClock();
					customer.setThoughtBubble(Character::ANGRY);
				}

			}
		}
	}

	if (currentState == Walking)
	{
	
		if (customer.walkQueueDone())
		{
			customer.PopToNextState();
			currentState = customer.GetState();
		}
	}

	if (currentState != Walking)
	{
	
		switch (currentState)
		{
		case Idle:
			// Calculate new desire
			// Do customer want to walk around or take an action
			if (customer.GetHungry() < 5 && customer.GetThirsty() < 5 && customer.GetTired() < 5)
			{
				if (customer.walkQueueDone())
				{
					// Get a path to a new location
					RequestPath(customer, RoomType::randomStupid);
					customer.SetAction(WalkAction);

				}
			}
			customer.PopToNextState();
			break;
		case Waiting:
			if (customer.GetWaitingForSpot())
			{
				// Check if there is an open spot
				_checkSpotInRoom(inn, customer);
			}
			if (customer.GetWaitingForSpot() && m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
			{
				_doWaiting(customer, inn);
			}
			
			break;
		case Drinking:
			if (!customer.GetAvailableSpotFound())
				_checkSpotInRoom(inn, customer);
			
			if (customer.GetThirsty() > 0)
			{
				customer.DoDrinking();
				customer.PopStateQueue();
				customer.SetAction(DrinkAction);
				customer.SetAction(Action::WaitingAction);
			}
			else
			{
				customer.PopToNextState();
				customer.releaseFurniture();
			}
				
			
			break;
		case Eating:
			if (!customer.GetAvailableSpotFound())
				_checkSpotInRoom(inn, customer);

			
			if (customer.GetHungry() > 0)
			{
				customer.DoEating();
				customer.PopStateQueue();
				customer.SetAction(EatAction);
				customer.SetAction(Action::WaitingAction);
			}
			else
			{
				customer.PopToNextState();
				customer.releaseFurniture();
			}
			
			break;
		case Sleeping:
			if (!customer.GetAvailableSpotFound())
				_checkSpotInRoom(inn, customer);
		
			if (customer.GetTired() > 0)
			{
				customer.DoSleeping();
				customer.PopStateQueue();
				customer.SetAction(SleepAction);
				customer.SetAction(Action::WaitingAction);
			}
			else
			{
				customer.PopToNextState();
				customer.releaseFurniture();
			}
			
			break;
		}
	}
}

void AISolver::Update(Customer& customer, Action desiredAction)
{
	CustomerState currentState = customer.GetState();	
	XMINT2 targetPosition = customer.getOwnerFurniturePosition();
	int gotPath = -1;
	if(targetPosition.x != -1)
		gotPath = RequestPath(customer, targetPosition);
	else
	{
		switch (currentState)
		{
		case Thinking:
			switch (desiredAction)
			{
			case DrinkAction:
				targetPosition = customer.findNearestRoom(m_roomctrl, Drinking);
				if (targetPosition.x == -1)
					break;
				gotPath = RequestPath(customer, targetPosition);
				break;
			case EatAction:
				targetPosition = customer.findNearestRoom(m_roomctrl, Eating);
				if (targetPosition.x == -1)
					break;
				gotPath = RequestPath(customer, targetPosition);
				break;
			case SleepAction:
				targetPosition = customer.findNearestRoom(m_roomctrl, Sleeping);
				if (targetPosition.x == -1)
					break;
				gotPath = RequestPath(customer, targetPosition);
				break;
			}

			break;
		}
	}

	customer.PopToNextState(); // pop Thinking state
	customer.PopToNextState(); // pop Idle state
	if (gotPath == 1)
	{
		customer.GotPathSetNextAction(desiredAction, m_roomctrl);
		customer.SetAvailableSpotFound(false);
		customer.SetWaitingForSpot(false);
		customer.SetWaitingForSpotMultiplier(1);
	}
	else if (gotPath == -1)
	{
		std::stringstream ss;
		ss << "A customer is leaving. The\ncustomers needs could not be\nfulfilled!" << std::endl;
		InGameConsole::pushString(ss.str());
		customer.RestartClock();
		customer.setThoughtBubble(Character::ANGRY);
	}
	
	
}

void AISolver::Update(Staff& staff)
{
	
	staff.Update();
	if (staff.isCleaning())
	{
		XMFLOAT2 trollPos = staff.getPosition();
		Furniture* furtoclean = staff.getCleaningFurniture();
		XMINT2 targetPosition = { (int)furtoclean->getPosition().x , (int)furtoclean->getPosition().z };
		XMINT2 deltaPos = { (int)abs(trollPos.x - targetPosition.x),(int)abs(trollPos.y - targetPosition.y) };
		if (deltaPos.x < 2 && deltaPos.y < 2)
		{
			staff.cleanFurniture();
			return;
		}
		
		
		if (staff.walkQueueDone())
		{
			staff.setSpeed(3.0f);
			float xLol, yLol;
			staff.getStepsLeft(xLol, yLol);
			//xLol = yLol = 0.0f;
			staff.clearWalkingQueue();

			//Shape * obj = this->p_pickingEvent->top();
			
			trollPos.x += xLol;
			trollPos.y += yLol;

			int xTile = (int)trollPos.x;// (int)(round_n(trollPos.x, 1) - 0.5f);
			int yTile = (int)trollPos.y;//(int)(round_n(trollPos.y, 1) - 0.5f);



			XMINT2 startPosition = { xTile, yTile };

			auto path = GetPathAndSmokeGrass(startPosition, targetPosition);


			XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

			if (path.size() != 0)
			{
				if (!staff.getCancelFlag())
					staff.setCancelFlag(true);


				staff.Move(staff.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

				for (int i = 0; i < path.size() - 1; i++)
				{
					float lol = 255 * (float(i) / float(path.size()));
					path[i + 1]->tile->getQuad().setColor(0, 0, lol);
					staff.Move(staff.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
				}
			}
		}
		
	}

}

void AISolver::Update(Staff& staff, Action desiredAction)
{

}

RoomCtrl * AISolver::getRoomCtrl()
{
	return m_roomctrl; 
}

// Return 
// -1: if failed
//  0: if pending
//  1: if succedded
int AISolver::RequestPath(Character & character, RoomType targetRoom)
{

	XMFLOAT2 charPos = character.getPosition(); // (x,y) == (x,z,0)

	int xTile = (int)(round_n2(charPos.x, 1) - 0.5f);
	int yTile = (int)(round_n2(charPos.y, 1) - 0.5f);

	XMINT2 targetPosition = { (int)character.getPosition().x , (int)character.getPosition().y };

	if (targetRoom == RoomType::randomStupid)
		targetPosition = { (int)rand() % 32, (int)rand() % 32 };
	else if (targetRoom == RoomType::leave)
	{
		targetPosition = { 16,0 };
	}
	else
	{
		XMFLOAT3 xmtarg = m_roomctrl->getClosestRoom(XMFLOAT2(static_cast<float>(xTile), static_cast<float>(yTile)), targetRoom);
		// There wasnt any good rooms
		if (xmtarg.x == -1)
		{
			return -1; // HENRIK WAS HERE
		}
		targetPosition = { (int)xmtarg.x, (int)xmtarg.z };
	}

	return RequestPath(character, targetPosition);

}

int AISolver::RequestPath(Character & character, XMINT2 targetPosition)
{
	bool notPendingPath = false;
	for(int i = 0; i< futureObjects.size();i++)
	{
		auto t = futureObjects.at(i);
		if (t.index == character.getUniqueIndex())
		{
			notPendingPath = true;
			auto status = t.futObj->wait_for(0ms);
			if (status == std::future_status::ready)
			{
				
				XMFLOAT2 charPos = character.getPosition(); // (x,y) == (x,z,0)

				int xTile = (int)(round_n2(charPos.x, 1) - 0.5f);
				int yTile = (int)(round_n2(charPos.y, 1) - 0.5f);

				auto path = t.futObj->get();
				XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

				if (path.size() != 0)
				{
					character.Move(character.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

					for (int i = 0; i < path.size() - 1; i++)
						character.Move(character.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
				}
				
				delete futureObjects[i].futObj; 
				futureObjects.erase(futureObjects.begin() + i);
				return 1;
			}
		}
		
	}
	if (!notPendingPath)
	{
		if (character.walkQueueDone())
		{
			
			XMFLOAT2 charPos = character.getPosition(); // (x,y) == (x,z,0)

			int xTile = (int)(round_n2(charPos.x, 1) - 0.5f);
			int yTile = (int)(round_n2(charPos.y, 1) - 0.5f);

			XMINT2 startPosition = { xTile, yTile };
			std::future<std::vector<std::shared_ptr<Node>>>* future = new std::future<std::vector<std::shared_ptr<Node>>>();
			// Starts the pathfinding 
			*future = std::async(std::launch::async, &AISolver::GetPathAndSmokeGrass, this, startPosition, targetPosition);
		
			PathThread futureObj;
			futureObj.futObj = future;
			futureObj.index = character.getUniqueIndex();
			futureObjects.push_back(futureObj);
		}
	}

	return 0;
}