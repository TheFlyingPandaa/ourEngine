#include "MasterAI.h"

MasterAI::MasterAI()
{
}

MasterAI::~MasterAI()
{
}

void MasterAI::update()
{
	// Not enough gold for wanted action = leave (?)

	// Loop through all customers
	std::vector<int> leavingCustomersIDs;
	int loopCounter = 0;

	// Iterate through customers
	// Evaluate what actions customers should take
	for (auto customer : this->customers)
	{
		// Check if the customer is busy or not
		if (customer.getQueueEmpty())
		{
			Action desiredAction;

			desiredAction = customer.getAction();
			int price = 0;

			switch (desiredAction)
			{
			case EatAction:
				price = inn.getFoodPrice();
				break;
			case DrinkAction:
				price = inn.getDrinkPrice();
				break;
			case SleepAction:
				price = inn.getSleepPrice();
				break;
			}

			if (customer.getEconomy().getGold() < price)
			{
				// Customer leaves inn
				customer.setAction(LeavingInnAction);
				// Save id for leaving customers
				leavingCustomersIDs.push_back(loopCounter);
			}
			else
			{
				// Customer wants path to Action area
			}
		}
		else
		{
			// Execute the action queue
			CustomerState currentState = customer.getState();

			switch (currentState)
			{
			case Idle:

				break;
			case Thinking:

				break;
			case Walking:

				break;
			case Drinking:

				break;
			case Eating:

				break;
			case Sleeping:

				break;
			case LeavingInn:

				break;
			}

		}

		loopCounter++;
	}
	for (int i = 0; i < leavingCustomersIDs.size(); i++)
	{
		this->leavingCustomers.push_back(this->customers[leavingCustomersIDs[i]]);
		this->customers.erase(this->customers.begin() + leavingCustomersIDs[i]);
	}
	
	std::vector<int> goneCustomers;
	loopCounter = 0;

	// Iterate through leaving customers
	for (auto leavingCustomer : this->leavingCustomers)
	{
		// Customer wants path to exit

		// Send review to inn if customer reached end of path
		//this->inn.customerReview(leavingCustomer.getAttributes());

		// If customer sent review then delete

		loopCounter++;
	}
	
	
}