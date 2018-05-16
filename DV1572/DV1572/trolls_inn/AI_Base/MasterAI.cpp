#include "MasterAI.h"
// round float to n decimals precision
float round_n3(float num, int dec)
{
	float m = (num < 0.0f) ? -1.0f : 1.0f;   // check if input is negative
	float pwr = pow(10.0f, dec);
	return float((float)floor((double)num * m * pwr + 0.5) / pwr) * m;
}
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

void MasterAI::_trollInnChase()
{
	if (m_InnTroll->getCancelFlag())
	{
		m_InnTroll->setCancelFlag(false);
		delete currentChase;
		currentChase = nullptr;
		return;
	}

	m_InnTroll->setSpeed(4.0f);
	static float lastPathCounter = 0;
	// Grab the path if it is done
	if (currentChase->pathReturn == 0) 
		currentChase->pathReturn = m_solver.RequestPath(*m_InnTroll, XMINT2(static_cast<int32_t>(currentChase->customerpath.x), static_cast<int32_t>(currentChase->customerpath.y)));

	// Now when we have the path we need to check if its valid
	if (currentChase->pathReturn == 1)
	{
		lastPathCounter++;
		XMFLOAT2 trollPos(round_n3(m_InnTroll->getPosition().x,0), round_n3(m_InnTroll->getPosition().y, 0));
		XMFLOAT2 custPos = m_customers[currentChase->charIndex]->getPosition();
		XMFLOAT2 deltaPos = XMFLOAT2(abs(custPos.x - trollPos.x), abs(custPos.y - trollPos.y));

		if ((deltaPos.x > 1.0f || deltaPos.y > 1.0f ))
		{
			if (lastPathCounter >= 25)
			{
				lastPathCounter = 0;
				m_InnTroll->clearWalkingQueue();

				currentChase->pathReturn = m_solver.RequestPath(*m_InnTroll, XMINT2(static_cast<int32_t>(currentChase->customerpath.x), static_cast<int32_t>(currentChase->customerpath.y)));
				currentChase->customerpath = custPos;
			}
			
		}
		else
		{
			std::cout << "Reached the customer!\n";
			m_InnTroll->setSpeed(2.0f);
			delete currentChase;
			currentChase = nullptr;
		}

	}

	
	
}

MasterAI::MasterAI(RoomCtrl* roomCtrl, Grid* grid, Inn * inn)
	: m_solver(roomCtrl,grid)
{
	m_customer_start = m_start = m_clock.now();
	m_customerSpawned = true;
	m_inn = inn;
	m_InnTroll = new Staff(); 
	m_showMenu = false;
	m_customerMenu = new ClickMenu(ClickMenu::MTYPE::CHA);
	currentChase = nullptr;
}

MasterAI::~MasterAI()
{
	for (auto& customer : m_customers)
	{
		delete customer;
		customer = nullptr;
	}

	if (m_nextCustomer != nullptr)
		delete m_nextCustomer;

	if (m_leavingCustomers.size() > 0)
		for (auto& customer : m_leavingCustomers)
			delete customer;
	delete m_InnTroll; 
	delete m_customerMenu;
}
Staff * MasterAI::getTroll()
{
	return m_InnTroll; 
}

void MasterAI::CharacterMenu()
{
	if (Input::isMouseLeftPressed(false))
	{
		if (m_customerMenu->ButtonClicked() == 1)
		{
			if (currentChase)
			{
				delete currentChase;
				currentChase = nullptr;
			}
			m_InnTroll->clearWalkingQueue();
			currentChase = new TROLL_CHASE;
			currentChase->customerpath = XMFLOAT2(round_n3(m_customers[m_selectedCustomer]->getPosition().x, 0), round_n3(m_customers[m_selectedCustomer]->getPosition().y, 0));
			currentChase->charIndex = m_selectedCustomer;

			currentChase->pathReturn = m_solver.RequestPath(*m_InnTroll, XMINT2(currentChase->customerpath.x, currentChase->customerpath.y));

		}
		else if (m_customerMenu->ButtonClicked() == -2)
		{
			m_showMenu = false;

			m_customers[m_selectedCustomer]->getShape()->setColor(1, 1, 1);
			m_selectedCustomer = -1;
		
		}
		
	}
	
}

void MasterAI::Update(Camera* cam)
{

	if (m_showMenu)
		CharacterMenu();

	// If there is a chase right now
	if(currentChase)
		_trollInnChase();

	m_solver.Update(*m_InnTroll); 


	// Get the elapsed time
	m_customer_now = m_now = m_clock.now();
	m_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_now - m_start);
	m_customer_spawn_timer = std::chrono::duration_cast<std::chrono::duration<double>>(m_customer_now - m_customer_start);

	// Check if customer needs shall be updated
	bool updateCustomerNeeds = false;
	
	// Generate new customer if the stars align
	if (!m_customerSpawned)
	{
		double duration = m_nextCustomer->GetTimeSpan().count();
		if (duration > CHECK_CUSTOMER_SPAWN)
		{
			if (m_nextCustomer->GetRace() == Elf)
			{
				if (duration > 30)
				{
					std::stringstream ss;
					ss << "An " << m_nextCustomer->GetRaceStr() << " has arrived!" << std::endl;
					std::cout << "An " << m_nextCustomer->GetRaceStr() << " has arrived!" << std::endl;
					InGameConsole::pushString(ss.str());
					m_nextCustomer->RestartClock();
					m_customers.push_back(m_nextCustomer);
					m_nextCustomer = nullptr;
					m_customerSpawned = true;
					m_customer_start = m_clock.now();
				}
			}
			else
			{
				if (duration > 15)
				{
					std::stringstream ss;
					ss << "A " << m_nextCustomer->GetRaceStr() << " has arrived!" << std::endl;
					std::cout << "A " << m_nextCustomer->GetRaceStr() << " has arrived!" << std::endl;
					InGameConsole::pushString(ss.str());
					m_nextCustomer->RestartClock();
					m_customers.push_back(m_nextCustomer);
					m_nextCustomer = nullptr;
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

			if (customer->GetEconomy().GetGold() < price || customer->GetThought() == Character::ANGRY)
			{
				// Customer leaves inn
				// Save id for leaving customers
				leavingCustomersIDs.push_back(loopCounter);
				if (customer->GetThought() == Character::ANGRY)
				{
					m_inn->AddAngryCustomer();
				}
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
		if (currentChase)
		{
			if (currentChase->charIndex == leavingCustomersIDs[i])
			{
				InGameConsole::pushString("You can't kill an\nleaving customer!");
				delete currentChase;
				currentChase = nullptr;
				// Reset the color
				m_customers[leavingCustomersIDs[i]]->getShape()->setColor(1, 1, 1);
			}

		}
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
			int result = m_solver.RequestPath(*leavingCustomer, leave);
			if (result == 1)
			{
				RandomNumberGenerator gen;
				int depth = gen.GenerateRandomNumber(1,5);
				for (int i = 0; i < depth; ++i)
					leavingCustomer->Move(Character::WalkDirection::DOWN);
				for (int i = 0; i < 16; ++i)
					leavingCustomer->Move(Character::WalkDirection::RIGHT);
				leavingCustomer->GotPathSetNextAction(LeavingInnAction, m_solver.getRoomCtrl());

			}

			
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

void MasterAI::PickCustomers()
{
	for (auto& cust : m_customers)
	{
		cust->CheckForPicking();
	}
}

void MasterAI::PickedCustomerShape(Shape * shape)
{
	XMFLOAT3 position = shape->getPosition();
	static int lastSelectedCustomer = -1;
	if (currentChase)
	{
		delete currentChase;
		currentChase = nullptr;
	}
	
	for (int i = 0; i < m_customers.size(); i++)
	{

		XMFLOAT2 customerPos = m_customers[i]->getPosition();
		XMFLOAT2 deltaPos(abs(customerPos.x - position.x), abs(customerPos.y - position.z));

		// INSIDE HERE DO WE HAVE A CLICK WITH m_customers[i]
		if (deltaPos.x < 0.1 && deltaPos.y < 0.1)
		{
			m_showMenu = true;
			if (lastSelectedCustomer == -1)
			{
				lastSelectedCustomer = i;
				m_selectedCustomer = i;
				m_selectedCustomerID = m_customers[i]->getUniqueIndex();
				
				m_customers[i]->getShape()->setColor(0, 1, 0);
			}
			else
			{
				if (m_customers[lastSelectedCustomer]->getUniqueIndex() != m_selectedCustomerID)
				{
					
				}
				else
				{
					m_customers[lastSelectedCustomer]->getShape()->setColor(1, 1, 1);
					lastSelectedCustomer = i;
					m_selectedCustomer = i;
					m_selectedCustomerID = m_customers[i]->getUniqueIndex();

					m_customers[i]->getShape()->setColor(0, 1, 0);
				}
				
			}

			m_customerMenu->setInfo(m_customers[i]->getInfoText());
		
			XMINT2 winSize = Input::getWindowSize();
			m_customerMenu->setPos(XMFLOAT2((winSize.x / 2.0f) - 200.0f, winSize.y *0.01f));
			

			


		}

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

	if (m_showMenu)
		m_customerMenu->Draw();
}

void MasterAI::spawn()
{
	m_customers.push_back(this->m_cFC.Update(this->m_inn->GetInnAttributes()));
}
