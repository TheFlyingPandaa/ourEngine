#include "InGameConsole.h"

std::vector < InGameConsole::text_t *> InGameConsole::text;

InGameConsole::InGameConsole()
{
}


InGameConsole::~InGameConsole()
{
	for (int i = 0; i < text.size(); i++)
	{	
		delete text[i];
	}
	text.clear();
}

void InGameConsole::update(double DeltaTime)
{	
	while (text.size() > maxSize)
	{
		delete text[0];
		text.erase(text.begin());
	}

	for (int i = 0; i < text.size(); i++)
	{
		text[i]->timer += DeltaTime;
		if (text[i]->timer > startFade)
		{			
			text[i]->text.setColor(1.0f, 1.0f, 1.0f, 1.0f - static_cast<float>((text[i]->timer - startFade) / fadeTime));
		}
		
	}


}

void InGameConsole::draw()
{
	for (auto & t : text)
		t->text.Draw();
}

void InGameConsole::pushString(const std::string & string, float r , float g , float b)
{

	std::stringstream ss;
	int rows = 1;
	for (int i = 0; i < string.length(); i++)
	{
		if (i % maxWidth == 0 && i != 0)
		{
			rows++;
			ss << "\n";
		}
		ss << string[i];
	}
	
	Text  t;
	t.setTextString(ss.str());
	t.setAllignment(TXT::ALLIGN::Right);
	t.setRelative(Text::RelativeTo::TR);
	t.setColor(r, g, b);
	t.setScale(.5f);

	XMVECTOR size = Text::getStringSize(&t);
	float y = DirectX::XMVectorGetY(size);
	y*=.5f;

	t.setPosition(static_cast<float>(xPos), y*maxSize);
	

	text_t * t_t = new text_t();
	t_t->position = DirectX::XMINT2(xPos, static_cast<int32_t>(y * maxSize));
	t_t->text = t;
	t_t->rows = rows;
	t_t->size = static_cast<int32_t>(y);

	text.push_back(t_t);

	for (int i = 0; i < text.size() - 1 && text.size() != 0; i++)
	{
		text[i]->position.y -= text[i + 1]->size;
		text[i]->text.setPosition(static_cast<float>(text[i]->position.x), static_cast<float>(text[i]->position.y));
	}
	
}
