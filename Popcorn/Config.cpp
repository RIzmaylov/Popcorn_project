#include "Config.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CColor::CColor(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}

const CColor CsConfig::BG_Color(15, 63, 31);
const CColor CsConfig::Red_Brick_Color(255,85,85);
const CColor CsConfig::Blue_Brick_Color(85, 255, 255);

HPEN CsConfig::BG_Pen;
HBRUSH CsConfig::BG_Brush;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsConfig::Setup_Colors()
{
	// создание кистей и карандашей
	CsConfig::Create_Pen_Brush(CsConfig::BG_Color, BG_Pen, BG_Brush);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

