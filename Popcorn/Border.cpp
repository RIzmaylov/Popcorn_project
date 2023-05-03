#include "Border.h"



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsBorder::CsBorder() :
	Border_Blue_Pen(0),
	Border_White_Pen(0),
	Border_Blue_Brush(0),
	Border_White_Brush(0)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Init()
{
	CsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
	CsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Draw(HDC hdc,  RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush) 
{	// Отрисовка рамки из тайлов

	// 1. Отрисовка левой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 2, 1 + i * 4, false, bg_pen, bg_brush);
	}

	// 2. Отрисовка правой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 201, 1 + i * 4, false, bg_pen, bg_brush);
	}

	// 3. Отрисовка верхней рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border, HPEN bg_pen, HBRUSH bg_brush) 
{	//Отрисовка одного тайла рамки игрового поля

	// Выводится основная линия рамки
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (is_top_border)
		Rectangle(hdc, x * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 4) * CsConfig::Global_Scale, (y + 4) * CsConfig::Global_Scale);
	else
		Rectangle(hdc, (x + 1) * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + 4) * CsConfig::Global_Scale, (y + 4) * CsConfig::Global_Scale);

	// Выводится дополнительная линия рамки
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (is_top_border)
		Rectangle(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + 4) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale);
	else 
		Rectangle(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + 1) * CsConfig::Global_Scale, (y + 4) * CsConfig::Global_Scale);


	// Выводится однопиксельная перфорация рамки
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	if (is_top_border)
		Rectangle(hdc, (x + 2) * CsConfig::Global_Scale, (y + 2) * CsConfig::Global_Scale, (x + 3) * CsConfig::Global_Scale, (y + 3) * CsConfig::Global_Scale);
	else
		Rectangle(hdc, (x + 2) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 3) * CsConfig::Global_Scale, (y + 2) * CsConfig::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
