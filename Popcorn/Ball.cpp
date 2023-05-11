#include "Ball.h"

CBall::CBall():
	Ball_Pen(0),
	Ball_Brush(0),
	Ball_X_Pos(20),															// положение шарика по оси Х
	Ball_Y_Pos(170),														// положение шарика по оси У
	Ball_Speed(4.0),														// скорость смещения шарика
	Ball_Direction(M_PI - M_PI_4),							// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)
	Ball_Rect{},
	Prev_Ball_Rect{}
{}
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

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))					// определение пересечения области рисования и перерисовки кадра
		return;

	// 1. Очищаем фон
	SelectObject(hdc, CsConfig::BG_Pen);
	SelectObject(hdc, CsConfig::BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

	// 2. Рисуем шарик
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Move(CLevel* level, int platform_x_pos, int platform_width)
{
	int next_x_pos, next_y_pos;
	int max_x_pos = CsConfig::Max_X_Pos - CsConfig::Ball_Size;
	int platform_y_pos = CsConfig::Platform_Y_Pos - CsConfig::Ball_Size;
	int max_y_pos = CsConfig::Max_Y_Pos - CsConfig::Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + static_cast<int>(Ball_Speed * cos(Ball_Direction));
	next_y_pos = Ball_Y_Pos - static_cast<int>(Ball_Speed * sin(Ball_Direction));

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
		next_y_pos = max_y_pos - (next_y_pos - max_y_pos);																	// преобразование движения шарика с учетом, если преграда раньше, чем следующее положение
		Ball_Direction = -Ball_Direction;
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

	Ball_Rect.left = Ball_X_Pos * CsConfig::Global_Scale;
	Ball_Rect.top = Ball_Y_Pos * CsConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + CsConfig::Ball_Size * CsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + CsConfig::Ball_Size * CsConfig::Global_Scale;

	InvalidateRect(CsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(CsConfig::Hwnd, &Ball_Rect, FALSE);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
