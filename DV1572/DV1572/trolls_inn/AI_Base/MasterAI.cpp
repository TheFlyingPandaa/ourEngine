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
			// Bool or new array with leaving customers?
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
	loopCounter = 0;
	
	for (auto leavingCustomer : this->leavingCustomers)
	{
		// Customer wants path to exit

		// Send review to inn
		if (true) // if customer reached end of path
			this->inn.customerReview(leavingCustomer.getAttributes());
	}
	
	
}