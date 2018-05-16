#pragma once
#include "Event.h"

class TutorialEvent : public Event
{
private:
	std::queue<void(TutorialEvent::*)()> m_queue;

	void CallFunction(void (TutorialEvent::*func)());
	
	void _BuildReception();
	void _BuildReceptionFurniture();
public:
	TutorialEvent(Inn * inn, RoomCtrl * roomctrl);
	~TutorialEvent();

	void Update() override;
	void Draw() override;

	
};
