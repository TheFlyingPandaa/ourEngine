#include "HUD.h"

void HUD::_cleanUp()
{
	for (size_t i = 0; i < m_quadsNonClickAble.size(); i++)
		delete m_quadsNonClickAble[i];
	m_quadsNonClickAble.clear();
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
		delete m_quadsClickAble[i];
	m_quadsClickAble.clear();
	for (size_t i = 0; i < m_mesh.size(); i++)
		delete m_mesh[i];
	m_mesh.clear();
	m_texts.clear();

	for (size_t i = 0; i < m_texts.size(); i++)
	{
		if (m_texts[i] != nullptr)
		{
			delete m_texts[i];
			m_texts[i] = nullptr;
		}
	}
	m_texts.clear();
}

void HUD::_setupAPotentialAreaCircle(int x, int y, int r, int relative)
{
	RectangleShape::RelativeTo rt = static_cast<RectangleShape::RelativeTo>(relative);
	DirectX::XMINT2 s = Input::getWindowSize();
	DirectX::XMINT2 pos(x, y);

	switch (rt)
	{
	case RectangleShape::BR:
		pos.x = static_cast<float>(s.x) - pos.x;
		break;
	case RectangleShape::TR:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case RectangleShape::TL:
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case RectangleShape::C:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		break;
	case RectangleShape::BC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		break;
	case RectangleShape::TC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		break;
	case RectangleShape::LC:
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		break;
	case RectangleShape::RC:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
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
		pos.x = static_cast<float>(s.x) - pos.x;
		offsetX = -scl.x;
		break;
	case RectangleShape::TR:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetX = -scl.x;
		offsetY = -scl.y;
		break;
	case RectangleShape::TL:
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetY = -scl.y;
		break;
	case RectangleShape::C:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetX = -(scl.x / 2.0f);
		offsetY = -(scl.y / 2.0f);
		break;
	case RectangleShape::BC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		offsetX = -(scl.x / 2.0f);
		break;
	case RectangleShape::TC:
		pos.x = (static_cast<float>(s.x) / 2.0f) + pos.x;
		pos.y = static_cast<float>(s.y) - pos.y;
		offsetX = -(scl.x / 2.0f);
		offsetY = -scl.y;
		break;
	case RectangleShape::LC:
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetY = -(scl.y / 2.0f);
		break;
	case RectangleShape::RC:
		pos.x = static_cast<float>(s.x) - pos.x;
		pos.y = (static_cast<float>(s.y) / 2.0f) + pos.y;
		offsetX = -scl.x;
		offsetY = -(scl.y / 2.0f);
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
				
				float pX, pY, sX, sY = 0.0f;
				int relativeTo;
				std::string shape = "";
				int index = 0;
				float d = 0;

				stream >> pX >> pY >> sX >> sY >> index >> d >> relativeTo >> shape;

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
					m_quadsNonClickAble.push_back(r);
				else
				{
					r->setIndex(index);
					m_quadsClickAble.push_back(r);
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
		int dist = static_cast<int>(DirectX::XMVectorGetX(DirectX::XMVector2Length((XMVectorSet(pa.x, pa.y, 0.0f, 0.0f) - XMLoadFloat2(&mousePos)))) + 0.5f);
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
	m_quadsClickAble[index]->setColor(r, g, b);
}

void HUD::ResetColorsExcept(int index)
{
	for (size_t i = 0; i < m_quadsClickAble.size(); i++)
	{
		if (m_quadsClickAble[i]->getIndex() != index)
			m_quadsClickAble[i]->setColor(1, 1, 1);
	}
}

//void HUD::CheckIfPicked()
//{
//	for (auto& p : m_quadsClickAble)
//	{
//		p->CheckPick();
//	}
//}

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
	
	for (auto& t : m_texts)
	{
		t->Draw();
	}
}

void HUD::addText(Text * text)
{
	m_texts.push_back(text);
}
