#include "AISolver.h"
#include "Inn.h"
AISolver::AISolver(Grid* grid)
{
	m_grid = grid;
}

AISolver::~AISolver()
{
	
}

void AISolver::Update(Customer& customer)
{
	CustomerState currentState = customer.getState();
	customer.Update();
	if (currentState == WalkingToInn)
	{
		if (customer.walkQueueDone())
		{
			m_grid->generatePath(customer, reception);
			customer.popToNextState();
			currentState = customer.getState();
		}
	}

	if (currentState == Walking)
	{
		if (customer.walkQueueDone())
		{
			customer.PopToNextState();
			currentState = customer.GetState();
		}
	}
	if (currentState != Walking)
	{
		switch (currentState)
		{
		case Idle:
			// Calculate new desire
			// Do customer want to walk around or take an action
			if (customer.GetHungry() < 5 && customer.GetThirsty() < 5 && customer.GetTired() < 5)
			{
				// Get a path to a new location
				// Walk towards a room with the highest value (?) (Tired, Hungry or Thirsty)
				// Go explore (?)
				// Get race desires (?)
			}
			break;
			// Update animations drink, eat, sleep (?)
		case Drinking:
			// Reduce how thirsty the customer is
			// Base this on time somehow
			if (customer.GetThirsty() > 0)
				customer.DoDrinking();
			else
				customer.PopToNextState();
			break;
		case Eating:
			// Reduce how hungry the customer is
			// Base this on time somehow
			if (customer.GetHungry() > 0)
				customer.DoEating();
			else
				customer.PopToNextState();
			break;
		case Sleeping:
			// Reduce how tired the customer is
			// Base this on time somehow
			if (customer.GetTired() > 0)
				customer.DoSleeping();
			else
				customer.PopToNextState();
			break;
		}
	}
}

void AISolver::Update(Customer& customer, Action desiredAction)
{
	CustomerState currentState = customer.GetState();

	switch (currentState)
	{
	case Thinking:
		//getPath(customer, desiredAction);
		//roomCtrl need action and spots open for customers (?)
		customer.PopToNextState(); // pop Thinking state
		customer.PopToNextState(); // pop Idle state
		customer.GotPathSetNextAction(desiredAction);
		break;
	}
}

void AISolver::Update(Staff& staff)
{
}

void AISolver::Update(Staff& staff, Action desiredAction)
{

}

bool AISolver::_findPath()
{
	bool result = false;
	//if (c.walkQueueDone() && m_move)
	//{
	//	//Shape * obj = this->p_pickingEvent->top();
	//	XMFLOAT2 charPos = c.getPosition(); // (x,y) == (x,z,0)

	//	int xTile = (int)(round_n(charPos.x, 1) - 0.5f);
	//	int yTile = (int)(round_n(charPos.y, 1) - 0.5f);

	//	std::vector<std::shared_ptr<Node>> path = grid->findPathHighLevel(grid->getTile(xTile, yTile), grid->getTile((int)obj->getPosition().x, (int)obj->getPosition().z));

	//	XMFLOAT3 oldPos = { float(xTile),0.0f, float(yTile) };

	//	if (path.size() != 0)
	//	{
	//		result = true;
	//		m_justMoved = false;

	//		c.Move(c.getDirectionFromPoint(oldPos, path[0]->tile->getQuad().getPosition()));

	//		for (int i = 0; i < path.size() - 1; i++)
	//			c.Move(c.getDirectionFromPoint(path[i]->tile->getQuad().getPosition(), path[i + 1]->tile->getQuad().getPosition()));
	//	}

	//}

	return result;
}
