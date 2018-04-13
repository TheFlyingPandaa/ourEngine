#include "AISolver.h"

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
			if (customer.getHungry() < 4 && customer.getThirsty() < 4 && customer.getTired() < 4)
			{
				// Get a path to a new location
				// Walk towards a room with the highest value (?) Tired, Hungry, Thirsty

			}
			break;
		case Drinking:
			// Get drink

			break;
		case Eating:
			// Get food

			break;
		case Sleeping:
			// Get a room

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

void AISolver::update(Staff& staff, Action desiredAction)
{

}