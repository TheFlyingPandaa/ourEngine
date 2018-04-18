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


	std::vector<bool> m_buttonPressed;

	bool m_readyToPick;

	bool m_doorBuild = false;
	void	_handleBuildRoom(Shape * pickedShape);
	void	_buildInput();

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

