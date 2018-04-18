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
				// Walk towards a room with the highest value (?) Tired, Hungry, Thirsty
				// Go explore (?)
			}
			break;
			// Update animations drink, eat, sleep(?)
		case Drinking:
			// Get drink
			if (customer.getThirsty() > 0)
				customer.drinking();
			break;
		case Eating:
			// Get food
			if (customer.getHungry() > 0)
				customer.eating();
			break;
		case Sleeping:
			// Get a room
			if (customer.getTired() > 0)
				customer.sleeping();
			break;
		case LeavingInn:
			// Send review
			
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
		customer.popToNextState(); // pop Thinking state
		customer.popToNextState(); // pop Idle state
		//getPath(customer, desiredAction);
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