#pragma once

class Attributes
{
private:
	float m_stat;

public:
	Attributes();
	Attributes(float stat);
	~Attributes();

	float GetStat() const;
	void SetStat(float stat);
	void AddStat(float stat);

};

//static Attributes elfAtr = { 1.0f };
//static Attributes dwarfAtr = { 0.0f };
