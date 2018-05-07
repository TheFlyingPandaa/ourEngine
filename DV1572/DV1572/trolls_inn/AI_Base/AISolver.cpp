﻿#include "AISolver.h"

void AISolver::_checkSpotInRoom(Inn& inn, Customer& customer)
{
	//customer.SetMovingTowardsActionArea(false);
	// If spot is available in the room at customer position
	//std::vector<Furniture> furnituresInRoom = this->m_roomctrl->getNoneBusyFurnitureInRoom(customer.getPosition());
	
	if (true)
	{
		// Get the seat/bed ID to lock it to the customer
		
		// Do quick maff
		int price = inn.GetDrinkPrice();
		customer.GetEconomy().Withdraw(price);
		inn.Deposit(price);
		customer.SetAvailableSpotFound(true);
	}
	else
	{
		// Get a path to a new room of the same type (?)
		/*CustomerState customerState = customer.GetState();

		switch (customerState)
		{
		case Drinking:

			break;
		case Eating:
			
			break;
		case Sleeping:
			
			break;
		}*/
		customer.GetAttributes().SetReputation(-1);
		customer.SetAvailableSpotFound(false);
		customer.SetWaitingForSpot(true);
	}
}

void AISolver::_doWaiting(Customer& customer)
{
	// Check if a spot is available

	if ((this->m_rNG.GenerateRandomNumber(1, 5) * customer.GetWaitingForSpotMultiplier()) > WAITING_FOR_SPOT_TIME_LIMIT)
	{
		customer.GetAttributes().SetReputation(-5);
		customer.SetWaitingForSpot(false);
		// Do angry face emote
		// Leave inn (?)
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

		if (0 == m_roomctrl->getRoomConnections(index))
			return std::vector<std::shared_ptr<Node>>();

		RoomCtrl::DoorPassage entranceDoor = m_roomctrl->getClosestEntranceDoor(startPosition);
		Room* entranceRoom = m_roomctrl->getRoomAt(entranceDoor.roomIndexes[1]);
		// Now do we wanna walk to the entrance
		path = m_grid->findPath(startTile, m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y));

		//Lets not talk about this one(This is so we walk straight through the door...)
		std::vector<std::shared_ptr<Node>> lol1;
		std::shared_ptr<Node> current(new Node(entranceRoom->getTile(entranceDoor.two.x, entranceDoor.two.y), nullptr, 0, 0));
		lol1.push_back(current);
		path.insert(path.end(), lol1.begin(), lol1.end());

		// Easy check, if its the main room we are wanting then we only need to perform pathfinding inside that room
		// and not start the higher level room path finding
		if (*targetRoom == *entranceRoom)
		{
			auto doorToEndTile = targetRoom->findPath(targetRoom->getTile(entranceDoor.two.x, entranceDoor.two.y), targetRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		// We want to be advanced :P
		else
		{

			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(entranceRoom->getTile(entranceDoor.two.x, entranceDoor.two.y), targetRoom->getTile(pos.x, pos.y));

			Room* startRoom = entranceRoom;
			XMINT2 DoorLeavePos;
			XMINT2 startPosition = entranceDoor.two;
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
			return std::vector<std::shared_ptr<Node>>();

		if (*targetRoom == *startRoom)
		{
			auto doorToEndTile = targetRoom->findPath(targetRoom->getTile(tPos.x, tPos.y), targetRoom->getTile(pos.x, pos.y));
			path.insert(path.end(), doorToEndTile.begin(), doorToEndTile.end());
		}
		else
		{
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(startRoom->getTile(tPos.x, tPos.y), targetRoom->getTile(pos.x, pos.y));

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

		RoomCtrl::DoorPassage entranceDoor = m_roomctrl->getClosestEntranceDoor(startPosition);

		XMINT2 tPos = targetPosition;
		int index2 = m_roomctrl->_intersect(entranceDoor.two);
		Room* entranceRoom = m_roomctrl->getRoomAt(index2);

		if (*entranceRoom == *targetRoom)
		{

			XMINT2 pos = startPosition;
			int index = m_roomctrl->_intersect(pos);
			Room* targetRoom = m_roomctrl->getRoomAt(index);

			if (0 == m_roomctrl->getRoomConnections(index))
				return std::vector<std::shared_ptr<Node>>();


			// Path to door
			auto toDoor = targetRoom->findPath(targetRoom->getTile(startPosition.x, startPosition.y), targetRoom->getTile(entranceDoor.two.x, entranceDoor.two.y));
			path.insert(path.end(), toDoor.begin(), toDoor.end());

			//Lets not talk about this one(This is so we walk straight through the door...)
			auto walkThroughDoor = m_grid->findPath(targetRoom->getTile(entranceDoor.two.x, entranceDoor.two.y), m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y));
			path.insert(path.end(), walkThroughDoor.begin(), walkThroughDoor.end());

			auto walkToTarget = m_grid->findPath(m_grid->getTile(entranceDoor.one.x, entranceDoor.one.y), targetTile);
			path.insert(path.end(), walkToTarget.begin(), walkToTarget.end());
		}
		else
		{
			// We need to find the entrace from inside then we path to the destination
			std::vector<int> roomIndexes = m_roomctrl->roomTraversal(targetRoom->getTile(startPosition.x, startPosition.y), entranceRoom->getTile(entranceDoor.two.x, entranceDoor.two.y));

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

void AISolver::restartClock()
{
	this->m_start = this->m_clock.now();
}

void AISolver::Update(Customer& customer, Inn& inn)
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
				// Walk along the catwalk then upwards towards the gridsystem where the rooms are located
				for (int i = 0; i < 16; ++i)
					customer.Move(Character::WalkDirection::RIGHT);
				for (int i = 0; i < 3; ++i)
					customer.Move(Character::WalkDirection::UP);
			}
			else
			{
				customer.PopToNextState();
				currentState = customer.GetState();
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
				// Get a path to a new location
				// Walk towards a room with the highest value (?) (Tired, Hungry or Thirsty)
				// Go explore (?)
				// Get race desires (?)
				GetPath(customer, RoomType::randomStupid);
				customer.SetAction(WalkAction);
			}
			customer.PopToNextState();
			break;
			// Update animations drink, eat, sleep (?)
		case Drinking:
			if (customer.GetAvailableSpotFound())
			{
				// Reduce how thirsty the customer is
				if (this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
				{
					if (customer.GetThirsty() > 0)
						customer.DoDrinking();
					else
						customer.PopToNextState();
				}
			}
			else if (customer.GetWaitingForSpot() && this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
			{
				this->_doWaiting(customer);
			}
			else
			{
				// Check if there is an open spot
				this->_checkSpotInRoom(inn, customer);
			}
			break;
		case Eating:
			if (customer.GetAvailableSpotFound())
			{
				// Reduce how hungry the customer is
				if (this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
				{
					if (customer.GetHungry() > 0)
						customer.DoEating();
					else
						customer.PopToNextState();
				}
			}
			else if (customer.GetWaitingForSpot() && this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
			{
				// Check if there is a spot open this time
				this->_doWaiting(customer);
			}
			else
			{
				// Check if there is an open spot
				this->_checkSpotInRoom(inn, customer);
			}
			break;
		case Sleeping:
			if (customer.GetAvailableSpotFound())
			{
				// Reduce how tired the customer is
				if (this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
				{
					if (customer.GetTired() > 0)
						customer.DoSleeping();
					else
						customer.PopToNextState();
				}
			}
			else if (customer.GetWaitingForSpot() && this->m_time_span.count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
			{
				// Check if there is a spot open this time
				this->_doWaiting(customer);
			}
			else
			{
				// Check if there is an open spot
				this->_checkSpotInRoom(inn, customer);
			}
			break;
		}
	}
}

void AISolver::Update(Customer& customer, Action desiredAction)
{
	CustomerState currentState = customer.GetState();

	switch (currentState)
	{
	case Thinking:
		switch (desiredAction)
		{
		case DrinkAction:
			GetPath(customer, RoomType::randomStupid);
			//GetPath(customer, RoomType::bar);
			break;
		case EatAction:
			//GetPath(customer, RoomType::randomStupid);
			GetPath(customer, RoomType::kitchen);
			break;
		case SleepAction:
			//GetPath(customer, RoomType::randomStupid);
			GetPath(customer, RoomType::bedroom);
			break;
		}
		customer.PopToNextState(); // pop Thinking state
		customer.PopToNextState(); // pop Idle state
		customer.GotPathSetNextAction(desiredAction);
		customer.SetAvailableSpotFound(false);
		customer.SetWaitingForSpot(false);
		customer.SetWaitingForSpotMultiplier(1);
		break;
	}
}

void AISolver::Update(Staff& staff)
{
}

void AISolver::Update(Staff& staff, Action desiredAction)
{

}
// round float to n decimals precision
float round_n2(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
void AISolver::GetPath(Character & character, RoomType targetRoom)
{
	if (character.walkQueueDone())
	{
		//Shape * obj = this->p_pickingEvent->top();
		XMFLOAT2 charPos = character.getPosition(); // (x,y) == (x,z,0)

		int xTile = (int)(round_n2(charPos.x, 1) - 0.5f);
		int yTile = (int)(round_n2(charPos.y, 1) - 0.5f);
		
		XMINT2 targetPosition = { (int)character.getPosition().x , (int)character.getPosition().y };

		if (targetRoom == RoomType::randomStupid)
			targetPosition = { (int)rand() % 32, (int)rand() % 32 };
		else
		{
			XMFLOAT3 xmtarg = m_roomctrl->getClosestRoom(XMFLOAT2(xTile, yTile), targetRoom);
			if (xmtarg.x == -1)// || xmtarg.y == -1)
			{
				xmtarg.x = this->m_rNG.GenerateRandomNumber(0, 31);
				xmtarg.z = this->m_rNG.GenerateRandomNumber(0, 31);
			}
			targetPosition = { (int)xmtarg.x, (int)xmtarg.z };
		}
		

		XMINT2 startPosition = { xTile, yTile };

		auto path = GetPathAndSmokeGrass(startPosition, targetPosition);


		XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

		if (path.size() != 0)
		{
			character.Move(character.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

			for (int i = 0; i < path.size() - 1; i++)
				character.Move(character.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
		}

	}
}
