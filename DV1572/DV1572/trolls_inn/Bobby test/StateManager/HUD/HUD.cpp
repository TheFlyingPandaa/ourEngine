#include "HUD.h"

void HUD::_cleanUp()
{
	for (size_t i = 0; i < m_quadsNonClickAble.size(); i++)
		delete m_quadsNonClickAble[i];
	m_quadsNonClickAble.clear();
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
		delete m_quadsClickAble[i];
	m_quadsClickAble.clear();
	
	for (size_t i = 0; i < m_meterBarsNotSlideAble.size(); i++)
		delete m_meterBarsNotSlideAble[i];
	m_meterBarsNotSlideAble.clear();

	for (size_t i = 0; i < m_meterBarsSlideAble.size(); i++)
		delete m_meterBarsSlideAble[i];
	m_meterBarsSlideAble.clear();


	for (size_t i = 0; i < m_mesh.size(); i++)
		delete m_mesh[i];
	// no need to clear
	//m_mesh.clear();
	
	for (size_t i = 0; i < m_texts.size(); i++)
	{
		if (m_texts[i] != nullptr)
		{
			delete m_texts[i];
			m_texts[i] = nullptr;
		}
	}
	//m_texts.clear();
}

void HUD::_setupAPotentialAreaCircle(int x, int y, int r, int relative)
{
	RectangleShape::RelativeTo rt = static_cast<RectangleShape::RelativeTo>(relative);
	DirectX::XMINT2 s = Input::getWindowSize();
	DirectX::XMINT2 pos(x, y);

	switch (rt)
	{
	case RectangleShape::BR:
		pos.x = s.x - pos.x;
		break;
	case RectangleShape::TR:
		pos.x = s.x - pos.x;
		pos.y = s.y - pos.y;
		break;
	case RectangleShape::TL:
		pos.y = s.y - pos.y;
		break;
	case RectangleShape::C:
		pos.x = (s.x / 2) + pos.x;
		pos.y = (s.y / 2) + pos.y;
		break;
	case RectangleShape::BC:
		pos.x = (s.x / 2) + pos.x;
		break;
	case RectangleShape::TC:
		pos.x = (s.x / 2) + pos.x;
		pos.y = s.y - pos.y;
		break;
	case RectangleShape::LC:
		pos.y = (s.y / 2) + pos.y;
		break;
	case RectangleShape::RC:
		pos.x = s.x - pos.x;
		pos.y = (s.y / 2) + pos.y;
		break;
	}
	
	m_potentialAreasCircle.push_back(PotentialAreaCircle{ pos.x, pos.y, r });

}

void HUD::_setupAPotentialAreaRect(int x, int y, int sx, int sy, int relative)
{
	DirectX::XMINT2 s = Input::getWindowSize();
	DirectX::XMINT2 scl(sx, sy);
	DirectX::XMINT2 pos(x, y);

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	switch (relative)
	{
	case RectangleShape::BR:
		pos.x = s.x - pos.x;
		offsetX = static_cast<float>(-scl.x);
		break;
	case RectangleShape::TR:
		pos.x = s.x - pos.x;
		pos.y = s.y - pos.y;
		offsetX = static_cast<float>(-scl.x);
		offsetY = static_cast<float>(-scl.y);
		break;
	case RectangleShape::TL:
		pos.y = s.y - pos.y;
		offsetY = static_cast<float>(-scl.y);
		break;
	case RectangleShape::C:
		pos.x = (s.x / 2) + pos.x;
		pos.y = (s.y / 2) + pos.y;
		offsetX = -(static_cast<float>(scl.x) / 2.0f);
		offsetY = -(static_cast<float>(scl.y) / 2.0f);
		break;
	case RectangleShape::BC:
		pos.x = (s.x / 2) + pos.x;
		offsetX = -(static_cast<float>(scl.x) / 2.0f);
		break;
	case RectangleShape::TC:
		pos.x = (s.x / 2) + pos.x;
		pos.y = s.y - pos.y;
		offsetX = -(static_cast<float>(scl.x) / 2.0f);
		offsetY = static_cast<float>(-scl.y);
		break;
	case RectangleShape::LC:
		pos.y = (s.y / 2) + pos.y;
		offsetY = -(static_cast<float>(scl.y) / 2.0f);
		break;
	case RectangleShape::RC:
		pos.x = s.x - pos.x;
		pos.y = (s.y / 2) + pos.y;
		offsetX = static_cast<float>(-scl.x);
		offsetY = -(static_cast<float>(scl.y)/ 2.0f);
		break;
	}

	PotentialAreaRect rect{ pos.x, pos.y, scl.x, scl.y };
	m_potentialAreasRect.push_back(rect);

}

HUD::HUD()
{

}

HUD::~HUD()
{
	_cleanUp();
	for (auto &t : m_pushedTexts)
		delete t;
	m_pushedTexts.clear();
}

bool HUD::LoadHud(const std::string & path)
{
	_cleanUp();
	
	std::ifstream inputFile;
	inputFile.open(path);
	if (!inputFile)
		return false;

	std::string currentLine = "";
	while (std::getline(inputFile, currentLine))
	{
		if (currentLine[0] != '#' && currentLine != "")
		{
			std::istringstream stream(currentLine);
			std::string type;

			stream >> type;

			if (type == "tx")
			{
				std::string path;
				//sscanf_s(currentLine.c_str(), "%*s %s", &path);
				stream >> path;
				Mesh* m = new Mesh();
				m->MakeRectangle();
				m->setDiffuseTexture(path);
				m_mesh.push_back(m);
			}
			else if (type == "r")
			{
				RectangleShape* r;
				r = new RectangleShape();
				r->setMesh(m_mesh[m_mesh.size() - 1]);
				
				float pX, pY, sX, sY;
				int relativeTo;
				std::string shape = "";

				DirectX::XMFLOAT3 hover;
				DirectX::XMFLOAT3 press;

				int index = 0;
				float d = 0;

				stream >> pX >> pY >> sX >> sY >> index >> d >> relativeTo >> shape >> hover.x >> hover.y >> hover.z >> press.x >> press.y >> press.z;

				d *= 0.00001f;
				if (static_cast<int>(sX) == 0)
					sX = static_cast<float>(Input::getWindowSize().x);
				if (static_cast<int>(sY) == 0)
					sY = static_cast<float>(Input::getWindowSize().y);

				r->setWidth(sX);
				r->setHeight(sY);

				r->setRelative(static_cast<RectangleShape::RelativeTo>(relativeTo));
				r->setScreenPos(pX, pY, d);

				if (shape == "circle")
					r->setShapeType(RectangleShape::ShapeType::Circle);
				else if (shape == "elipse")
					r->setShapeType(RectangleShape::ShapeType::Elipse);
				else if (shape == "rectangle")
					r->setShapeType(RectangleShape::ShapeType::Rectangle);
				
				if (index < 0)
				{
					m_quadsNonClickAble.push_back(r);
				}
				else
				{
					r->setIndex(index);
					r->setHoverColor(hover);
					r->setPressColor(press);
					m_quadsClickAble.push_back(r);
				}
			}
			else if (type == "meter")
			{
				std::string path;
				stream >> path;
				Mesh* m = new Mesh();
				m->MakeRectangle();
				m->setDiffuseTexture(path);
				m_mesh.push_back(m);
				std::getline(inputFile, currentLine);
				path = currentLine;
				m = new Mesh();
				m->MakeRectangle();
				m->setDiffuseTexture(path);
				m_mesh.push_back(m);

				std::getline(inputFile, currentLine);
				std::istringstream aStream(currentLine);

				RectangleShape* back;
				back = new RectangleShape();
				back->setMesh(m_mesh[m_mesh.size() - 2]);

				RectangleShape* needle;
				needle = new RectangleShape();
				needle->setMesh(m_mesh[m_mesh.size() - 1]);

				float pX, pY, sX, sY;
				int relativeTo;
				std::string shape = "";

				DirectX::XMFLOAT3 hover;
				DirectX::XMFLOAT3 press;

				int index = 0;
				float d = 0;

				aStream >> pX >> pY >> sX >> sY >> index >> d >> relativeTo >> shape >> hover.x >> hover.y >> hover.z >> press.x >> press.y >> press.z;

				d *= 0.00001f;
				if (static_cast<int>(sX) == 0)
					sX = static_cast<float>(Input::getWindowSize().x);
				if (static_cast<int>(sY) == 0)
					sY = static_cast<float>(Input::getWindowSize().y);

				needle->setWidth(sX);
				needle->setHeight(sY);

				needle->setRelative(static_cast<RectangleShape::RelativeTo>(relativeTo));
				needle->setScreenPos(pX, pY, d);

				if (shape == "circle")
					needle->setShapeType(RectangleShape::ShapeType::Circle);
				else if (shape == "elipse")
					needle->setShapeType(RectangleShape::ShapeType::Elipse);
				else if (shape == "rectangle")
					needle->setShapeType(RectangleShape::ShapeType::Rectangle);

				MeterBar * meterBar = new MeterBar;
				meterBar->setNeedle(needle);

				std::getline(inputFile, currentLine);
				aStream = std::istringstream(currentLine);
				
				aStream >> pX >> pY >> sX >> sY >> d >> relativeTo >> shape;

				d *= 0.00001f;
				if (static_cast<int>(sX) == 0)
					sX = static_cast<float>(Input::getWindowSize().x);
				if (static_cast<int>(sY) == 0)
					sY = static_cast<float>(Input::getWindowSize().y);
				back->setWidth(sX);
				back->setHeight(sY);

				back->setRelative(static_cast<RectangleShape::RelativeTo>(relativeTo));
				back->setScreenPos(pX, pY, d);

				if (shape == "circle")
					back->setShapeType(RectangleShape::ShapeType::Circle);
				else if (shape == "elipse")
					back->setShapeType(RectangleShape::ShapeType::Elipse);
				else if (shape == "rectangle")
					back->setShapeType(RectangleShape::ShapeType::Rectangle);

				meterBar->setBackground(back);
				meterBar->SlideNeedleHorizontalBasedOnPrecentage(0.0f);
				meterBar->SlideNeedleVerticalBasedOnPrecentage(0.0f);

				if (index < 0)
				{
					m_meterBarsNotSlideAble.push_back(meterBar);
				}
				else
				{
					needle->setIndex(index);
					needle->setHoverColor(hover);
					needle->setPressColor(press);
					m_meterBarsSlideAble.push_back(meterBar);
				}

			}
			else if (type == "txt")
			{
				float x, y, scl, r, g, b, a, rot;
				char allignment;
				int relativeTo;
				std::string text = "";

				stream >> x >> y >> scl >> r >> g >> b >> a >> rot >> allignment >> relativeTo;
				std::getline(inputFile, text);
				Text* t = new Text();
				t->setFontType(TXT::FONT_TYPE::Arial_Black);
				t->setColor(r, g, b, a);
				t->setRelative(static_cast<Text::RelativeTo>(relativeTo));
				t->setPosition(x, y);
				t->setScale(scl);
				t->setRotation(rot);
				t->setTextString(text);

				switch (allignment)
				{
				case 'R':
					t->setAllignment(TXT::Right);
					break;
				case 'C':
					t->setAllignment(TXT::Center);
					break;
				default:
					t->setAllignment(TXT::Left);
					break;
				}

				m_texts.push_back(t);
			}
			else if (type == "hc")
			{
				int x, y, r;
				int relativeTo;
				stream >> x >> y >> r >> relativeTo;


				_setupAPotentialAreaCircle(x, y, r, relativeTo);
			}
			else if (type == "hr")
			{
				int x, y, sx, sy;
				int relativeTo;
				stream >> x >> y >> sx >> sy >> relativeTo;


				_setupAPotentialAreaRect(x, y, sx, sy, relativeTo);
			}

		}
	}
	inputFile.close();


	return true;
}

RectangleShape * HUD::Pick(DirectX::XMFLOAT2 at)
{
	for (RectangleShape * s : m_quadsClickAble)
	{
		DirectX::XMFLOAT3 scrnPosXYZ = s->getScreenPos();
		DirectX::XMFLOAT2 scrnPos(scrnPosXYZ.x, scrnPosXYZ.y);
		float w = s->getWidth();
		float h = s->getHeight();
		
		if (at.x > scrnPos.x && at.x < scrnPos.x + w &&
			at.y > scrnPos.y && at.y < scrnPos.y + h)
			return s;
	}
	return nullptr;
}

bool HUD::_isMouseInsidePotentialAreaCircle(DirectX::XMFLOAT2 mousePos) const
{
	for (auto pa : m_potentialAreasCircle)
	{
		int dist = static_cast<int>(DirectX::XMVectorGetX(DirectX::XMVector2Length((XMVectorSet(static_cast<float>(pa.x), static_cast<float>(pa.y), 0.0f, 0.0f) - XMLoadFloat2(&mousePos)))) + 0.5f);
		if (dist < pa.r)
			return true;
	}

	return false;
}

bool HUD::_isMouseInsidePotentialAreaRect(DirectX::XMFLOAT2 mousePos) const
{
	for (auto pa : m_potentialAreasRect)
	{
		if (mousePos.x > pa.x && mousePos.x < pa.x + pa.sx &&
			mousePos.y > pa.y && mousePos.y < pa.y + pa.sy)
			return true;
	}

	return false;
}

bool HUD::_checkAgainstCircle(DirectX::XMFLOAT2 mousePos, const RectangleShape & rect) const
{
	float r = rect.getWidth() / 2;
	
	DirectX::XMVECTOR o = DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(rect.getWidth(), rect.getHeight())) / 2;
	DirectX::XMVECTOR m = DirectX::XMLoadFloat2(&mousePos);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&rect.getScreenPos());
	p += o;
	float d = DirectX::XMVectorGetX(DirectX::XMVector2Length(m - p));

	if (d > r)
		return false;
	
	return true;
}

bool HUD::_checkAgainstRectangle(DirectX::XMFLOAT2 mousePos, const RectangleShape & rect) const
{
	DirectX::XMFLOAT3 pa = rect.getScreenPos();
	DirectX::XMFLOAT2 s(rect.getWidth(), rect.getHeight());

		if (mousePos.x > pa.x && mousePos.x < pa.x + s.x &&
			mousePos.y > pa.y && mousePos.y < pa.y + s.y)
			return true;

	return false;
}


void HUD::ResetColorsOnPickableWithIndex(int index)
{
	for (RectangleShape * s : m_quadsClickAble)
	{
		if (s->getIndex() == index)
		{
			s->setColor(1, 1, 1);
			return;
		}

	}
}

int HUD::getNrOfPickableButtons() const
{
	return static_cast<int>(m_quadsClickAble.size());
}

int HUD::PickHud(DirectX::XMFLOAT2 mousePos) const
{
	int returnIndex = -2;

	if (_isMouseInsidePotentialAreaRect(mousePos) || _isMouseInsidePotentialAreaCircle(mousePos))
	{
		returnIndex = -1;
		for (size_t i = 0; i < m_quadsClickAble.size(); i++)
		{
			switch (m_quadsClickAble[i]->getShapeType())
			{
			case RectangleShape::ShapeType::Circle:
				if (_checkAgainstCircle(mousePos, *m_quadsClickAble[i]))
				{
					return m_quadsClickAble[i]->getIndex();
				}
				break;
			case RectangleShape::ShapeType::Elipse:
				printf("Yet to be implemented\n");
				/*if (_checkAgainstElipse(mousePos, *m_quadsClickAble[i]))
				{
					return m_quadsClickAble[i]->getIndex();
				}*/
				break;
			default:
				if (_checkAgainstRectangle(mousePos, *m_quadsClickAble[i]))
				{
					return m_quadsClickAble[i]->getIndex();
				}
				break;
			}
		}
	}
	
	return returnIndex;
}

void HUD::SetColorOnButton(int index, float r, float g, float b)
{
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
	{
		if (m_quadsClickAble[i]->getIndex() == index)
		{
			m_quadsClickAble[i]->setColor(r, g, b);
			return;
		}
	}
}

void HUD::SetHoverColorOnButton(int index)
{
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
	{
		if (m_quadsClickAble[i]->getIndex() == index)
		{
			m_quadsClickAble[i]->setColor(m_quadsClickAble[i]->getHoverColor());
			return;
		}
	}
}

void HUD::SetPressColorOnButton(int index)
{
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
	{
		if (m_quadsClickAble[i]->getIndex() == index)
		{
			m_quadsClickAble[i]->setColor(m_quadsClickAble[i]->getPressedColor());
			return;
		}
	}
}

void HUD::ResetColorsExcept(int index)
{
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
	{
		if (m_quadsClickAble[i]->getIndex() != index)
			m_quadsClickAble[i]->setColor(1, 1, 1);
	}
}

// Blblbla
void HUD::SlideMeterBarWithIndex(int index, float x, float y)
{
	for (auto &m : m_meterBarsSlideAble)
	{
		if (m->getNeedleIndex() == index)
		{
			m->SlideNeedleHorizontalBasedOnPrecentage(x);
			m->SlideNeedleVerticalBasedOnPrecentage(y);
			return;
		}
	}
}

MeterBar * HUD::getMeterBarWithIndex(int index)
{
	for (auto &m : m_meterBarsSlideAble)
	{
		if (m->getNeedleIndex() == index)
		{
			return m;
		}
	}
	return nullptr;
}

MeterBar * HUD::getMeterBarAtMousePosition()
{
	for (auto &m : m_meterBarsSlideAble)
	{
		if (_checkAgainstRectangle(Input::getMousePositionLH(), *m->getNeedle()))
			return m;	
	}
	return nullptr;
}

void HUD::Draw()
{
	for (auto& p : m_quadsNonClickAble)
	{
		p->DrawAsHud();
	}

	for (auto& p : m_quadsClickAble)
	{
		p->DrawAsHud();
	}
	
	for (auto& t : m_pushedTexts)
	{
		t->Draw();
	}
	for (auto& t : m_texts)
	{
		t->Draw();
	}
	for (auto& p : m_meterBarsNotSlideAble)
		p->Draw();
	for (auto& p : m_meterBarsSlideAble)
		p->Draw();
}

void HUD::addText(Text * text)
{
	m_pushedTexts.push_back(text);
}
