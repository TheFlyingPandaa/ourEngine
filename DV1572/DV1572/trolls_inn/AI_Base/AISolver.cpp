#include "AISolver.h"
#include "RandomNumberGenerator.h"

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

void AISolver::update(Staff& staff, std::vector<Customer>& customers, StaffController& staffController)
{
	if(staff.getActionQueue().front() == Unassigned)
	{
		if (staff.getDuration().count() >= 6)
		{
			staff.getActionQueue().pop();
			staff.resetClock();  
			staffController.giveNewAction(&staff); 
		}
		//Add idle logic here 
		std::cout << "I am unassigned!" << std::endl;
	}
	else if (staff.getActionQueue().front() == Chef)
	{
		if (staff.getDuration().count() >= 10)
		{
			staff.getActionQueue().pop();
			staff.resetClock(); 
		}
		//Add cooking logic here 
		std::cout << "I am cooking" << std::endl;
	}
	else if (staff.getActionQueue().front() == Assasin)
	{
		if (staff.getTaskCompleted())
		{
			staff.getActionQueue().pop();
			staff.resetClock();
		}
		std::cout << "I am an assasin!" << std::endl;

		//Add killing logic here. 
		RandomNumberGenerator rndNumberGen;
		std::vector<Customer*> customerOfChoice;
		for (int i = 0; i < customers.size(); i++)
		{
			if (customers[i].getEconomy().getGold() > 200)
			{
				customerOfChoice.push_back(&customers[i]); 
			}
		}
		int rndNum = rndNumberGen.generateRandomNumber(0, customers.size()); 
		Customer* customerPtr = customerOfChoice[rndNum];


	}
	else if (staff.getActionQueue().front() == Maid)
	{
		if (!staff.getTaskCompleted())
		{
			staff.getActionQueue().pop(); 
			staff.resetClock(); 
		}
		//Add cleaning logic 
		std::cout << "I am a maid!" << std::endl;
	}
	else if (staff.getActionQueue().front() == Guard)
	{
		std::cout << "I am, a guard!" << std::endl; 
	}
	else if (staff.getActionQueue().front() == Theif)
	{
		std::cout << "I am a Theif!" << std::endl; 
	}
	else
	{
		std::cout << "I am a bartender!" << std::endl; 
	}
}

void AISolver::update(Staff & staff)
{
}


