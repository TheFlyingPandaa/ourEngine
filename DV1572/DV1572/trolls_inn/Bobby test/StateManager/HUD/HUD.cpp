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
	
	m_potentialAreas.push_back(PotentialAreaCircle{ pos.x, pos.y, r });

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
				std::string buffer = "";
				int index = 0;
				float d = 0;

				stream >> pX >> pY >> sX >> sY >> index >> d >> relativeTo;

				d *= 0.00001f;
				if (static_cast<int>(sX) == 0)
					sX = static_cast<float>(Input::getWindowSize().x);
				if (static_cast<int>(sY) == 0)
					sY = static_cast<float>(Input::getWindowSize().y);

				r->setWidth(sX);
				r->setHeight(sY);

				r->setRelative(static_cast<RectangleShape::RelativeTo>(relativeTo));
				r->setScreenPos(pX, pY, d);

				
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
				Text t;
				t.setColor(r, g, b, a);
				t.setRelative(static_cast<Text::RelativeTo>(relativeTo));
				t.setPosition(x, y);
				t.setScale(scl);
				t.setRotation(rot);
				t.setTextString(text);

				switch (allignment)
				{
				case 'R':
					t.setAllignment(TXT::Right);
					break;
				case 'C':
					t.setAllignment(TXT::Center);
					break;
				default:
					t.setAllignment(TXT::Left);
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

bool HUD::isMouseInsidePotentialArea(DirectX::XMFLOAT2 mousePos)
{
	for (auto pa : m_potentialAreas)
	{
		int dist = static_cast<int>(DirectX::XMVectorGetX(DirectX::XMVector2Length((XMVectorSet(pa.x, pa.y, 0.0f, 0.0f) - XMLoadFloat2(&mousePos)))) + 0.5f);
		if (dist < pa.r)
			return true;
	}

	return false;
}


void HUD::ResetColorsOnPickable()
{
	for (RectangleShape * s : m_quadsClickAble)
	{
		s->setColor(1.0f, 1.0f, 1.0f);
	}
}

void HUD::CheckIfPicked()
{
	for (auto& p : m_quadsClickAble)
	{
		p->CheckPick();
	}
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
	
	for (auto& t : m_texts)
	{
		t.Draw();
	}
}
