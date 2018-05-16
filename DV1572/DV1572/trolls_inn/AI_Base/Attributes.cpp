#include "OurEngine.h"
#include "Attributes.h"

Attributes::Attributes()
{
	m_stat = 0.0f;
}

Attributes::Attributes(float stat) : m_stat(stat)
{
}

Attributes::~Attributes()
{
}

float Attributes::GetStat() const
{
	return m_stat;
}

void Attributes::SetStat(float stat)
{
	m_stat = stat;
	//m_stat = std::min(std::max(m_stat, -1.0f), 1.0f);
}

void Attributes::AddStat(float stat)
{
	m_stat += stat;
	//m_stat = std::min(std::max(m_stat, -1.0f), 1.0f);
}