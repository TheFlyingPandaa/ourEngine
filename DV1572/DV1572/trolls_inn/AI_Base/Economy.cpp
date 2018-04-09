#include "Economy.h"

Economy::Economy()
{
	this->gold = 0;
}

Economy::Economy(int startGold)
{
	this->gold = startGold;
}

Economy::~Economy()
{
}

void Economy::deposit(int amount)
{
	this->gold += amount;
	this->transactions.push_back(amount);
	this->transactionsDocumentation.push_back("Deposited: ");
}

bool Economy::withdraw(int amount)
{
	bool enoughGold = false;

	if (this->gold >= amount)
	{
		enoughGold = true;
		this->gold -= amount;
		this->transactions.push_back(amount);
		this->transactionsDocumentation.push_back("Withdrew: ");
	}

	return enoughGold;
}

int Economy::getGold() const
{
	return this->gold;
}

std::vector<std::string> Economy::getTransactionHistory() const
{
	std::vector<std::string> completeTransactions;
	std::string mergeTransactions;

	for (int i = 0; i < this->transactions.size(); i++)
	{
		mergeTransactions = this->transactionsDocumentation[i];
		mergeTransactions += std::to_string(this->transactions[i]);
		completeTransactions.push_back(mergeTransactions);
	}

	return completeTransactions;
}
