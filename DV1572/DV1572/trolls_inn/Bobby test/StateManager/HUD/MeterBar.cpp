#include "OurEngine.h"
#include "MeterBar.h"

MeterBar::MeterBar()
{
	m_background = nullptr;
	m_needle = nullptr;
}

MeterBar::~MeterBar()
{
	delete m_background;
	m_background = nullptr;
	delete m_needle;
	m_needle = nullptr;
	for (size_t i = 0; i < m_texts.size(); i++)
		delete m_texts[i];
	m_texts.clear();
}

void MeterBar::setBackground(RectangleShape * rect)
{
	m_background = rect;
}

void MeterBar::setNeedle(RectangleShape * rect)
{
	m_needle = rect;
}

void MeterBar::AddText(Text * t)
{
	m_texts.push_back(t);
}

void MeterBar::setNeedlePosXY(DirectX::XMFLOAT2 needlePos)
{
	DirectX::XMFLOAT3 p = m_background->getScreenPos();
	float x = m_background->getWidth();
	float y = m_background->getHeight();
	
	if (needlePos.x < p.x)
		needlePos.x = p.x;
	else if (needlePos.x > p.x + x)
		needlePos.x = p.x + x;

	if (needlePos.y < p.y)
		needlePos.y = p.y;
	else if (needlePos.y > p.y + y)
		needlePos.y = p.y + y;
	
	needlePos.x -= m_needle->getWidth() / 2;
	needlePos.y -= m_needle->getHeight() / 2;

	m_needle->setScreenPos(needlePos.x, needlePos.y);
}

void MeterBar::setNeedlePosHorizontal(float pHor)
{
	DirectX::XMFLOAT3 p = m_background->getScreenPos();
	float x = m_background->getWidth();

	if (pHor < p.x)
		pHor = p.x;
	else if (pHor > p.x + x)
		pHor = p.x + x;

	pHor -= m_needle->getWidth() / 2;

	m_needle->setScreenPos(pHor, m_needle->getScreenPos().y);
}

void MeterBar::setNeedlePosVertical(float pVer)
{
	DirectX::XMFLOAT3 p = m_background->getScreenPos();
	float y = m_background->getHeight();

	if (pVer < p.y)
		pVer = p.y;
	else if (pVer > p.y + y)
		pVer = p.y + y;

	pVer -= m_needle->getHeight() / 2;
	m_needle->setScreenPos(m_needle->getScreenPos().x, pVer);
}

void MeterBar::SlideNeedleHorizontalBasedOnPrecentage(float hor)
{
	float bX = m_background->getScreenPos().x;
	float bW = m_background->getWidth();
	float bM = (bX + bX + bW) * 0.5f;
	float needlePos = bM + (bW * 0.5f * hor);
	needlePos -= m_needle->getWidth() / 2;
	m_needle->setScreenPos(needlePos, m_needle->getScreenPos().y);
}

void MeterBar::SlideNeedleVerticalBasedOnPrecentage(float ver)
{
	float bY = m_background->getScreenPos().y;
	float bH = m_background->getHeight();
	float bM = (bY + bY + bH) * 0.5f;
	float needlePos = bM + (bH * 0.5f * ver);
	needlePos -= m_needle->getHeight() / 2;
	m_needle->setScreenPos(m_needle->getScreenPos().x, needlePos);
}

int MeterBar::getNeedleIndex() const
{
	return m_needle->getIndex();
}

RectangleShape * MeterBar::getNeedle() const
{
	return m_needle;
}

DirectX::XMFLOAT2 MeterBar::getNeedleTarget() const
{
	DirectX::XMFLOAT3 temp = m_background->getScreenPos();
	DirectX::XMFLOAT2 bPos(temp.x, temp.y);
	DirectX::XMFLOAT2 bSize(m_background->getWidth(), m_background->getHeight());
	DirectX::XMFLOAT2 bMid((bPos.x + bPos.x + bSize.x) * 0.5f, (bPos.y + bPos.y + bSize.y) * 0.5f);

	temp = m_needle->getScreenPos();
	DirectX::XMFLOAT2 nPos(temp.x, temp.y);

	DirectX::XMFLOAT2 target;
	target.x = (nPos.x - bMid.x) / (bSize.x * 0.5f);
	target.y = (nPos.y - bMid.y) / (bSize.y * 0.5f);

	return target;
}

void MeterBar::Draw()
{
	m_background->DrawAsHud();
	m_needle->DrawAsHud();

	for (size_t i = 0; i < m_texts.size(); i++)
		m_texts[i]->Draw();
}
