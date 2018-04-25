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
	Object3D test;
	//-----
	int is = 0;

	Shape *		m_startTile;
	Shape *		m_selectedTile;
	BuildStage	m_buildStage;
	bool		m_roomPlaceable;
	Grid *		grid;
	RoomCtrl*	m_roomCtrl;


	// <Main HUD>
	bool m_madeFullResetMain;
	bool m_clickedLastFrame;
	std::vector<bool> m_hudButtonsPressed;
	// </Main Hud>

	// <Create Room HUD>
	bool				m_madeFullResetRoomHud;
	HUD					m_roomHUD;
	std::vector<bool>	m_roomHUDButtonsPressed;
	RoomType			m_selectedRoomType;
	// </Create Room HUD>

	// <Create Room HUD>
	HUD					m_doorHUD;
	int					m_selectedDoor;
	// </Create Room HUD>

	HUD		m_furnitureHUD;
	int		m_selectedFurniture = -1;
	
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
	bool	_mainHudPick();
	bool	_roomBuildHudPick();
	bool	_doorBuildHudPick();

	void _resetHudButtonPressedExcept(int index, std::vector<bool> &vec, HUD &selectedHud);
public:

	BuildState(Camera * cam, 
	std::stack<Shape *>* pickingEvent,
	Grid * grid,
	RoomCtrl* roomCtrl);
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

