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

HUD::HUD()
{
	m_windowX = 1280.0f;
	m_windowY = 720.0f;
}

HUD::~HUD()
{
	_cleanUp();
}

void HUD::setWindowSize(float x, float y)
{
	m_windowX = x;
	m_windowY = y;
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
				int p = 0;
				float d = 0;
				sscanf_s(currentLine.c_str(), "%*s %f %f %f %f %d %f", &pX, &pY, &sX, &sY, &p, &d);

				d *= 0.00001f;

				r->setScreenPos(pX, pY, d);

				if (static_cast<int>(sX) == 0)
					sX = m_windowX;
				if (static_cast<int>(sY) == 0)
					sY = m_windowY;

				r->setWidth(sX);
				r->setHeight(sY);
				
				if (p != 0)
					m_quadsNonClickAble.push_back(r);
				else
					m_quadsClickAble.push_back(r);
			}
			else if (type == "txt")
			{
				float x, y, scl, r, g, b, a, rot;
				std::string text = "";
				sscanf_s(currentLine.c_str(), "%*s %f %f %f %f %f %f %f %f", &x, &y, &scl, &r, &g, &b, &a, &rot);
				std::getline(inputFile, text);
				Text t;
				t.setColor(r, g, b, a);
				t.setPosition(x, y);
				t.setScale(scl);
				t.setRotation(rot);
				t.setTextString(text);
				m_texts.push_back(t);
			}

		}
	}
	inputFile.close();


	return true;
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
	for (auto& p : m_quadsClickAble)
	{
		p->DrawAsHud();
	}
	for (auto& p : m_quadsNonClickAble)
	{
		p->DrawAsHud();
	}
	for (auto& t : m_texts)
	{
		t.Draw();
	}
}
