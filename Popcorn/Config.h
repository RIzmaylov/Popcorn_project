#pragma once

#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CColor
{
public: 
	CColor(unsigned char r, unsigned char g, unsigned char b);

	int Get_RGB() const;

	unsigned char R, G, B;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CsConfig
{
public:
	static void Setup_Colors();
	static void Create_Pen_Brush(const CColor& color, HPEN& pen, HBRUSH& brush);
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
	static int Rand(int range);

	static bool Level_Has_Floor;
	static int Current_Timer_Tick;

	static const CColor BG_Color, Red_Brick_Color, Blue_Brick_Color;

	static HPEN BG_Pen;
	static HBRUSH BG_Brush;
	static HWND Hwnd;

	static HPEN Brick_Red_pen, Brick_Blue_pen, Letter_Pen;
	static HBRUSH Brick_Red_brush, Brick_Blue_brush;

	static const double Moving_Step_Size;

	static const int Global_Scale = 4;																																// глобальный масштаб всех элементов игры
	static const int FPS = 20;																																				// FPS для таймера

	static const int Level_Width = 12;																																// ширина уровня в КИРПИЧАХ
	static const int Level_Height = 14;																																// высота уровня в КИРПИЧАХ
	static const int Level_X_Offset = 8;																															// смещение игрового уровня по оси X от начала координат экрана
	static const int Level_Y_Offset = 6;																															// смещение игрового уровня по оси Y от начала координат экрана
	static const int Brick_Width = 15;																																// ширина элемента игры кирпич
	static const int Brick_Height = 7;																																// высота элемента игры кирпич
	static const int Cell_Width = Brick_Width + 1;																										// ширина ячейки (кирпич + 1 пиксельный отступ)
	static const int Cell_Height = Brick_Height + 1;																									// высота ячейки (кирпич + 1 пиксельный отступ)

	static const int Border_X_Offset = 6;																															// координаты минимальной границы рамки по Х
	static const int Border_Y_Offset = 4;																															// координаты минимальной границы рамки по У
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;														// максимальноый Х уровня
	static const int Max_Y_Pos = 199;																																	// максимальный У уровня
	static const int Platform_Y_Pos = 185;																														// положение платформы по оси y
	static const int Max_Active_Bricks_Count = 10;																										
	static const int Hits_Per_Letter = 1;																															// Вероятность выбить букву = 1.0 / Hits_Per_Letter
	static const int Max_Falling_Letters_Count = 10;
};