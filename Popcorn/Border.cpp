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
void CsBorder::Draw(HDC hdc,  RECT& paint_area) 
{	// Отрисовка рамки из тайлов

	// 1. Отрисовка левой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 2, 1 + i * 4, false);
	}

	// 2. Отрисовка правой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 201, 1 + i * 4, false);
	}

	// 3. Отрисовка верхней рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 3 + i * 4, 0, true);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsBorder::Check_Hit(double next_x_pos, double next_y_pos, CBall* ball)
{
	bool got_hit = false;
	if (next_x_pos - ball->Radius < CsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}
	if (next_y_pos - ball->Radius < CsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect(true);
	}

	if (next_x_pos + ball->Radius > CsConfig::Max_X_Pos + 1)
	{
		got_hit = true;
		ball->Reflect(false);
	}
	if (next_y_pos + ball->Radius > CsConfig::Max_Y_Pos)
	{
		if (CsConfig::Level_Has_Floor)
		{
			got_hit = true;
			ball->Reflect(true);
		}
		else
		{
			if (next_y_pos + ball->Radius > CsConfig::Max_Y_Pos + ball->Radius * 4.0)											// Проверка нижней границы с учетом размера шарика
				ball->Set_State(EBall_State::Lost, next_x_pos);
		}
	}
	return got_hit;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border) 
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
	SelectObject(hdc, CsConfig::BG_Pen);
	SelectObject(hdc, CsConfig::BG_Brush);

	if (is_top_border)
		Rectangle(hdc, (x + 2) * CsConfig::Global_Scale, (y + 2) * CsConfig::Global_Scale, (x + 3) * CsConfig::Global_Scale, (y + 3) * CsConfig::Global_Scale);
	else
		Rectangle(hdc, (x + 2) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 3) * CsConfig::Global_Scale, (y + 2) * CsConfig::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
