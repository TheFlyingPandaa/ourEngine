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

	void deposit(int amount);
	void withdraw(int amount);
	
	int GetGold() const;

	std::vector<std::string> GetTransactionHistory() const;

};