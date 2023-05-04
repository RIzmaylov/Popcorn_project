﻿#include "Active_Brick.h"

HPEN CActive_Brick::Fading_Red_Brick_Pens[Max_Fade_Step];
HBRUSH CActive_Brick::Fading_Red_Brick_Brushes[Max_Fade_Step];
HPEN CActive_Brick::Fading_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH CActive_Brick::Fading_Blue_Brick_Brushes[Max_Fade_Step];


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CActive_Brick::CActive_Brick(EBrick_Type brick_type) :
	Fade_Step(0), Brick_Type(brick_type)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Act(HWND hwnd)
{
	if (Fade_Step < Max_Fade_Step - 1)
	{
		++Fade_Step;

		InvalidateRect(hwnd, &Brick_Rect, FALSE);
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
	HPEN pen = 0;
	HBRUSH brush = 0;


	switch (Brick_Type)
	{
	case EBrick_Type::Red:
		pen = Fading_Red_Brick_Pens[Fade_Step];
		brush = Fading_Red_Brick_Brushes[Fade_Step];
		break;

	case EBrick_Type::Blue:
		pen = Fading_Blue_Brick_Pens[Fade_Step];
		brush = Fading_Blue_Brick_Brushes[Fade_Step];
		break;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	Brick_Rect.left = (CsConfig::Level_X_Offset + CsConfig::Cell_Width) * CsConfig::Global_Scale;
	Brick_Rect.top = (CsConfig::Level_Y_Offset + CsConfig::Cell_Height) * CsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + CsConfig::Brick_Width * CsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + CsConfig::Brick_Height * CsConfig::Global_Scale;


	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * CsConfig::Global_Scale, 2 * CsConfig::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Setup_Colors()
{
	for (int i = 0; i < Max_Fade_Step; ++i)
	{
		Get_Fading_Color(CsConfig::Red_Brick_Color, i, Fading_Red_Brick_Pens[i], Fading_Red_Brick_Brushes[i]);
		Get_Fading_Color(CsConfig::Blue_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned char CActive_Brick::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Get_Fading_Color(const CColor& color, int step, HPEN& pen, HBRUSH& brush)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(color.R, CsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(color.G, CsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(color.B, CsConfig::BG_Color.B, step);

	CsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------