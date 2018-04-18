#include "AISolver.h"

AISolver::AISolver()
{
}

AISolver::AISolver(Grid* grid)
{
	this->map = grid;
}

AISolver::~AISolver()
{
	delete map;
}

void AISolver::update(Customer& customer)
{
	CustomerState currentState = customer.getState();

	if (currentState == Walking)
	{
		if (customer.walkQueueDone())
		{
			customer.popToNextState();
			currentState = customer.getState();
		}
	}
	if (currentState != Walking)
	{
		switch (currentState)
		{
		case Idle:
			// Calculate new desire
			// Do customer want to walk around or take an action
			if (customer.getHungry() < 5 && customer.getThirsty() < 5 && customer.getTired() < 5)
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
			if (customer.getThirsty() > 0)
				customer.drinking();
			else
				customer.popToNextState();
			break;
		case Eating:
			// Reduce how hungry the customer is
			// Base this on time somehow
			if (customer.getHungry() > 0)
				customer.eating();
			else
				customer.popToNextState();
			break;
		case Sleeping:
			// Reduce how tired the customer is
			// Base this on time somehow
			if (customer.getTired() > 0)
				customer.sleeping();
			else
				customer.popToNextState();
			break;
		}
	}
}

void AISolver::update(Customer& customer, Action desiredAction)
{
	CustomerState currentState = customer.getState();

	switch (currentState)
	{
	case Thinking:
		//getPath(customer, desiredAction);
		//roomCtrl need action and spots open for customers (?)
		customer.popToNextState(); // pop Thinking state
		customer.popToNextState(); // pop Idle state
		customer.gotPathSetNextAction(desiredAction);
		break;
	}
}

void AISolver::update(Staff& staff)
{
}

void AISolver::update(Staff& staff, Action desiredAction)
{

}