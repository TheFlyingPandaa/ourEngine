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

void MasterAI::_generateCustomer()
{
	m_nextCustomer = m_cFC.Update(m_inn->GetInnAttributes());
}

MasterAI::MasterAI(RoomCtrl* roomCtrl, Grid* grid, Inn * inn)
	: m_solver(roomCtrl,grid)
{
	m_customer_start = m_start = m_clock.now();
	m_customerSpawned = true;
	m_inn = inn;
	m_InnTroll = new Staff(); 
}

MasterAI::~MasterAI()
{
	for (auto& customer : m_customers)
		delete customer;
	delete m_InnTroll; 
}

void MasterAI::Update(Camera* cam)
{
	// Get the elapsed time
	m_customer_now = m_now = m_clock.now();
	m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_now - m_start);
	m_customer_spawn_timer = std::chrono::duration_cast<std::chrono::duration<double>>(m_customer_now - m_customer_start);

	// Check if customer needs shall be updated
	bool updateCustomerNeeds = false;
	
	if (!m_customerSpawned)
	{
		double duration = m_nextCustomer->GetTimeSpan().count();
		if (duration > CHECK_CUSTOMER_SPAWN)
		{
			if (m_nextCustomer->GetRace() == Elf)
			{
				if (duration > 30)
				{
					m_nextCustomer->RestartClock();
					m_customers.push_back(m_nextCustomer);
					m_customerSpawned = true;
					m_customer_start = m_clock.now();
				}
			}
			else
			{
				if (duration > 15)
				{
					m_nextCustomer->RestartClock();
					m_customers.push_back(m_nextCustomer);
					m_customerSpawned = true;
					m_customer_start = m_clock.now();
				}
			}
		}
	}
	else
	{
		if (m_customer_spawn_timer.count() > 30)
		{
			m_customerSpawned = false;
			_generateCustomer();
		}
	}

	if (this->m_time_span.count() > UPDATE_FREQUENCY_CUSTOMER_NEEDS)
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
			/*std::cout << "Customer Hungry: " << customer->GetHungry() << std::endl;
			std::cout << "Customer Tired: " << customer->GetTired() << std::endl;
			std::cout << "Customer Thirsty: " << customer->GetThirsty() << std::endl;
			std::cout << "Customer Gold: " << customer->GetEconomy().GetGold() << std::endl;*/
			customer->SetHungry(customer->GetHungry() + (1 * customer->GetHungryRate()));
			customer->SetTired(customer->GetTired() + (1 * customer->GetTiredRate()));
			customer->SetThirsty(customer->GetThirsty() + (1 * customer->GetThirstyRate()));
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
				this->m_solver.Update(*customer, desiredAction);
			}
		}
		else
		{
			/*if (updateCustomerNeeds)
			{
				std::cout << "Customer Action: " << customer->GetStateStr() << std::endl << std::endl;
			}*/
			// Execute the action queue
			m_solver.Update(*customer, m_inn);
		}

		loopCounter++;
	}

	if (this->m_solver.getTimeSpan().count() > UPDATE_FREQUENCY_EAT_DRINK_SLEEP_WAIT)
		this->m_solver.RestartClock();
	if (leavingCustomersIDs.size() > 0)
		this->_sortVectorID(leavingCustomersIDs);
	
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
			m_solver.GetPath(*leavingCustomer, leave);

			for (int i = 0; i < 3; ++i)
				leavingCustomer->Move(Character::WalkDirection::DOWN);
			for (int i = 0; i < 16; ++i)
				leavingCustomer->Move(Character::WalkDirection::RIGHT);

			leavingCustomer->GotPathSetNextAction(LeavingInnAction);
		}
		else
		{
			// Send review to inn if customer reached end of path
			if (leavingCustomer->walkQueueDone())
				leavingCustomer->PopToNextState();
			else
				this->m_solver.Update(*leavingCustomer, this->m_inn);
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
	for (auto& leavingCustomer : this->m_leavingCustomers)
		leavingCustomer->Draw();
	m_InnTroll->Draw(); 
}

void MasterAI::spawn()
{
	m_customers.push_back(this->m_cFC.Update(this->m_inn->GetInnAttributes()));
}
