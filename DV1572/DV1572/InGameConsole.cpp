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

void InGameConsole::update()
{	
	while (text.size() > maxSize)
	{
		delete text[0];
		text.erase(text.begin());
	}

	for (int i = 0; i < text.size() - 1; i++)
	{
		text[i]->position.y -= textHight * text[i + 1]->rows ;
		text[i]->text.setPosition(text[i]->position.x, text[i]->position.y);
	}
}

void InGameConsole::draw()
{
	for (auto & t : text)
		t->text.Draw();
}

void InGameConsole::pushString(const std::string & string)
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
	t.setPosition(startPosition.x, startPosition.y);
	t.setScale(.3f);
	

	text_t * t_t = new text_t();
	t_t->position = startPosition;
	t_t->text = t;
	t_t->rows = rows;

	text.push_back(t_t);
	update();
}
