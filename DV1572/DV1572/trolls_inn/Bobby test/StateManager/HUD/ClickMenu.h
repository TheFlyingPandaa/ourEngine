#pragma once
#include "../../../../ourEngine/interface/Interface.h"
#include "../../../Mesh Manager/MeshManager.h"
#include <vector>

class ClickMenu
{
public:
	enum MTYPE : short
	{
		FUR,
		CHA
	} m_type;


private:
	RectangleShape		m_background;
	std::vector<Text>	m_texts;
	std::vector<Mesh*>	m_mesh;
	Text				m_info;
	int					m_distFromBoarder;
	std::vector<RectangleShape*> m_buttons;
	bool m_wasPressed;
	Mesh* m_recMesh;

public:
	ClickMenu(MTYPE type);
	~ClickMenu();

	void ClearSubText();
	void PushText(const std::string & row);

	void setPos(DirectX::XMFLOAT2 pos);

	//Returns -2 if not even the background is pressed, else -1 or button index
	int ButtonClicked();
	void setInfo(const std::string & info);
	void Draw();

private:
	void _initFurnitureMenu();
	void _initCharacterMenu();
};