#include "OurEngine.h"
#include "MainMenu.h"

MainMenu::MainMenu(std::stack<Shape*>* pickingEvent, std::stack<int>* keyEvent, Camera * cam) : State(pickingEvent, keyEvent)
{
	std::cout << "hellp\n";
	this->m_cam = cam;
	this->_init();
	m_stateHUD.LoadHud("trolls_inn/Resources/HUD/MainHud/LoseHud.txt");

}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(double deltaTime)
{
	if (Input::isKeyPressed('M'))
		m_exitState = true;
	m_cam->update();
}

void MainMenu::Draw()
{
}

void MainMenu::DrawHUD()
{
	m_stateHUD.Draw();
}

void MainMenu::_init()
{
}
