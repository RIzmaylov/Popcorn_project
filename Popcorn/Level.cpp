#include "Level.h"

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



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CLevel::CLevel() :
	Active_Brick(EBrick_Type::Blue),
	Brick_Red_pen(0),
	Brick_Blue_pen(0),
	Letter_Pen(0),
	Brick_Red_brush(0),
	Brick_Blue_brush(0),
	Level_Rect{}
{}
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
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void CLevel::Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction)
{// Корректировки движения при отражении от кирпича

	int brick_y_pos = CsConfig::Level_Y_Offset + CsConfig::Level_Height * CsConfig::Cell_Height;

	for (int i = CsConfig::Level_Height - 1; i >= 0; --i)
	{
		for (int j = 0; j < CsConfig::Level_Width; ++j)
		{
			if (Level_01[i][j] == 0)
				continue;
			if (next_y_pos < brick_y_pos)
			{
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);						// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
				ball_direction = -ball_direction;
			}

		}
		brick_y_pos -= CsConfig::Cell_Height;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw(HWND hwnd, HDC hdc, RECT& paint_area)
{	// Отрисовка уровня игры
	RECT  intersection_rect;																								// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))				// определение пересечения области рисования и перерисовки кадра
		return;

	for (int i = 0; i < CsConfig::Level_Height; ++i) 
		for (int j = 0; j < CsConfig::Level_Width; ++j) 
			Draw_Brick(hdc, CsConfig::Level_X_Offset + j * CsConfig::Cell_Width, CsConfig::Level_Y_Offset + i * CsConfig::Cell_Height, static_cast<EBrick_Type>(Level_01[i][j]));
	
	Active_Brick.Draw(hdc, paint_area);
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
