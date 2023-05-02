#include "Engine.h"

char Level_01[CLevel::Level_Height][CLevel::Level_Width] = {												// массив игрового уровня
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
CBall::CBall():
	Ball_X_Pos(20),															// положение шарика по оси Х
	Ball_Y_Pos(170),														// положение шарика по оси У
	Ball_Speed(4.0),														// скорость смещения шарика
	Ball_Direction(M_PI - M_PI_4)								// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Init()
{	
	// создание кистей и карандашей
	CsEngine::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Draw(HDC hdc, RECT& paint_area, CsEngine* engine) 
{	// Отрисовка шарика 
	RECT intersection_rect;																										// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))					// определение пересечения области рисования и перерисовки кадра
		return;

	// 1. Очищаем фон
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

	// 2. Рисуем шарик
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Move(CsEngine* engine, CLevel* level, CsPlatform* platform)
{
	int next_x_pos, next_y_pos;
	int max_x_pos = CsEngine::Max_X_Pos - Ball_Size;
	int platform_y_pos = CsPlatform::Y_Pos - Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + static_cast<int>(Ball_Speed * cos(Ball_Direction));
	next_y_pos = Ball_Y_Pos - static_cast<int>(Ball_Speed * sin(Ball_Direction));

	// Корректировки движения при отражении от рамки
	if (next_x_pos < CsBorder::Border_X_Offset)
	{
		next_x_pos = CLevel::Level_X_Offset - (next_x_pos - CLevel::Level_X_Offset);
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < CsBorder::Border_Y_Offset)
	{
		next_y_pos = CsBorder::Border_Y_Offset - (next_y_pos - CsBorder::Border_Y_Offset);	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = - Ball_Direction;
	}
	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);						// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos > CsEngine::Max_Y_Pos)
	{
		next_y_pos = CsEngine::Max_Y_Pos - (next_y_pos - CsEngine::Max_Y_Pos);						// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	// Корректировки движения при отражении от платформы
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);						// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Корректировки движения при отражении от кирпича
	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

	// Смещение шарика
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Ball_Rect.left = Ball_X_Pos * CsEngine::Global_Scale;
	Ball_Rect.top = Ball_Y_Pos * CsEngine::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + Ball_Size *CsEngine:: Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * CsEngine::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, CsEngine::Global_Scale, RGB(255, 255, 255));

	// создание кистей и карандашей
	CsEngine::Create_Pen_Brush(255, 85, 85, Brick_Red_pen, Brick_Red_brush);
	CsEngine::Create_Pen_Brush(85, 255, 255, Brick_Blue_pen, Brick_Blue_brush);

	// инициализация габаритов игрового уровня
	Level_Rect.left = CLevel::Level_X_Offset * CsEngine::Global_Scale;
	Level_Rect.top = CLevel::Level_Y_Offset * CsEngine::Global_Scale;
	Level_Rect.right = Level_Rect.left + CLevel::Cell_Width * CLevel::Level_Width * CsEngine::Global_Scale;
	Level_Rect.bottom = Level_Rect.top + CLevel::Cell_Height * CLevel::Level_Height * CsEngine::Global_Scale;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void CLevel::Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction)
{// Корректировки движения при отражении от кирпича

	int brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;

	for (int i = Level_Height - 1; i >= 0; --i)
	{
		for (int j = 0; j < Level_Width; ++j)
		{
			if (Level_01[i][j] == 0)
				continue;
			if (next_y_pos < brick_y_pos)
			{
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
				ball_direction = -ball_direction;
			}

		}
		brick_y_pos -= Cell_Height;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CLevel::Draw(HDC hdc, RECT& paint_area)
{	// Отрисовка уровня игры
	RECT  intersection_rect;																								// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))				// определение пересечения области рисования и перерисовки кадра
		return;

	for (int i = 0; i < Level_Height; ++i) 
		for (int j = 0; j < Level_Width; ++j) 
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(Level_01[i][j]));
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
	RoundRect(hdc, x * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + Brick_Width) * CsEngine::Global_Scale, (y + Brick_Height) * CsEngine::Global_Scale, 
		2 * CsEngine::Global_Scale, 2 * CsEngine::Global_Scale);
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
	int brick_half_height = Brick_Height * CsEngine::Global_Scale / 2;
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

		Rectangle(hdc, x, y + brick_half_height - CsEngine::Global_Scale, x + Brick_Width * CsEngine::Global_Scale, y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * CsEngine::Global_Scale, y + brick_half_height + CsEngine::Global_Scale - 1);
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

		offset = 3.0 * (1.0 - abs(xform.eM22)) * static_cast<double>(CsEngine::Global_Scale);
		back_part_offset = static_cast<int>(round(offset));
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * CsEngine::Global_Scale, brick_half_height - back_part_offset);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, Brick_Width * CsEngine::Global_Scale, brick_half_height);
		if (rotation_step > 4 && rotation_step < 13)
		{
			if (letter_type == ELetter_Type::O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + 5 * CsEngine::Global_Scale, -5 * CsEngine::Global_Scale / 2, 0 + 10 * CsEngine::Global_Scale, 5 * CsEngine::Global_Scale / 2);
			}
		}


		SetWorldTransform(hdc, &old_xform);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::CsPlatform() :
	Inner_Width(21),														// ширина платформы между шариками
	X_Pos(CsBorder::Border_X_Offset),						// положение платформы по оси x
	X_Step(CsEngine::Global_Scale * 2),					// смещение платформы по оси x
	Width(28)																		// ширина всей платформы (иеняется в зависимости от ситуации в игре)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Init()
{
	Hightlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	CsEngine::Create_Pen_Brush(151, 0, 0, Platform_Circle_pen, Platform_Circle_brush);
	CsEngine::Create_Pen_Brush(0, 128, 192, Platform_Inner_pen, Platform_Inner_brush);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Redraw(CsEngine* engine) 
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = (X_Pos) * CsEngine::Global_Scale;
	Platform_Rect.top = Y_Pos * CsEngine::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * CsEngine::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CsEngine::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}    

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw(HDC hdc, CsEngine* engine, RECT& paint_area)
{	// Отрисовка элемента игры Платформа
	RECT intersection_rect;																									// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))			// определение пересечения области рисования и перерисовки кадра
		return;

	int x = X_Pos;
	int y = Y_Pos;

	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + Circle_Size) * CsEngine::Global_Scale, (y + Circle_Size) * CsEngine::Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + Inner_Width + Circle_Size) * CsEngine::Global_Scale, (y + Circle_Size) * CsEngine::Global_Scale);

	// 2. Рисуем блик на левом шарике
	SelectObject(hdc, Hightlight_Pen);

	Arc(hdc, (x + 1) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + Circle_Size - 1) * CsEngine::Global_Scale, (y + Circle_Size - 1) * CsEngine::Global_Scale, 
		(x + 1 + 1) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + 1) * CsEngine::Global_Scale, (y + 1 + 3) * CsEngine::Global_Scale);
	Arc(hdc, (x + 1) * CsEngine::Global_Scale + 1, (y + 1) * CsEngine::Global_Scale + 1, (x + Circle_Size - 1) * CsEngine::Global_Scale, (y + Circle_Size - 1) * CsEngine::Global_Scale, 
		(x + 1 + 1) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + 1) * CsEngine::Global_Scale, (y + 1 + 3) * CsEngine::Global_Scale);

	// 3. Рисуем платформу между шариками
	SelectObject(hdc, Platform_Inner_pen);
	SelectObject(hdc, Platform_Inner_brush);

	RoundRect(hdc, (x + 4) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + 4 + Inner_Width - 1) * CsEngine::Global_Scale, (y + 1 + 5) * CsEngine::Global_Scale, 4 * CsEngine::Global_Scale, 4 * CsEngine::Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Init()
{
	CsEngine::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
	CsEngine::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border, CsEngine* engine) 
{	//Отрисовка одного тайла рамки игрового поля

	// Выводится основная линия рамки
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (is_top_border)
		Rectangle(hdc, x * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + 4) * CsEngine::Global_Scale, (y + 4) * CsEngine::Global_Scale);
	else
		Rectangle(hdc, (x + 1) * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + 4) * CsEngine::Global_Scale, (y + 4) * CsEngine::Global_Scale);

	// Выводится дополнительная линия рамки
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (is_top_border)
		Rectangle(hdc, x * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + 4) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale);
	else 
		Rectangle(hdc, x * CsEngine::Global_Scale, y * CsEngine::Global_Scale, (x + 1) * CsEngine::Global_Scale, (y + 4) * CsEngine::Global_Scale);


	// Выводится однопиксельная перфорация рамки
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	if (is_top_border)
		Rectangle(hdc, (x + 2) * CsEngine::Global_Scale, (y + 2) * CsEngine::Global_Scale, (x + 3) * CsEngine::Global_Scale, (y + 3) * CsEngine::Global_Scale);
	else
		Rectangle(hdc, (x + 2) * CsEngine::Global_Scale, (y + 1) * CsEngine::Global_Scale, (x + 3) * CsEngine::Global_Scale, (y + 2) * CsEngine::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsBorder::Draw(HDC hdc,  RECT& paint_area, CsEngine* engine) 
{	// Отрисовка рамки из тайлов

	// 1. Отрисовка левой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 2, 1 + i * 4, false, engine);
	}

	// 2. Отрисовка правой рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 201, 1 + i * 4, false, engine);
	}

	// 3. Отрисовка верхней рамки
	for (int i = 0; i < 50; ++i) 
	{
		Draw_Element(hdc, 3 + i * 4, 0, true, engine);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsEngine::CsEngine()				
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Init_Engine(HWND hwnd)
{	//Настройка игры при старте

	Hwnd = hwnd;


	// создание кистей и карандашей
	Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
	Border.Init();
	Platform.Init();
	Ball.Init();
	// инициализация габаритов игрового уровня
	Level.Init();

	// очищение области платформы при первом запуске (чтобы не оставался след при первом перемещении платформы)
	Platform.Redraw(this);

	// установка таймера
	SetTimer(Hwnd, Timer_Id, 50, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Draw_Frame(HDC hdc, RECT& paint_area) 
{	// Отрисовка экрана игры

	Level.Draw(hdc, paint_area);

	Platform.Draw(hdc, this, paint_area);
	
	Ball.Draw(hdc, paint_area, this);

	Border.Draw(hdc, paint_area, this);

	//for (int i = 0; i < 16; ++i) {
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
	//}

}    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsEngine::On_Key_Down(EKey_Type key_type) 
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= CsBorder::Border_X_Offset)
			Platform.X_Pos = CsBorder::Border_X_Offset;

		Platform.Redraw(this);
		break;

	case EKey_Type::Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = Max_X_Pos - Platform.Width + 1;

		Platform.Redraw(this);
		break;

	case EKey_Type::Space:
		break;
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsEngine::On_Timer()
{
	Ball.Move(this, &Level, &Platform);
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
