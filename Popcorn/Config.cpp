#include "Config.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CColor::CColor(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}

bool CsConfig::Level_Has_Floor = true;
int CsConfig::Current_Timer_Tick = 0;
const CColor CsConfig::BG_Color(15, 63, 31);
const CColor CsConfig::Red_Brick_Color(255,85,85);
const CColor CsConfig::Blue_Brick_Color(85, 255, 255);

HPEN CsConfig::BG_Pen;
HBRUSH CsConfig::BG_Brush;
HWND CsConfig::Hwnd;

HPEN CsConfig::Brick_Red_pen, CsConfig::Brick_Blue_pen,CsConfig::Letter_Pen;
HBRUSH CsConfig::Brick_Red_brush, CsConfig::Brick_Blue_brush;

const double CsConfig::Moving_Step_Size = 1.0 / CsConfig::Global_Scale;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsConfig::Setup_Colors()
{
	CsConfig::Letter_Pen = CreatePen(PS_SOLID, CsConfig::Global_Scale, RGB(255, 255, 255));

	// создание кистей и карандашей
	CsConfig::Create_Pen_Brush(CsConfig::BG_Color, BG_Pen, BG_Brush);
	CsConfig::Create_Pen_Brush(CsConfig::Red_Brick_Color, CsConfig::Brick_Red_pen, CsConfig::Brick_Red_brush);
	CsConfig::Create_Pen_Brush(CsConfig::Blue_Brick_Color, CsConfig::Brick_Blue_pen, CsConfig::Brick_Blue_brush);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsConfig::Rand(int range)
{// Вычисление псевдослучайного числа в диапазоне [0, range - 1]
	return rand() * range / RAND_MAX;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsConfig::Create_Pen_Brush(const CColor& color, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

