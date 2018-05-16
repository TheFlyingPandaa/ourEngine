#include "OurEngine.h"
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
	
	if (m_transactionsDocumentation.size() > 50)
	{
		m_transactions.erase(m_transactions.begin(), m_transactions.begin() + 30);
		m_transactionsDocumentation.erase(m_transactionsDocumentation.begin() + 0, m_transactionsDocumentation.begin() + 30);
	}
}

void Economy::Withdraw(int amount)
{
	this->m_gold -= amount;
	this->m_transactions.push_back(amount);
	this->m_transactionsDocumentation.push_back("Withdrew: ");

	if (m_transactionsDocumentation.size() > 50)
	{
		m_transactions.erase(m_transactions.begin(), m_transactions.begin() + 30);
		m_transactionsDocumentation.erase(m_transactionsDocumentation.begin() + 0, m_transactionsDocumentation.begin() + 30);
	}
}

void Economy::GetRefund(int amount)
{
	m_gold -= amount;
	m_transactions.push_back(amount);
	m_transactionsDocumentation.push_back("Customer refund: ");

	if (m_transactionsDocumentation.size() > 50)
	{
		m_transactions.erase(m_transactions.begin(), m_transactions.begin() + 30);
		m_transactionsDocumentation.erase(m_transactionsDocumentation.begin() + 0, m_transactionsDocumentation.begin() + 30);
	}
}

void Economy::GetCashback(int amount)
{
	m_gold += amount;
	m_transactions.push_back(amount);
	m_transactionsDocumentation.push_back("Cashback: ");

	if (m_transactionsDocumentation.size() > 50)
	{
		m_transactions.erase(m_transactions.begin(), m_transactions.begin() + 30);
		m_transactionsDocumentation.erase(m_transactionsDocumentation.begin() + 0, m_transactionsDocumentation.begin() + 30);
	}
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
