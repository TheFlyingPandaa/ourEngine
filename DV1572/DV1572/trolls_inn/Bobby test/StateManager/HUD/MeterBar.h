#pragma once
#include "../../../../ourEngine/interface/Interface.h"

class MeterBar
{
private:
	RectangleShape*		m_background;
	RectangleShape*		m_needle;
	std::vector<Text*>	m_texts;

public:
	MeterBar();
	~MeterBar();
	void setBackground(RectangleShape * rect);
	void setNeedle(RectangleShape * rect);
	void AddText(Text * t);
	void setNeedlePosXY(DirectX::XMFLOAT2 needlePos);
	void setNeedlePosHorizontal(float hor);
	void setNeedlePosVertical(float ver);
	void SlideNeedleHorizontalBasedOnPrecentage(float hor);
	void SlideNeedleVerticalBasedOnPrecentage(float ver);
	int getNeedleIndex() const;
	RectangleShape* getNeedle() const;

	DirectX::XMFLOAT2 getNeedleTarget() const;

	void Draw();
};