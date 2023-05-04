#pragma once

#include "Config.h"

class CActive_Brick
{
public:
	CActive_Brick();
	void Act(HWND hwnd);
	void Draw(HDC hdc , RECT& paint_area);

	RECT Brick_Rect;

	int Fade_Step;
	static const int Max_Fade_Step = 40;

};