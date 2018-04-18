#include "AISolver.h"
#include "Inn.h"
AISolver::AISolver(Grid* grid)
{
	m_grid = grid;
}

AISolver::~AISolver()
{
	
}

void AISolver::Update(Customer& customer)
{
	CustomerState currentState = customer.GetState();
	customer.Update();
	if (currentState == WalkingToInn)
	{
		if (customer.walkQueueDone())
		{
			m_grid->generatePath(customer, reception);
			customer.PopToNextState();
			currentState = customer.GetState();
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
			break;
			// Update animations drink, eat, sleep (?)
		case Drinking:
			// Reduce how thirsty the customer is
			// Base this on time somehow
			if (customer.GetThirsty() > 0)
				customer.DoDrinking();
			else
				customer.PopToNextState();
			break;
		case Eating:
			// Reduce how hungry the customer is
			// Base this on time somehow
			if (customer.GetHungry() > 0)
				customer.DoEating();
			else
				customer.PopToNextState();
			break;
		case Sleeping:
			// Reduce how tired the customer is
			// Base this on time somehow
			if (customer.GetTired() > 0)
				customer.DoSleeping();
			else
				customer.PopToNextState();
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