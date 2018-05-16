#pragma once
#include "SubState.h"
#include "../../Room/Grid.h"
#include "../../../Furniture/Table.h"
#include "../../../Mesh Manager/MeshManager.h"
#include "../../../AI_Base/Inn.h"
#include "../../../Furniture/Bed.h"
#include "../HUD/InfoBoard.h"
#include "../HUD/ClickMenu.h"

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
	Object3D door;
	Furniture* table;
	ClickMenu * m_cmFur;
	ClickMenu * m_cmRoom;


	bool drawSelectedThing;
	bool twoStepThingy = false;
	bool rotationKeyPressed = false;
	int lastSelected = -1;
	//-----
	int is = 0;

	Shape *		m_startTile;
	Shape *		m_selectedTile;
	BuildStage	m_buildStage;
	bool		m_roomPlaceable;
	Grid *		grid;
	RoomCtrl*	m_roomCtrl;

	InfoBoard	m_floatingText;
	bool		m_drawFloatingText;

	// <Main HUD>
	bool m_madeFullResetMain;
	bool m_clickedLastFrame;
	std::vector<bool> m_hudButtonsPressed;
	// </Main Hud>

	HUD					m_roomHUD;
	HUD					m_doorHUD;
	HUD					m_furnitureHUD;
	int					m_selectedThing;
	Room*				m_selectedRoom;
	Room*				m_selectedRoomm;
	// </Create Room HUD>

	Inn * m_inn;
	Text m_priceOfRoom;
	
	

	bool m_readyToPick;
	bool m_doorBuild = false;
	bool m_canBuildFurniture = false;
	bool m_furnitureDeleteMode = false;
	bool m_clickedLast = false;
	bool m_freeFurniture = false;
	Furniture* m_furnitureRemove = nullptr;

	enum CurrentBuildType	//This will replace the old system of setting bools aka m_doorBuild =...
	{
		RoomBuild,
		Door,
		Furniture,
		NONE
	};
	CurrentBuildType m_currentBuildType;

	void	_handleBuildRoom(Shape * pickedShape);
	void	_buildInput();
	void	_objectBuildInput();
	bool	_handleHUDPicking();
	bool	_mainHudPick();
	bool	_selectionBuildHudPick(HUD & h, int & getIndex);
	

	void	_inputBuildRoom();
	void	_buildRoom();
	void	_inputDoor();

	void	_inputFurniture();

	void	_resetHudButtonPressedExcept(int index, std::vector<bool> &vec, HUD &selectedHud);
public:

	BuildState(Camera * cam, 
	std::stack<Shape *>* pickingEvent,
	Grid * grid,
	RoomCtrl* roomCtrl, Inn * i);
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

