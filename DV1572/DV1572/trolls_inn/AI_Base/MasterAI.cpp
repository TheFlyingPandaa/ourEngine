#include "MasterAI.h"

void MasterAI::_sortVectorID(std::vector<int>& ID)
{
	int highestValueIndex;

	for (int i = 0; i < ID.size() - 1; i++)
	{
		highestValueIndex = i;
		for (int k = i + 1; k < ID.size(); k++)
		{
			if (ID[k] > ID[highestValueIndex])
			{
				highestValueIndex = k;
			}
		}
		this->_swap(i, highestValueIndex, ID);
	}

	for (int i = 0; i < ID.size(); i++)
		std::cout << ID[i] << std::endl;
	std::cout << std::endl;
}

void MasterAI::_swap(int index1, int index2, std::vector<int>& ID)
{
	int temp = ID[index1];
	ID[index1] = ID[index2];
	ID[index2] = temp;
}

MasterAI::MasterAI(RoomCtrl* roomCtrl, Grid* grid, Inn * inn)
	: m_solver(roomCtrl,grid)
{
	this->m_start = this->m_clock.now();
	m_inn = inn;
}

MasterAI::~MasterAI()
{
	for (auto& customer : m_customers)
		delete customer;
}

void MasterAI::Update(Camera* cam)
{
	// Get the elapsed time
	this->m_now = this->m_clock.now();
	this->m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(this->m_now - this->m_start);
	
	// Check if customer needs shall be updated
	bool updateCustomerNeeds = false;
	
	if (this->m_time_span.count() > 10)
	{
		updateCustomerNeeds = true;
		this->m_start = this->m_clock.now();
	}
	// Set spawn time limit (?)
	//this->customers.push_back(this->cFC.Update(this->inn.getInnAttributes()));
	// Not enough gold for wanted action = leave (?)

	// Loop through all customers
	std::vector<int> leavingCustomersIDs;
	int loopCounter = 0;

	// Iterate through customers
	// Evaluate what actions customers should take
	for (auto& customer : this->m_customers)
	{
		//solver.update(*customer);
		if (updateCustomerNeeds)
		{
			std::cout << "Customer Hungry: " << customer->GetHungry() << std::endl;
			std::cout << "Customer Tired: " << customer->GetTired() << std::endl;
			std::cout << "Customer Thirsty: " << customer->GetThirsty() << std::endl;
			std::cout << "Customer Gold: " << customer->GetEconomy().GetGold() << std::endl;
			customer->SetHungry(customer->GetHungry() + 1);
			customer->SetTired(customer->GetTired() + 1);
			customer->SetThirsty(customer->GetThirsty() + 1);
		}
		// Check if the customer is busy or not
		if (customer->GetQueueEmpty())
		{
			Action desiredAction;

			desiredAction = customer->GetAction();
			int price = 0;

			switch (desiredAction)
			{
			case EatAction:
				price = m_inn->GetFoodPrice();
				break;
			case DrinkAction:
				price = m_inn->GetDrinkPrice();
				break;
			case SleepAction:
				price = m_inn->GetSleepPrice();
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
				this->m_solver.Update(*customer, desiredAction, price);
			}
		}
		else
		{
			if (updateCustomerNeeds)
			{
				std::cout << "Customer Action: " << customer->GetStateStr() << std::endl << std::endl;
			}
			// Execute the action queue
			this->m_solver.Update(*customer, this->m_time_span);
		}

		loopCounter++;
	}

	if (this->m_solver.getTimeSpan().count() > 1)
		this->m_solver.restartClock();
	if (leavingCustomersIDs.size() > 0)
		this->_sortVectorID(leavingCustomersIDs);
	
	// BROKEN, subscript changes when first customer is deleted. Fixed(?)
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
			m_solver.GetPath(*leavingCustomer, randomStupid);
			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!This customer is now leaving!!!!!!!!!!!!!!!!!!!!\n";
			leavingCustomer->GotPathSetNextAction(LeavingInnAction);
		}
		else
		{
			// Send review to inn if customer reached end of path
			if (leavingCustomer->walkQueueDone())
				leavingCustomer->PopToNextState();
			if (leavingCustomer->GetState() == LeavingInn)
			{
				this->m_inn->CustomerReview(leavingCustomer->GetAttributes());
				// If customer sent review then delete the customer
				goneCustomers.push_back(loopCounter);
			}
		}
		loopCounter++;
	}
	// Delete customers that left the inn area
	if (goneCustomers.size() > 0)
		this->_sortVectorID(goneCustomers);
	// BROKEN, same as previous
	for (int i = 0; i < goneCustomers.size(); i++)
	{
		int index = goneCustomers[i];
		delete this->m_leavingCustomers[index];
		this->m_leavingCustomers.erase(this->m_leavingCustomers.begin() + index);
	}
}

void MasterAI::Draw()
{
	m_inn->Draw();
	for (auto& customer : m_customers)
		customer->Draw();
}

void MasterAI::spawn()
{
	m_customers.push_back(this->m_cFC.Update(this->m_inn->GetInnAttributes()));
}
