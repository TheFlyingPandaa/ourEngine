#pragma once
#include <vector>
#include <string>

class Economy
{
private:
	int	gold;
	std::vector<int> transactions;
	std::vector<std::string> transactionsDocumentation;

public:
	Economy();
	Economy(int startGold);
	~Economy();

	void deposit(int amount);
	bool withdraw(int amount);

	void update();
	
	int getGold() const;

	std::vector<std::string> getTransactionHistory() const;

};