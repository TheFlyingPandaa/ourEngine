#include "MasterAI.h"

MasterAI::MasterAI()
{
	m_staffController = StaffController(); 
}

MasterAI::~MasterAI()
{
}

void MasterAI::update()
{
	// Set spawn time limit (?)
	this->customers.push_back(this->cFL.update(this->inn.getInnAttributes()));
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
				// Save id for leaving customers
				leavingCustomersIDs.push_back(loopCounter);
			}
			else
			{
				// Customer wants path to Action area
				customer.setAction(ThinkingAction);
				this->solver.update(customer, desiredAction);
			}
		}
		else
		{
			// Execute the action queue
			this->solver.update(customer);
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
		if (leavingCustomer.getQueueEmpty())
		{
			// Customer wants path to exit
			leavingCustomer.gotPathSetNextAction(LeavingInnAction);
		}
		else
		{
			// Send review to inn if customer reached end of path
			if (leavingCustomer.walkQueueDone())
				leavingCustomer.popToNextState();
			if (leavingCustomer.getState() == LeavingInn)
			{
				this->inn.customerReview(leavingCustomer.getAttributes());
				// If customer sent review then delete the customer
				goneCustomers.push_back(loopCounter);
			}
		}
		loopCounter++;
	}
	// Delete customers that left the inn area
	for (int i = 0; i < goneCustomers.size(); i++)
		leavingCustomers.erase(this->leavingCustomers.begin() + goneCustomers[i]);

	//Update staff members
	for (auto& currentStaffMember : m_staffController.getStaffMembers())
	{
		solver.update(*currentStaffMember, customers, m_staffController);
	}
}