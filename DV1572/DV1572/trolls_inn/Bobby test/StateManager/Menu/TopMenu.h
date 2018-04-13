#pragma once
#include "../State.h"
class TopMenu : public State
{
private:
	void _init() override;



public:
	TopMenu();
	~TopMenu();


	void Update(double deltaTime) override;
	void Draw() override;
	
};
