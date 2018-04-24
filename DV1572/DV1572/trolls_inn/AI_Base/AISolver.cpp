#include "AISolver.h"

AISolver::AISolver(Grid* grid)
{
	this->m_grid = grid;
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

void AISolver::Update(Customer& customer, std::chrono::duration<double> time_span)
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
			}
			customer.PopToNextState();
			break;
			// Update animations drink, eat, sleep (?)
		case Drinking:
			// Reduce how thirsty the customer is
			if (this->m_time_span.count() > 2)
			{
				if (customer.GetThirsty() > 0)
					customer.DoDrinking();
				else
					customer.PopToNextState();
			}
			break;
		case Eating:
			// Reduce how hungry the customer is
			if (this->m_time_span.count() > 2)
			{
				if (customer.GetHungry() > 0)
					customer.DoEating();
				else
					customer.PopToNextState();
			}
			break;
		case Sleeping:
			// Reduce how tired the customer is
			if (this->m_time_span.count() > 2)
			{
				if (customer.GetTired() > 0)
					customer.DoSleeping();
				else
					customer.PopToNextState();
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
		//getPath(customer, desiredAction);
		switch (desiredAction)
		{
		case DrinkAction:
			this->m_grid->generatePath(customer, RoomType::randomStupid);
			//this->m_grid->generatePath(customer, RoomType::kitchen);
			break;
		case EatAction:
			this->m_grid->generatePath(customer, RoomType::randomStupid);
			//this->m_grid->generatePath(customer, RoomType::kitchen);
			break;
		case SleepAction:
			this->m_grid->generatePath(customer, RoomType::randomStupid);
			//this->m_grid->generatePath(customer, RoomType::bedroom);
			break;
		}
		//roomCtrl need action and spots open for customers (?)
		customer.PopToNextState(); // pop Thinking state
		customer.PopToNextState(); // pop Idle state
		customer.GotPathSetNextAction(desiredAction);
		break;
	}
}

void AISolver::Update(Staff& staff)
{
}

void AISolver::Update(Staff& staff, Action desiredAction)
{

}