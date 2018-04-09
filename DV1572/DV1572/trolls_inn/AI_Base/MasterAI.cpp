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
	Action desiredAction;
	std::vector<int> leavingCustomersIDs;
	int loopCounter = 0;

	// iterate through customers
	// calculate what actions customers should take
	for (auto customer : this->customers)
	{
		desiredAction = customer.getAction();
		int price = 0;

		if (desiredAction == EatAction)
			price = inn.getFoodPrice();
		else if (desiredAction == DrinkAction)
			price = inn.getDrinkPrice();
		else
			price = inn.getSleepPrice();

		if (customer.getEconomy().getGold() < price)
		{
			// Customer leaves inn
			// Save id for leaving customers
			leavingCustomersIDs.push_back(loopCounter);
		}
		else
		{
			// Customer wants path to Action area

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

	// iterate through leaving customers
	for (auto leavingCustomer : this->leavingCustomers)
	{
		// Customer wants path to exit

		// Send review to inn if customer reached end of path
		//this->inn.customerReview(leavingCustomer.getAttributes());

		// if customer sent review then delete

		loopCounter++;
	}
	
	
}