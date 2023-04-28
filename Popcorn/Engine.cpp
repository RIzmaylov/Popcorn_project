#include "Engine.h"

#define _USE_MATH_DEFINES
#include <cmath>

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

HWND Hwnd;
HPEN BG_Pen, Letter_Pen, Hightlight_Pen, Brick_Red_pen, Brick_Blue_pen, Platform_Circle_pen, Platform_Inner_pen;
HBRUSH BG_Brush, Brick_Red_brush, Brick_Blue_brush, Platform_Circle_brush, Platform_Inner_brush;

const int Global_Scale = 4;											// глобальный масштаб всех элементов игры
const int Brick_Width = 15;											// ширина элемента игры кирпич
const int Brick_Height = 7;											// высота элемента игры кирпич
const int Cell_Width = Brick_Width + 1;					// ширина ячейки (кирпич + 1 пиксельный отступ)
const int Cell_Height = Brick_Height + 1;				// высота ячейки (кирпич + 1 пиксельный отступ)
const int Level_X_Offset = 8;										// смещение игрового уровня по оси X от начала координат экрана
const int Level_Y_Offset = 6;										// смещение игрового уровня по оси Y от начала координат экрана
const int Level_Width = 14;											// ширина уровня в КИРПИЧАХ
const int Level_Height = 12;										// высота уровня в КИРПИЧАХ
const int Circle_Size = 7;											// размер шарика платформы
const int Platform_Y_Pos = 185;									// положение платформы по оси y
const int Platform_Height = 7;									// высота всей платформы (не меняется)

int Inner_Width = 21;														// ширина платформы между шариками
int Platform_X_Pos = 0;													// положение платформы по оси x
int Platform_X_Step = Global_Scale * 2;					// смещение платформы по оси x
int Platform_Width = 28;												// ширина всей платформы (иеняется в зависимости от ситуации в игре)


RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

char Level_01[14][12] = {												// массив игрового уровня
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Redraw_Platform() 
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = (Level_X_Offset + Platform_X_Pos) * Global_Scale;
	Platform_Rect.top = Platform_Y_Pos * Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

	InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hwnd)
{	//Настройка игры при старте

	Hwnd = hwnd;

	Hightlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));

	// создание кистей и карандашей
	Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
	Create_Pen_Brush(255, 85, 85, Brick_Red_pen, Brick_Red_brush);
	Create_Pen_Brush(85, 255, 255, Brick_Blue_pen, Brick_Blue_brush);
	Create_Pen_Brush(151, 0, 0, Platform_Circle_pen, Platform_Circle_brush);
	Create_Pen_Brush(0, 128, 192, Platform_Inner_pen, Platform_Inner_brush);

	// инициализация габаритов игрового уровня
	Level_Rect.left = Level_X_Offset * Global_Scale;
	Level_Rect.top = Level_Y_Offset * Global_Scale;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Global_Scale;

	// очищение области платформы при первом запуске (чтобы не оставался след при первом перемещении платформы)
	Redraw_Platform();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{	//Отрисовка элемента игры - кирпича

	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBrick_Type::None: return;
	case EBrick_Type::Red:
		pen = Brick_Red_pen;
		brush = Brick_Red_brush;
		break;
	case EBrick_Type::Blue:
		pen = Brick_Blue_pen;
		brush = Brick_Blue_brush;
		break;
	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 
		2 * Global_Scale, 2 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush) 
{
	if (is_switch_color) 
	{
		front_pen = Brick_Red_pen;
		front_brush = Brick_Red_brush;

		back_pen = Brick_Blue_pen;
		back_brush = Brick_Blue_brush;
	}
	else 
	{
		front_pen = Brick_Blue_pen;
		front_brush = Brick_Blue_brush;

		back_pen = Brick_Red_pen;
		back_brush = Brick_Red_brush;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step) 
{// Вывод вращения падающего кирпича

	bool switch_color;
	double offset = 0.0;
	double rotation_angle = 0.0; //Преорабзование шага в угол поворота
	int brick_half_height = Brick_Height * Global_Scale / 2;
	int back_part_offset = 0;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;

	XFORM xform, old_xform;

	if (brick_type != EBrick_Type::Blue && brick_type != EBrick_Type::Red) 
	{
		return; // Только голубые или красные кирпичи
	}

	rotation_step = rotation_step % 16; // Делаю так, чтобы шагов было всегда меньше 16

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI / 16.0 * static_cast<double>(rotation_step);
	else
		rotation_angle = 2.0 * M_PI / 16.0 * static_cast<double>(8 - rotation_step);

	if (rotation_step > 4 && rotation_step < 13) // с 5 по 12 шаги цвет основного цвета кирпича должен быть красный, в остальном случае синий
	{
		switch_color = (brick_type == EBrick_Type::Blue); // Если цвет кирпича синий, то меняем цвет переворота
	}
	else 
	{
		switch_color = (brick_type == EBrick_Type::Red);  // Если цвет кирпича красный, то меняем цвет кирпича
	}

	Set_Brick_Letter_Colors(switch_color, brick_type, front_pen, back_pen, front_brush, back_brush);

	if (rotation_step == 4 || rotation_step == 12) 
	{
		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
	}
	else {
		SetGraphicsMode(hdc, GM_ADVANCED);

		// Настраивание матрицы переворота кирпича
		xform.eM11 = 1.0f;
		xform.eM12 = 0.0f;
		xform.eM21 = 0.0f;
		xform.eM22 = static_cast<float>(cos(rotation_angle));
		xform.eDx  = static_cast<float>(x);
		xform.eDy  = static_cast<float>(y + brick_half_height);

		GetWorldTransform(hdc, &old_xform);
		SetWorldTransform(hdc, &xform);

		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = 3.0 * (1.0 - abs(xform.eM22)) * static_cast<double>(Global_Scale);
		back_part_offset = static_cast<int>(round(offset));
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);
		if (rotation_step > 4 && rotation_step < 13)
		{
			if (letter_type == ELetter_Type::O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + 5 * Global_Scale, -5 * Global_Scale / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
			}
		}


		SetWorldTransform(hdc, &old_xform);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{	//Отрисовка уровня игры
	for (int i = 0; i < 14; ++i) 
		for (int j = 0; j < 12; ++j) 
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(Level_01[i][j]));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
{	//Отрисовка элемента игры Платформа

	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);

	// 2. Рисуем блик на левом шарике
	SelectObject(hdc, Hightlight_Pen);

	Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale, 
					 (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);
	Arc(hdc, (x + 1) * Global_Scale + 1, (y + 1) * Global_Scale + 1, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale, 
					 (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);

	// 3. Рисуем платформу между шариками
	SelectObject(hdc, Platform_Inner_pen);
	SelectObject(hdc, Platform_Inner_brush);

	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 4 * Global_Scale, 4 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc, RECT& paint_area) 
{	//Отрисовка экрана игры

	RECT intersection_rect;									// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))				// определение пересечения области рисования и перерисовки кадра
	{
		Draw_Level(hdc);
	}
	if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))				// определение пересечения области рисования и перерисовки кадра
	{
		Draw_Platform(hdc, Level_X_Offset + Platform_X_Pos, Platform_Y_Pos);
	}


	//for (int i = 0; i < 16; ++i) {
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
	//}
}    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type) 
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform_X_Pos -= Platform_X_Step;
		Redraw_Platform();
		break;

	case EKey_Type::Right:
		Platform_X_Pos += Platform_X_Step;
		Redraw_Platform();
		break;

	case EKey_Type::Space:
		break;
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
