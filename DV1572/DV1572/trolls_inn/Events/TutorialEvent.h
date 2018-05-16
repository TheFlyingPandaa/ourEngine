#pragma once
#include "Event.h"



class TutorialEvent : public Event
{
private:
	CallQueue<TutorialEvent, void> q;
	
	void _BuildReception();
	void _BuildReceptionFurniture();
	void _BuildDoor();
	void _BuildBedRoom();
	void _BuildBedRoomBed();
	void _BuildKitchenRoom();

public:
	TutorialEvent(Inn * inn, RoomCtrl * roomctrl);
	~TutorialEvent();

	void Update() override;
	void Draw() override;

	
};


