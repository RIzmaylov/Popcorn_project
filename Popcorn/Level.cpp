#include "Level.h"

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

		Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
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

char CLevel::Test_Level[CsConfig::Level_Height][CsConfig::Level_Width] = {											// массив игрового уровня
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
	Level_Rect{},
	Active_Bricks_Count(0),
	Falling_Letter_Count(0)
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

	if (next_y_pos + ball->Radius > CsConfig::Level_Y_Offset + (CsConfig::Level_Height - 1) * CsConfig::Cell_Height + CsConfig::Brick_Height)
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
				On_Hit(j, i);
				return true;
			}
			else
			{
				if (got_horizontal_hit)
				{
					ball->Reflect(false);
					On_Hit(j, i);
					return true;
				}
				else 
					if (got_vertical_hit)
					{
						ball->Reflect(true);
						On_Hit(j, i);
						return true;
					}
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Init()
{
	// инициализация габаритов игрового уровня
	Level_Rect.left = CsConfig::Level_X_Offset * CsConfig::Global_Scale;
	Level_Rect.top = CsConfig::Level_Y_Offset * CsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left + CsConfig::Cell_Width * CsConfig::Level_Width * CsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + CsConfig::Cell_Height * CsConfig::Level_Height * CsConfig::Global_Scale;

	memset(Current_Level, 0, sizeof(Current_Level));
	memset(Active_Bricks, 0, sizeof(Active_Bricks));
	memset(Falling_Letters, 0, sizeof(Falling_Letters));

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Set_Current_Level(char level[CsConfig::Level_Height][CsConfig::Level_Width])
{
	memcpy(Current_Level, level, sizeof(Current_Level));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Act()
{
	for (int i = 0; i < CsConfig::Max_Active_Bricks_Count; ++i)
	{
		if (Active_Bricks[i] != 0)
		{
			Active_Bricks[i]->Act();

			if (Active_Bricks[i]->Is_Finished())
			{
				delete Active_Bricks[i];
				Active_Bricks[i] = 0;
				--Active_Bricks_Count;
			}
		}
	}

	// !!!	Копия логики
	for (int i = 0; i < CsConfig::Max_Falling_Letters_Count; ++i)
	{
		if (Falling_Letters[i] != 0)
		{
			Falling_Letters[i]->Act();

			if (Falling_Letters[i]->Is_Finished())
			{
				delete Falling_Letters[i];
				Falling_Letters[i] = 0;
				--Falling_Letter_Count;
			}
		}
	}

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw(HDC hdc, RECT& paint_area)
{	// Отрисовка уровня игры
	RECT  intersection_rect;																								// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))				// определение пересечения области рисования и перерисовки кадра
	{
		for (int i = 0; i < CsConfig::Level_Height; ++i)
			for (int j = 0; j < CsConfig::Level_Width; ++j)
				Draw_Brick(hdc, CsConfig::Level_X_Offset + j * CsConfig::Cell_Width, CsConfig::Level_Y_Offset + i * CsConfig::Cell_Height, static_cast<EBrick_Type>(Current_Level[i][j]));

		for (int i = 0; i < CsConfig::Max_Active_Bricks_Count; ++i)
		{
			if (Active_Bricks[i] != 0)
				Active_Bricks[i]->Draw(hdc, paint_area);
		}
	}

	// !!!	Копия логики
	for (int i = 0; i < CsConfig::Max_Falling_Letters_Count; ++i)
	{
		if (Falling_Letters[i] != 0)
			Falling_Letters[i]->Draw(hdc, paint_area);
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::On_Hit(int brick_x, int brick_y)
{
	EBrick_Type brick_type;

	brick_type = static_cast<EBrick_Type>(Current_Level[brick_y][brick_x]);

	if (Add_Falling_Letter(brick_x, brick_y, brick_type))
		Current_Level[brick_y][brick_x] = static_cast<char>(EBrick_Type::None);
	else
		Add_Active_Brick(brick_x, brick_y, brick_type);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{ // Создаем падающую букву, если можем

	int letter_x, letter_y;
	CFalling_Letter* falling_letter;

	if (brick_type == EBrick_Type::Red || brick_type == EBrick_Type::Blue)
	{
		if (CsConfig::Rand(CsConfig::Hits_Per_Letter) == 0)
		{
			if (Falling_Letter_Count < CsConfig::Max_Falling_Letters_Count)
			{
				for (int i = 0; i < CsConfig::Max_Falling_Letters_Count; ++i)
				{
					if (Falling_Letters[i] == 0)
					{
						letter_x = (brick_x * CsConfig::Cell_Width + CsConfig::Level_X_Offset) * CsConfig::Global_Scale;
						letter_y = (brick_y * CsConfig::Cell_Height + CsConfig::Level_Y_Offset) * CsConfig::Global_Scale;

						falling_letter = new CFalling_Letter(brick_type, ELetter_Type::O, letter_x, letter_y);
						Falling_Letters[i] = falling_letter;
						++Falling_Letter_Count;
						break;
					}
				}
				return true;;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{ // Создаем активный кирпич, если можем

	CActive_Brick* active_brick;

	if (Active_Bricks_Count >= CsConfig::Max_Active_Bricks_Count)
		return;	// Активных кирпичей слишеом много, заглушка

	switch (brick_type)
	{
	case EBrick_Type::None:
		return;

	case EBrick_Type::Red:
	case EBrick_Type::Blue:
		active_brick = new CActive_Brick(brick_type, brick_x, brick_y);
		Current_Level[brick_y][brick_x] = static_cast<char>(EBrick_Type::None);
		break;

	default:
		return;
	}

	for (int i = 0; i < CsConfig::Max_Active_Bricks_Count; ++i)
	{
		if (Active_Bricks[i] == 0)
		{
			Active_Bricks[i] = active_brick;
			++Active_Bricks_Count;
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos)
{
	double direction = ball->Get_Direction();

	if (ball->Is_Moving_Up())
	{// Проверка попадания в нижнюю грань кирпича
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{// Проверка возможности отскока вниз
			if (level_y < CsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0)
				return true;
			else
				return false;
		}
	}
	else
	{	// Проверка попадания в верхнюю грань кирпича
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{// Проверка возможности отскока вверх
			if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
				return true;
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

	if (! ball->Is_Moving_Left())
	{// Проверка попадания в левую грань кирпича
		if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{// Проверка возможности отскока влево
			if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
				return true;
			else
				return false;
		}
	}
	else
	{// Проверка попадания в правую грань кирпича
		if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{// Проверка возможности отскока вправо
			if (level_x < CsConfig::Level_Width && Current_Level[level_y][level_x + 1] == 0)
				return true;
			else
				return false;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{	//Отрисовка элемента игры - кирпича

	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBrick_Type::None:		
		pen = CsConfig::BG_Pen;
		brush = CsConfig::BG_Brush;
		break;

	case EBrick_Type::Red:
		pen = CsConfig::Brick_Red_pen;
		brush = CsConfig::Brick_Red_brush;
		break;
	case EBrick_Type::Blue:
		pen = CsConfig::Brick_Blue_pen;
		brush = CsConfig::Brick_Blue_brush;
		break;
	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + CsConfig::Brick_Width) * CsConfig::Global_Scale - 1, (y + CsConfig::Brick_Height) * CsConfig::Global_Scale - 1, 
		2 * CsConfig::Global_Scale, 2 * CsConfig::Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
