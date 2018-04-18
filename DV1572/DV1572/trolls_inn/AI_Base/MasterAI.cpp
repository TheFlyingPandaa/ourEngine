#include "MasterAI.h"

MasterAI::MasterAI()
	: m_solver(m_inn.getGrid())
{
}

MasterAI::~MasterAI()
{
	for (int i = 0; i < this->m_customers.size(); i++)
		delete this->m_customers[i];
}

Grid * MasterAI::GetGrid()
{
	return m_inn.getGrid();
}

void MasterAI::Update(Camera* cam)
{
	m_inn.update(cam);
	// Set spawn time limit (?)
	//this->customers.push_back(this->cFL.update(this->inn.getInnAttributes()));
	// Not enough gold for wanted action = leave (?)

	// Loop through all customers
	std::vector<int> leavingCustomersIDs;
	int loopCounter = 0;

	// Iterate through customers
	// Evaluate what actions customers should take
	for (auto& customer : this->m_customers)
	{
		//solver.update(*customer);

		// Check if the customer is busy or not
		if (customer->GetQueueEmpty())
		{
			Action desiredAction;

			desiredAction = customer->GetAction();
			int price = 0;

			switch (desiredAction)
			{
			case EatAction:
				price = m_inn.getFoodPrice();
				break;
			case DrinkAction:
				price = m_inn.getDrinkPrice();
				break;
			case SleepAction:
				price = m_inn.getSleepPrice();
				break;
			}

			if (customer->GetEconomy().GetGold() < price)
			{
				// Customer leaves inn
				// Save id for leaving customers
				leavingCustomersIDs.push_back(loopCounter);
			}
			else
			{
				// Customer wants path to Action area
				customer->SetAction(ThinkingAction);
				this->m_solver.Update(*customer, desiredAction);
			}
		}
		else
		{
			// Execute the action queue
			this->m_solver.Update(*customer);
		}

		loopCounter++;
	}
	for (int i = 0; i < leavingCustomersIDs.size(); i++)
	{
		this->m_leavingCustomers.push_back(this->m_customers[leavingCustomersIDs[i]]);
		this->m_customers.erase(this->m_customers.begin() + leavingCustomersIDs[i]);
	}
	
	std::vector<int> goneCustomers;
	loopCounter = 0;

	// Iterate through leaving customers
	for (auto leavingCustomer : this->m_leavingCustomers)
	{
		if (leavingCustomer->GetQueueEmpty())
		{
			// Customer wants path to exit
			leavingCustomer->GotPathSetNextAction(LeavingInnAction);
		}
		else
		{
			// Send review to inn if customer reached end of path
			if (leavingCustomer->walkQueueDone())
				leavingCustomer->PopToNextState();
			if (leavingCustomer->GetState() == LeavingInn)
			{
				this->m_inn.customerReview(leavingCustomer->GetAttributes());
				// If customer sent review then delete the customer
				goneCustomers.push_back(loopCounter);
			}
		}
		loopCounter++;
	}
	// Delete customers that left the inn area
	for (int i = 0; i < goneCustomers.size(); i++)
	{
		int index = goneCustomers[i];
		delete this->m_leavingCustomers[index];
		this->m_leavingCustomers.erase(this->m_leavingCustomers.begin() + index);
	}
}

void MasterAI::Draw()
{
	m_inn.Draw();
	for (auto& customer : m_customers)
		customer->Draw();
}

void MasterAI::spawn()
{
	m_customers.push_back(this->m_cFC.update(this->m_inn.getInnAttributes()));
}
