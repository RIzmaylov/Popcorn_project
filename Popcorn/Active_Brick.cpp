#include "Active_Brick.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CActive_Brick::CActive_Brick() :
	Fade_Step(0)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Act(HWND hwnd)
{
	if (Fade_Step < Max_Fade_Step)
	{
		++Fade_Step;

		InvalidateRect(hwnd, &Brick_Rect, FALSE);
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
	HPEN pen;
	HBRUSH brush;

	CsConfig::Create_Pen_Brush(85 - Fade_Step * 85 / Max_Fade_Step, 255 - Fade_Step * 255 / Max_Fade_Step, 255 - Fade_Step * 255 / Max_Fade_Step, pen, brush);


	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	Brick_Rect.left = (CsConfig::Level_X_Offset + CsConfig::Cell_Width) * CsConfig::Global_Scale;
	Brick_Rect.top = (CsConfig::Level_Y_Offset + CsConfig::Cell_Height) * CsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + CsConfig::Brick_Width * CsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + CsConfig::Brick_Height * CsConfig::Global_Scale;


	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * CsConfig::Global_Scale, 2 * CsConfig::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
