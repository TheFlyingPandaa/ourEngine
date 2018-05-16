#pragma once
#include "Event.h"



class TutorialEvent : public Event
{
private:
	CallQueue<TutorialEvent> q;
	
	void _BuildReception();
	void _BuildReceptionFurniture();
public:
	TutorialEvent(Inn * inn, RoomCtrl * roomctrl);
	~TutorialEvent();

	void Update() override;
	void Draw() override;

	
};


