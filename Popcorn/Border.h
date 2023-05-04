﻿#pragma once
#include "Config.h"


class CsBorder
{
public:
	CsBorder();
	void Init();

	void Draw(HDC hdc, RECT& paint_area);

private:
	void Draw_Element(HDC hdc, int x, int y, bool is_top_border);


	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;

};
