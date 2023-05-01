#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <cmath>


const int Timer_Id = WM_USER + 1;								// ID процесса юзера
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class EKey_Type
{
	Left,
	Right,
	Space
};

enum class ELetter_Type
{
	None,
	O
};

enum class EBrick_Type
{
	None,
	Red,
	Blue
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CsEngine
{
public:
	CsEngine();
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	static const int Level_Width = 12;																				// ширина уровня в КИРПИЧАХ
	static const int Level_Height = 14;																				// высота уровня в КИРПИЧАХ

private:
	void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
	void Redraw_Platform(); 
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
	void Draw_Level(HDC hdc);
	void Draw_Platform(HDC hdc, int x, int y);
	void Draw_Ball(HDC hdc, RECT& paint_area);
	void Draw_Border(HDC hdc, int x, int y, bool is_top_border);
	void Draw_Bounds(HDC hdc, RECT& paint_area);
	void Check_Level_Brick_Hit(int& next_y_pos);
	void Move_Ball();

	HWND Hwnd;
	HPEN BG_Pen, Letter_Pen, Hightlight_Pen, Brick_Red_pen, Brick_Blue_pen, Platform_Circle_pen, Platform_Inner_pen, 
		Ball_Pen, Border_Blue_Pen, Border_White_Pen;
	HBRUSH BG_Brush, Brick_Red_brush, Brick_Blue_brush, Platform_Circle_brush, Platform_Inner_brush, Ball_Brush, 
		Border_Blue_Brush, Border_White_Brush;

	int Inner_Width;																													// ширина платформы между шариками
	int Platform_X_Pos;																												// положение платформы по оси x
	int Platform_X_Step;																											// смещение платформы по оси x
	int Platform_Width;																												// ширина всей платформы (иеняется в зависимости от ситуации в игре)
	int Ball_X_Pos;																														// положение шарика по оси Х
	int Ball_Y_Pos;																														// положение шарика по оси У
	double Ball_Speed;																												// скорость смещения шарика
	double Ball_Direction;																										// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)

	RECT Platform_Rect, Prev_Platform_Rect;
	RECT Level_Rect;
	RECT Ball_Rect, Prev_Ball_Rect;


	static const int Global_Scale = 4;																				// глобальный масштаб всех элементов игры
	static const int Brick_Width = 15;																				// ширина элемента игры кирпич
	static const int Brick_Height = 7;																				// высота элемента игры кирпич
	static const int Cell_Width = Brick_Width + 1;														// ширина ячейки (кирпич + 1 пиксельный отступ)
	static const int Cell_Height = Brick_Height + 1;													// высота ячейки (кирпич + 1 пиксельный отступ)
	static const int Level_X_Offset = 8;																			// смещение игрового уровня по оси X от начала координат экрана
	static const int Level_Y_Offset = 6;																			// смещение игрового уровня по оси Y от начала координат экрана
	static const int Circle_Size = 7;																					// размер шарика платформы
	static const int Platform_Y_Pos = 185;																		// положение платформы по оси y
	static const int Platform_Height = 7;																			// высота всей платформы (не меняется)
	static const int Ball_Size = 4;																						// размер шарика
	static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;		// максимальноый Х уровня
	static const int Max_Y_Pos = 199 - Ball_Size;															// максимальный У уровня
	static const int Border_X_Offset = 6;																			// координаты минимальной границы рамки по Х
	static const int Border_Y_Offset = 4;																			// координаты минимальной границы рамки по У
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------