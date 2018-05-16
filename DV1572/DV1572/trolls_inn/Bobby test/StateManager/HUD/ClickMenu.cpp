#include "ClickMenu.h"

ClickMenu::ClickMenu(MTYPE type)
	: m_type(type)
{
	DirectX::XMFLOAT2 pos = Input::getMousePositionLH();
	
	m_recMesh = new Mesh();
	m_recMesh->MakeRectangle();
	m_background.setMesh(m_recMesh);
	
	m_background.setPos(pos.x, pos.y, 0.1);
	m_background.setWidth(150);
	m_background.setHeight(150);
	
	m_info.setAllignment(TXT::Center);
	m_info.setRelative(Text::BL);
	m_info.setScale(0.4f);

	switch (m_type)
	{
	case ClickMenu::FUR:
		_initFurnitureMenu();
		m_background.GetMesh()->setDiffuseTexture("trolls_inn/Resources/HUD/plain.png");
		break;
	case ClickMenu::CHA:
		_initCharacterMenu();
		m_background.GetMesh()->setDiffuseTexture("trolls_inn/Resources/HUD/MainHud/ButtonBackground.png");
		m_background.setWidth(400);
		m_background.setHeight(115);
		break;
	}


	setPos(DirectX::XMFLOAT2(300, 300));

}

ClickMenu::~ClickMenu()
{
	for (auto & m : m_mesh)
		delete m;
	m_mesh.clear();
	for (auto & b : m_buttons)
		delete b;
	m_buttons.clear();
	delete m_recMesh;
}

void ClickMenu::ClearSubText()
{
	m_texts.clear();
}

void ClickMenu::PushText(const std::string & row)
{
	Text _row;
	_row.setAllignment(TXT::Center);
	_row.setRelative(Text::BL);
	_row.setScale(0.4f);
	_row.setTextString(row);

	m_texts.push_back(_row);
}

void ClickMenu::setPos(DirectX::XMFLOAT2 pos)
{

	m_background.setScreenPos(pos.x, pos.y, 0.1);
	m_info.setPosition(
		(pos.x + m_background.getWidth() / 2),
			pos.y + m_background.getHeight() - DirectX::XMVectorGetY(Text::getStringSize(&m_info) * m_info.getScale()) / 2);
	int counter = 0;
	for (auto & t : m_texts)
	{
		t.setPosition(
			(pos.x + m_background.getWidth() / 2),
			pos.y + m_background.getHeight() - DirectX::XMVectorGetY(Text::getStringSize(&m_info)) - DirectX::XMVectorGetY(Text::getStringSize(&t) * counter++ * m_info.getScale()) / 2);
	}

	m_buttons[0]->setScreenPos(
		pos.x + 10.0f,
		pos.y + 10.0f
	);
	for (size_t i = 1; i < m_buttons.size(); i++)
	{
		m_buttons[i]->setScreenPos(
			(pos.x + m_background.getWidth() / (m_buttons.size()*4.0f)) * i,
		pos.y + 10.0f
		);
	}
	m_buttons[m_buttons.size() - 1]->setScreenPos(
		pos.x + m_background.getWidth() - m_buttons[m_buttons.size() - 1]->getWidth() - 10.0f,
		pos.y + 10.0f
	);
}

int ClickMenu::ButtonClicked()
{
	int index = 0;
	DirectX::XMFLOAT2 mousePos = Input::getMousePositionLH();

	for (auto & b : m_buttons)
	{
		
		DirectX::XMFLOAT3 pa = b->getScreenPos();
		DirectX::XMFLOAT2 s(b->getWidth(), b->getHeight());
		if (mousePos.x > pa.x && mousePos.x < pa.x + s.x &&
			mousePos.y > pa.y && mousePos.y < pa.y + s.y)
		{
			return index;
		}
		index++;
	}

	DirectX::XMFLOAT3 pa = m_background.getScreenPos();
	DirectX::XMFLOAT2 s(m_background.getWidth(), m_background.getHeight());
	if (mousePos.x > pa.x && mousePos.x < pa.x + s.x &&
		mousePos.y > pa.y && mousePos.y < pa.y + s.y)
	{
		return -1;
	}


	return -2;
}

void ClickMenu::setInfo(const std::string & info)
{
	m_info.setTextString(info);
}

void ClickMenu::Draw()
{
	m_background.DrawAsHud();

	for (auto & b : m_buttons)
		b->DrawAsHud();

	for (auto & t : m_texts)
		t.Draw();
	m_info.Draw();
}

void ClickMenu::_initFurnitureMenu()
{
	Mesh * m = new Mesh();
	m->MakeRectangle();
	m->setDiffuseTexture("trolls_inn/Resources/HUD/ClickHUD/PickUpItem.png");

	RectangleShape * r = new RectangleShape();
	r->setColor(5, 5, 5);
	r->setMesh(m);
	r->setWidth(32);
	r->setHeight(40);

	m_mesh.push_back(m);
	m_buttons.push_back(r);

	m = new Mesh();
	m->MakeRectangle();
	m->setDiffuseTexture("trolls_inn/Resources/HUD/ClickHUD/Upgrade.png");
	r = new RectangleShape();
	r->setColor(5, 5, 5);
	r->setMesh(m);
	r->setWidth(32);
	r->setHeight(40);

	m_mesh.push_back(m);
	m_buttons.push_back(r);

	m = new Mesh();
	m->MakeRectangle();
	m->setDiffuseTexture("trolls_inn/Resources/HUD/ClickHUD/Sell.png");
	r = new RectangleShape();
	r->setColor(5, 5, 5);
	r->setMesh(m);
	r->setWidth(40);
	r->setHeight(40);

	m_mesh.push_back(m);
	m_buttons.push_back(r);
}

void ClickMenu::_initCharacterMenu()
{
	Mesh * m = new Mesh();
	m->MakeRectangle();
	m->setDiffuseTexture("trolls_inn/Resources/HUD/ClickHUD/killIcon.png");

	RectangleShape * r = new RectangleShape();
	r->setMesh(m);
	r->setWidth(63*2);
	r->setHeight(44*2);

	m_mesh.push_back(m);
	m_buttons.push_back(r);

	m = new Mesh();
	m->MakeRectangle();
	m->setDiffuseTexture("trolls_inn/Resources/HUD/ClickHUD/stealcon.png");
	r = new RectangleShape();
	r->setMesh(m);
	r->setWidth(63*2);
	r->setHeight(44*2);

	m_mesh.push_back(m);
	m_buttons.push_back(r);

}
