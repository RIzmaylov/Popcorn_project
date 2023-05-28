﻿#include "Level.h"

char CLevel::Level_01[CsConfig::Level_Height][CsConfig::Level_Width] = {												// массив игрового уровня
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

char CLevel::Test_Level[CsConfig::Level_Height][CsConfig::Level_Width] = {												// массив игрового уровня
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CLevel::CLevel() :
	//Active_Brick(EBrick_Type::Red),
	Brick_Red_pen(0),
	Brick_Blue_pen(0),
	Letter_Pen(0),
	Brick_Red_brush(0),
	Brick_Blue_brush(0),
	Level_Rect{}
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Check_Hit(double next_x_pos, double next_y_pos, CBall* ball)
{// Корректировки движения при отражении от кирпича

	// Объявление вспомогательных переменных для определения ближайших кирпичей для мячика для обсчета столкновения
	double direction;
	double min_ball_x,  max_ball_x, min_ball_y, max_ball_y;
	int min_level_x,  max_level_x, min_level_y, max_level_y;
	bool got_horizontal_hit, got_vertical_hit;
	double horizontal_reflection_pos, vertical_reflection_pos;

	if (next_y_pos > CsConfig::Level_Y_Offset + (CsConfig::Level_Height - 1) * CsConfig::Cell_Height + CsConfig::Brick_Height)
		return false;

	direction = ball->Get_Direction();

	min_ball_x = next_x_pos - ball->Radius;
	max_ball_x = next_x_pos + ball->Radius;
	min_ball_y = next_y_pos - ball->Radius;
	max_ball_y = next_y_pos + ball->Radius;

	min_level_x = static_cast<int>((min_ball_x - CsConfig::Level_X_Offset) / CsConfig::Cell_Width);
	max_level_x = static_cast<int>((max_ball_x - CsConfig::Level_X_Offset) / CsConfig::Cell_Width);
	min_level_y = static_cast<int>((min_ball_y - CsConfig::Level_Y_Offset) / CsConfig::Cell_Height);
	max_level_y = static_cast<int>((max_ball_y - CsConfig::Level_Y_Offset) / CsConfig::Cell_Height);

	for (int i = max_level_y; i >= min_level_y; --i)
	{
		Current_Brick_Top_Y = CsConfig::Level_Y_Offset + i * CsConfig::Cell_Height;
		Current_Brick_Low_Y = Current_Brick_Top_Y + CsConfig::Brick_Height;

		for (int j = min_level_x; j <= max_level_x; ++j)
		{
			if (Current_Level[i][j] == 0)
				continue;

			Current_Brick_Left_X = CsConfig::Level_X_Offset + j * CsConfig::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + CsConfig::Brick_Width;

			// Нахождение минимальных расстояний от центра шарика до горизонтальных и вертикальных граней кирпича
			// Если расстояние до горизонтальной грани меньше, то считаем сначала рефлект от горизонтальной, если до вертикальной - до вертикальной

			got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);
			got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);

			if (got_horizontal_hit && got_vertical_hit)
			{
				if (got_vertical_hit <= got_horizontal_hit)
					ball->Reflect(true);
				else
					ball->Reflect(false);
				return true;
			}
			else
			{
				if (got_horizontal_hit)
				{
					ball->Reflect(false);
					return true;
				}
				else if (got_vertical_hit)
				{
					ball->Reflect(true);
					return true;
				}
			}

			//if (Is_Check_Horizontal_First(next_x_pos, next_y_pos))
			//{
			//	if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
			//		return true;
			//	if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
			//		return true;
			//}
			//else
			//{
			//	if (Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball))
			//		return true;
			//	if (Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball))
			//		return true;
			//}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, CsConfig::Global_Scale, RGB(255, 255, 255));

	// создание кистей и карандашей
	CsConfig::Create_Pen_Brush(CsConfig::Red_Brick_Color, Brick_Red_pen, Brick_Red_brush);
	CsConfig::Create_Pen_Brush(CsConfig::Blue_Brick_Color, Brick_Blue_pen, Brick_Blue_brush);

	// инициализация габаритов игрового уровня
	Level_Rect.left = CsConfig::Level_X_Offset * CsConfig::Global_Scale;
	Level_Rect.top = CsConfig::Level_Y_Offset * CsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + CsConfig::Cell_Width * CsConfig::Level_Width * CsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + CsConfig::Cell_Height * CsConfig::Level_Height * CsConfig::Global_Scale;

	memset(Current_Level, 0, sizeof(Current_Level));

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Set_Current_Level(char level[CsConfig::Level_Height][CsConfig::Level_Width])
{
	memcpy(Current_Level, level, sizeof(Current_Level));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw(HDC hdc, RECT& paint_area)
{	// Отрисовка уровня игры
	RECT  intersection_rect;																								// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))				// определение пересечения области рисования и перерисовки кадра
		return;

	for (int i = 0; i < CsConfig::Level_Height; ++i) 
		for (int j = 0; j < CsConfig::Level_Width; ++j) 
			Draw_Brick(hdc, CsConfig::Level_X_Offset + j * CsConfig::Cell_Width, CsConfig::Level_Y_Offset + i * CsConfig::Cell_Height, static_cast<EBrick_Type>(Current_Level[i][j]));
	
	//Active_Brick.Draw(hdc, paint_area);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
//bool CLevel::Is_Check_Horizontal_First(double next_x_pos, double next_y_pos)
//{
//	double min_distance_to_horizontal, another_min_distance, min_distance_to_vertical;
//
//	// Нахождение минимальных расстояний от центра шарика до горизонтальных и вертикальных граней кирпича
//	min_distance_to_horizontal = fabs(next_x_pos - Current_Brick_Left_X);
//	another_min_distance = fabs(next_x_pos - Current_Brick_Right_X);
//
//	if (another_min_distance < min_distance_to_horizontal)
//		min_distance_to_horizontal = another_min_distance;
//
//	min_distance_to_vertical = fabs(next_y_pos - Current_Brick_Top_Y);
//	another_min_distance = fabs(next_y_pos - Current_Brick_Low_Y);
//
//	if (another_min_distance < min_distance_to_vertical)
//		min_distance_to_vertical = another_min_distance;
//
//	// Если расстояние до горизонтальной грани меньше, то считаем сначала рефлект от горизонтальной, если до вертикальной - до вертикальной
//	if (min_distance_to_horizontal <= min_distance_to_vertical)
//		return true;
//	else
//		return false;
//}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos)
{
	double direction = ball->Get_Direction();

	// Проверка попадания в нижнюю грань кирпича
	if (direction >= 0.0 && direction < M_PI)
	{
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{

			// Проверка возможности отскока вниз
			if (level_y < CsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0)
			{
				//ball->Reflect(true);
				return true;
			}
			else
				return false;
		}
	}
	// Проверка попадания в верхнюю грань кирпича
	if (direction >= M_PI && direction <= 2.0 * M_PI)
	{
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{
			// Проверка возможности отскока вверх
			if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
			{
				//ball->Reflect(true);
				return true;
			}
			else
				return false;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos)
{
	double direction = ball->Get_Direction();

	// Проверка попадания в левую грань кирпича
	if (direction >= 0.0 && direction <= M_PI_2 || direction >= M_PI + M_PI_2 && direction <= 2.0 * M_PI)
	{
		if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{
			// Проверка возможности отскока влево
			if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
			{
				//ball->Reflect(false);
				return true;
			}
			else
				return false;
		}
	}
	// Проверка попадания в правую грань кирпича
	if (direction > M_PI_2 && direction < M_PI + M_PI_2)
	{
		if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{
			// Проверка возможности отскока вправо
			if (level_x < CsConfig::Level_Width && Current_Level[level_y][level_x + 1] == 0)
			{
				//ball->Reflect(false);
				return true;
			}
			else
				return false;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{// функция проверяет пересечение горизонтального отрезка (проходящего от left_x до right_x через y) с окружностью радиусом radius
	// x * x + y * y = R * R - формула окружности для определения пересечения мячика с кирпичем
	double min_x, max_x;

	if (y > radius)
		return false;

	x = sqrt(radius * radius - y * y);

	min_x = next_x_pos - x;
	max_x = next_x_pos + x;

	if (max_x >= left_x && max_x <= right_x || min_x >= left_x && min_x <= right_x)
		return true;
	else
		return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
	RoundRect(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + CsConfig::Brick_Width) * CsConfig::Global_Scale, (y + CsConfig::Brick_Height) * CsConfig::Global_Scale, 
		2 * CsConfig::Global_Scale, 2 * CsConfig::Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush) 
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
void CLevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step) 
{// Вывод вращения падающего кирпича

	bool switch_color;
	double offset = 0.0;
	double rotation_angle = 0.0; //Преорабзование шага в угол поворота
	int brick_half_height = CsConfig::Brick_Height * CsConfig::Global_Scale / 2;
	int back_part_offset = 0;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;

	XFORM xform, old_xform;

	if (brick_type != EBrick_Type::Blue && brick_type != EBrick_Type::Red) 
	{
		return; // Только голубые или красные кирпичи
	}

	// Корректировка шага вращения и угла поворота
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

		Rectangle(hdc, x, y + brick_half_height - CsConfig::Global_Scale, x + CsConfig::Brick_Width * CsConfig::Global_Scale, y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + CsConfig::Brick_Width * CsConfig::Global_Scale, y + brick_half_height + CsConfig::Global_Scale - 1);
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

		offset = 3.0 * (1.0 - abs(xform.eM22)) * static_cast<double>(CsConfig::Global_Scale);
		back_part_offset = static_cast<int>(round(offset));
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, CsConfig::Brick_Width * CsConfig::Global_Scale, brick_half_height - back_part_offset);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, CsConfig::Brick_Width * CsConfig::Global_Scale, brick_half_height);
		if (rotation_step > 4 && rotation_step < 13)
		{
			if (letter_type == ELetter_Type::O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + 5 * CsConfig::Global_Scale, -5 * CsConfig::Global_Scale / 2, 0 + 10 * CsConfig::Global_Scale, 5 * CsConfig::Global_Scale / 2);
			}
		}


		SetWorldTransform(hdc, &old_xform);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
