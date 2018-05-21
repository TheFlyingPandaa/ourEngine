#include "InGameConsole.h"

std::vector < InGameConsole::text_t *> InGameConsole::text;
std::vector < InGameConsole::text_t *> InGameConsole::textEvent;



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
	for (int i = 0; i < textEvent.size(); i++)
	{
		delete textEvent[i];
	}
	textEvent.clear();
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
	/*while (textEvent.size() > maxSize)
	{
		delete textEvent[0];
		textEvent.erase(textEvent.begin());
	}

	for (int i = 0; i < textEvent.size(); i++)
	{
		textEvent[i]->timer += DeltaTime;
		if (textEvent[i]->timer > startFade)
		{
			textEvent[i]->text.setColor(1.0f, 1.0f, 1.0f, 1.0f - static_cast<float>((textEvent[i]->timer - startFade) / fadeTime));
		}

	}*/


}

void InGameConsole::draw()
{
	for (auto & t : text)
		t->text.Draw();
	for (auto & t : textEvent)
		t->text.Draw();
}

void InGameConsole::pushString(const std::string & string, float r , float g , float b)
{
	std::stringstream ss;
	int rows = 1;
	int lastRows = 1;
	for (int i = 0; i < string.length(); i++)
	{
		if (string[i] == '\n')
			rows++;
		if (i % maxWidth == 0 && i != 0)
		{
			if (lastRows == rows)
			{
				rows++;
				lastRows++;
				ss << "\n";
			}
			else
				lastRows++;
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

void InGameConsole::pushStringEvent(const std::string & string, float r, float g, float b)
{
	std::stringstream ss;
	int rows = 1;
	int lastRows = 1;
	for (int i = 0; i < string.length(); i++)
	{
		if (string[i] == '\n')
			rows++;
		if (i % maxWidth == 0 && i != 0)
		{
			if (lastRows == rows)
			{
				rows++;
				lastRows++;
				ss << "\n";
			}
			else
				lastRows++;
		}
		ss << string[i];
	}

	Text  t;
	t.setTextString(ss.str());
	t.setAllignment(TXT::ALLIGN::Left);
	t.setRelative(Text::RelativeTo::TL);
	t.setColor(r, g, b);
	t.setScale(.5f);

	XMVECTOR size = Text::getStringSize(&t);
	float y = DirectX::XMVectorGetY(size);
	y *= .5f;

	float x = DirectX::XMVectorGetX(size);
	x *= .5f;

	t.setPosition(static_cast<float>(0), y);


	text_t * t_t = new text_t();
	t_t->position = DirectX::XMINT2(0, static_cast<int32_t>(y));
	t_t->text = t;
	t_t->rows = rows;
	t_t->size = static_cast<int32_t>(y);

	textEvent.push_back(t_t);

	for (int i = 0; i < textEvent.size() - 1 && textEvent.size() != 0; i++)
	{
		textEvent[i]->position.y += textEvent[i + 1]->size;
		textEvent[i]->text.setPosition(static_cast<float>(textEvent[i]->position.x), static_cast<float>(textEvent[i]->position.y));
	}

}

void InGameConsole::RemoveStringEvent()
{
	if (!textEvent.empty())
	{
		for (int i = 0; i < textEvent.size(); i++)
		{
			delete textEvent[i];
		}
		textEvent.clear();
	}
}
