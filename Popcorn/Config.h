#pragma once

#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

class CsConfig
{
public:
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

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

	static const int Ball_Size = 4;																																		// размер шарика

	static const int Border_X_Offset = 6;																															// координаты минимальной границы рамки по Х
	static const int Border_Y_Offset = 4;																															// координаты минимальной границы рамки по У
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;														// максимальноый Х уровня
	static const int Max_Y_Pos = 199 - Ball_Size;																											// максимальный У уровня
	static const int Platform_Y_Pos = 185;																														// положение платформы по оси y

};