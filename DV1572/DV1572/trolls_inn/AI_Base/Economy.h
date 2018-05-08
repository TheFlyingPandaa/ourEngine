#pragma once
#include <vector>
#include <string>

class Economy
{
private:
	int	m_gold;
	std::vector<int> m_transactions;
	std::vector<std::string> m_transactionsDocumentation;

public:
	Economy();
	Economy(int startGold);
	~Economy();

	void Deposit(int amount);
	void Withdraw(int amount);
	// For unhappy customers
	void GetRefund(int amount);
	void GetCashback(int amount);
	
	int GetGold() const;

	std::vector<std::string> GetTransactionHistory() const;

};