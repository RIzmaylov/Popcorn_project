#include "Ball.h"

const double CBall::Start_Ball_Y_Pos = 181.0;

CBall::CBall():
	Ball_State(EBall_State::Normal),
	Ball_Pen(0),
	Ball_Brush(0),
	Ball_X_Pos(0.0),														// положение шарика по оси Х
	Ball_Y_Pos(Start_Ball_Y_Pos),								// положение шарика по оси У
	Ball_Speed(0.0),														// скорость смещения шарика
	Ball_Direction(0),													// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)
	Ball_Rect{},
	Prev_Ball_Rect{}
{
	Set_State(EBall_State::Normal, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Init()
{
	// создание кистей и карандашей
	CsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Draw(HDC hdc, RECT& paint_area) 
{	// Отрисовка шарика 
	RECT intersection_rect;																										// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))					// определение пересечения области рисования и перерисовки кадра
	{
		// 1. Очищаем фон
		SelectObject(hdc, CsConfig::BG_Pen);
		SelectObject(hdc, CsConfig::BG_Brush);

		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
	}
	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))					// определение пересечения области рисования и перерисовки кадра
	{
		// 2. Рисуем шарик
		SelectObject(hdc, Ball_Pen);
		SelectObject(hdc, Ball_Brush);

		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Move(CLevel* level, int platform_x_pos, int platform_width)
{
	double next_x_pos, next_y_pos;
	int max_x_pos = CsConfig::Max_X_Pos - CsConfig::Ball_Size;
	int platform_y_pos = CsConfig::Platform_Y_Pos - CsConfig::Ball_Size;
	int max_y_pos = CsConfig::Max_Y_Pos - CsConfig::Ball_Size;

	if (Ball_State != EBall_State::Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (Ball_Speed * cos(Ball_Direction));
	next_y_pos = Ball_Y_Pos - (Ball_Speed * sin(Ball_Direction));

	// Корректировки движения при отражении от рамки
	if (next_x_pos < CsConfig::Border_X_Offset)
	{
		next_x_pos = CsConfig::Level_X_Offset - (next_x_pos - CsConfig::Level_X_Offset);
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < CsConfig::Border_Y_Offset)
	{
		next_y_pos = CsConfig::Border_Y_Offset - (next_y_pos - CsConfig::Border_Y_Offset);	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = - Ball_Direction;
	}
	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);																	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos > max_y_pos)
	{
		if (level->Has_Floor)
		{
			next_y_pos = max_y_pos - (next_y_pos - max_y_pos);																	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
			Ball_Direction = -Ball_Direction;
		}
		else
		{
			if (next_y_pos > max_y_pos + CsConfig::Ball_Size)																		// Проверка нижней границы с учетом размера шарика
				Ball_State = EBall_State::Lost;
		}
	}

	// Корректировки движения при отражении от платформы
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);											// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Корректировки движения при отражении от кирпича
	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

	// Смещение шарика
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Redraw_Ball();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
EBall_State CBall::Get_State()
{
	return Ball_State;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Set_State(EBall_State new_state, int x_pos)
{
	switch (new_state)
	{
	case EBall_State::Normal:
		Ball_X_Pos = x_pos - CsConfig::Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 3.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::On_Platform:
		Ball_X_Pos = x_pos - CsConfig::Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::Lost:
		Ball_Speed = 0.0;
		break;
	}

	Ball_State = EBall_State::Normal;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Redraw_Ball()
{
	Ball_Rect.left = static_cast<int>(Ball_X_Pos * CsConfig::Global_Scale);
	Ball_Rect.top = static_cast<int>(Ball_Y_Pos * CsConfig::Global_Scale);
	Ball_Rect.right = Ball_Rect.left + CsConfig::Ball_Size * CsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + CsConfig::Ball_Size * CsConfig::Global_Scale;

	InvalidateRect(CsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(CsConfig::Hwnd, &Ball_Rect, FALSE);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
