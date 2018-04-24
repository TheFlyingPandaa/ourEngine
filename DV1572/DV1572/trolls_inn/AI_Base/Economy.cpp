#include "Economy.h"

Economy::Economy()
{
	this->m_gold = 0;
}

Economy::Economy(int startGold)
{
	this->m_gold = startGold;
}

Economy::~Economy()
{
}

void Economy::Deposit(int amount)
{
	this->m_gold += amount;
	this->m_transactions.push_back(amount);
	this->m_transactionsDocumentation.push_back("Deposited: ");
}

bool Economy::Withdraw(int amount, bool hardWithdraw)
{
	if (this->m_gold > amount || hardWithdraw) {
		this->m_gold -= amount;
		this->m_transactions.push_back(amount);
		this->m_transactionsDocumentation.push_back("Withdrew: ");
		return true;
	}
	else
		return false;
}

int Economy::GetGold() const
{
	return this->m_gold;
}

std::vector<std::string> Economy::GetTransactionHistory() const
{
	std::vector<std::string> completeTransactions;
	std::string mergeTransactions;

	for (int i = 0; i < this->m_transactions.size(); i++)
	{
		mergeTransactions = this->m_transactionsDocumentation[i];
		mergeTransactions += std::to_string(this->m_transactions[i]);
		completeTransactions.push_back(mergeTransactions);
	}

	return completeTransactions;
}
