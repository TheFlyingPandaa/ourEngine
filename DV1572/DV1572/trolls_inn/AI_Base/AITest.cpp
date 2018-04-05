#include "../../ourEngine/interface/Window.h"
#include "Customer.h"
#include "Staff.h"
#include <vector>
#include <iostream>
#include "CustomerFlowControl.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{//	Activation of Console
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	
	std::vector<Customer> customers;
	std::vector<Staff> staff;
	CustomerFlowControl cFC;

	staff.push_back(Staff(Cook));
	customers.push_back(Customer(Human, 100));

	system("Pause");

	return 0;
}