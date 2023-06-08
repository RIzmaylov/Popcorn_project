#include "Falling_Letter.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CFalling_Letter::CFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y) :
	Brick_Type(brick_type), Letter_Type(letter_type), Got_Hit(false), X(x), Y(y), Rotation_Step(2), Next_Rotation_Tick(CsConfig::Current_Timer_Tick + Ticks_Per_Step)
{
	Letter_Cell.left = X;
	Letter_Cell.top = Y;
	Letter_Cell.right = Letter_Cell.left + CsConfig::Brick_Width * CsConfig::Global_Scale;
	Letter_Cell.bottom = Letter_Cell.top + CsConfig::Brick_Height * CsConfig::Global_Scale;

	Prev_Letter_Cell = Letter_Cell;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFalling_Letter::Act()
{
	Prev_Letter_Cell = Letter_Cell;

	Y += CsConfig::Global_Scale;
	Letter_Cell.top += CsConfig::Global_Scale;
	Letter_Cell.bottom += CsConfig::Global_Scale;

	if (CsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
	{
		++Rotation_Step;
		Next_Rotation_Tick += Ticks_Per_Step;
	}

	InvalidateRect(CsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
	InvalidateRect(CsConfig::Hwnd, &Letter_Cell, FALSE);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))					// определение пересечения области рисования и перерисовки кадра
	{
		// 1. Очищаем фон
		SelectObject(hdc, CsConfig::BG_Pen);
		SelectObject(hdc, CsConfig::BG_Brush);

		Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right - 1, Prev_Letter_Cell.bottom - 1);
	}

	if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))								// определение пересечения области рисования и перерисовки кадра
		Draw_Brick_Letter(hdc);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CFalling_Letter::Is_Finished()
{
	if (Got_Hit || Letter_Cell.top >= CsConfig::Max_Y_Pos * CsConfig::Global_Scale)
		return true;
	else
		return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush) 
{
	if (is_switch_color) 
	{
		front_pen = CsConfig::Brick_Red_pen;
		front_brush = CsConfig::Brick_Red_brush;

		back_pen = CsConfig::Brick_Blue_pen;
		back_brush = CsConfig::Brick_Blue_brush;
	}
	else 
	{
		front_pen = CsConfig::Brick_Blue_pen;
		front_brush = CsConfig::Brick_Blue_brush;

		back_pen = CsConfig::Brick_Red_pen;
		back_brush = CsConfig::Brick_Red_brush;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFalling_Letter::Draw_Brick_Letter(HDC hdc) 
{// Вывод вращения падающего кирпича

	bool switch_color;
	double offset = 0.0;
	double rotation_angle = 0.0; //Преорабзование шага в угол поворота
	int brick_half_height = CsConfig::Brick_Height * CsConfig::Global_Scale / 2;
	int back_part_offset = 0;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;

	XFORM xform, old_xform;

	if (Brick_Type != EBrick_Type::Blue && Brick_Type != EBrick_Type::Red) 
	{
		return; // Только голубые или красные кирпичи
	}

	// Корректировка шага вращения и угла поворота
	Rotation_Step = Rotation_Step % 16; // Делаю так, чтобы шагов было всегда меньше 16

	if (Rotation_Step < 8)
		rotation_angle = 2.0 * M_PI / 16.0 * static_cast<double>(Rotation_Step);
	else
		rotation_angle = 2.0 * M_PI / 16.0 * static_cast<double>(8 - Rotation_Step);

	if (Rotation_Step > 4 && Rotation_Step < 13) // с 5 по 12 шаги цвет основного цвета кирпича должен быть красный, в остальном случае синий
	{
		switch_color = (Brick_Type == EBrick_Type::Blue); // Если цвет кирпича синий, то меняем цвет переворота
	}
	else 
	{
		switch_color = (Brick_Type == EBrick_Type::Red);  // Если цвет кирпича красный, то меняем цвет кирпича
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, back_pen, front_brush, back_brush);

	if (Rotation_Step == 4 || Rotation_Step == 12) 
	{
		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, X, Y + brick_half_height - CsConfig::Global_Scale, X + CsConfig::Brick_Width * CsConfig::Global_Scale, Y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, X, Y + brick_half_height, X + CsConfig::Brick_Width * CsConfig::Global_Scale, Y + brick_half_height + CsConfig::Global_Scale - 1);
	}
	else {

		// Настраивание матрицы переворота кирпича
		xform.eM11 = 1.0f;
		xform.eM12 = 0.0f;
		xform.eM21 = 0.0f;
		xform.eM22 = static_cast<float>(cos(rotation_angle));
		xform.eDx  = static_cast<float>(X);
		xform.eDy  = static_cast<float>(Y + brick_half_height);

		GetWorldTransform(hdc, &old_xform);
		SetWorldTransform(hdc, &xform);

		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = 3.0 * (1.0 - abs(xform.eM22)) * static_cast<double>(CsConfig::Global_Scale);
		back_part_offset = static_cast<int>(round(offset));
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, CsConfig::Brick_Width * CsConfig::Global_Scale, brick_half_height - back_part_offset);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, CsConfig::Brick_Width * CsConfig::Global_Scale, brick_half_height);
		if (Rotation_Step > 4 && Rotation_Step < 13)
		{
			if (Letter_Type == ELetter_Type::O)
			{
				SelectObject(hdc, CsConfig::Letter_Pen);
				Ellipse(hdc, 0 + 5 * CsConfig::Global_Scale, -5 * CsConfig::Global_Scale / 2, 0 + 10 * CsConfig::Global_Scale, 5 * CsConfig::Global_Scale / 2);
			}
		}


		SetWorldTransform(hdc, &old_xform);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

