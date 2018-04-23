#pragma once
#include "SubState.h"
#include "../../Room/Grid.h"

class BuildState :
	public SubState
{
private:
	enum BuildStage
	{
		None,
		Start,
		Selection,
		End
	};


	RoomType	m_selectedRoomType;
	Shape *		m_startTile;
	Shape *		m_selectedTile;
	BuildStage	m_buildStage;
	bool		m_roomPlaceable;
	Grid *		grid;


	bool m_madeFullReset;
	bool m_clickedLastFrame;
	std::vector<bool> m_hudButtonsPressed;
	void _resetHudButtonPressedExcept(int index);

	Text m_priceOfRoom;
	
	
	bool m_readyToPick;

	bool m_doorBuild = false;
	void	_handleBuildRoom(Shape * pickedShape);
	void	_buildInput();
	void	_doorBuildInput();
	bool	_handleHUDPicking();

public:

	BuildState(Camera * cam, 
	std::stack<Shape *>* pickingEvent,
	Grid * grid);
	~BuildState();

	

	// Inherited via SubState
	virtual void _init() override;
	virtual void Update(double deltaTime) override;
	virtual void Draw() override;
	virtual void DrawHUD() override;
	

	// Inherited via SubState
	virtual void HandlePicking(Shape * pickedObject) override;

	virtual void HandleInput() override;

};

