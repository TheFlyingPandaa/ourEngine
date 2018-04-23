#pragma once
#include "SubState.h"
#include "../../Room/Grid.h"
#include "../../../Furniture/Table.h"
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

	//TEMP
	Mesh table;
	//-----


	RoomType	m_selectedRoomType;
	Shape *		m_startTile;
	Shape *		m_selectedTile;
	BuildStage	m_buildStage;
	bool		m_roomPlaceable;
	Grid *		grid;

	HUD			m_roomHUD;
	HUD			m_doorHUD;
	HUD			m_furnitureHUD;


	bool m_madeFullReset;
	bool m_clickedLastFrame;
	std::vector<bool> m_hudButtonsPressed;
	void _resetHudButtonPressedExcept(int index);

	Text m_priceOfRoom;
	
	
	bool m_readyToPick;

	bool m_doorBuild = false;
	bool m_canBuildFurniture = false;
	enum CurrentBuildType	//This will replace the old system of setting bools aka m_doorBuild =...
	{
		Room,
		Door,
		Furniture,
		NONE
	};
	CurrentBuildType m_currentBuildType;

	void	_handleBuildRoom(Shape * pickedShape);
	void	_buildInput();
	void	_doorBuildInput();
	void	_roomBuildInput();
	void	_objectBuildInput();
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

