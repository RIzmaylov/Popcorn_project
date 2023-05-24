#include "Ball.h"

const double CBall::Start_Ball_Y_Pos = 181.0;
const double CBall::Radius = 2.0;
int CBall::Hit_Checkers_Count = 0;
CHit_Checker* CBall::Hit_Checkers[] = {};

CBall::CBall():
	Ball_State(EBall_State::Normal),
	Ball_Pen(0),
	Ball_Brush(0),
	Center_X_Pos(0.0),													// положение центра шарика по оси Х
	Center_Y_Pos(Start_Ball_Y_Pos),							// положение центра шарика по оси У
	Ball_Speed(0.0),														// скорость смещения шарика
	Rest_Distance(0.0),
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
void CBall::Move()
{
	bool got_hit;
	double next_x_pos, next_y_pos;
	int platform_y_pos = CsConfig::Platform_Y_Pos - CsConfig::Ball_Size;
	double step_size = 1.0 / CsConfig::Global_Scale;

	if (Ball_State != EBall_State::Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;

	Rest_Distance += Ball_Speed;


	while (Rest_Distance >= step_size)
	{
		got_hit = false;
		next_x_pos = Center_X_Pos + (step_size * cos(Ball_Direction));
		next_y_pos = Center_Y_Pos - (step_size * sin(Ball_Direction));

		for (int i = 0; i < Hit_Checkers_Count; ++i)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);									// |= - накопление булевых значений с помощью операции ИЛИ

		//// Корректировки движения при отражении от рамки
		//got_hit |= border_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);									// |= - накопление булевых значений с помощью операции ИЛИ
		//// Корректировки движения при отражении от кирпича
		//got_hit |= level_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);
		//// Корректировки движения при отражении от платформы
		//got_hit |= platform_hit_checker->Check_Hit(next_x_pos, next_y_pos, this);


		if (!got_hit)
		{
			// Шарик продолит смещение, если не взаимодействует с другими объектами
			Rest_Distance -= step_size;

			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;
		}
	}

	Redraw_Ball();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
EBall_State CBall::Get_State()
{
	return Ball_State;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Set_State(EBall_State new_state, double x_pos)
{
	switch (new_state)
	{
	case EBall_State::Normal:
		Center_X_Pos = x_pos;
		Center_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 3.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::On_Platform:
		Center_X_Pos = x_pos;
		Center_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::Lost:
		Ball_Speed = 0.0;
		break;
	}

	Ball_State = new_state;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Add_Hit_Checker(CHit_Checker* hit_checker)
{
	if (Hit_Checkers_Count >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]))
		return;

	Hit_Checkers[Hit_Checkers_Count++] = hit_checker;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Redraw_Ball()
{
	Ball_Rect.left = static_cast<int>((Center_X_Pos - Radius) * CsConfig::Global_Scale);
	Ball_Rect.top = static_cast<int>((Center_Y_Pos - Radius) * CsConfig::Global_Scale);
	Ball_Rect.right = static_cast<int>((Center_X_Pos + Radius) * CsConfig::Global_Scale);
	Ball_Rect.bottom = static_cast<int>((Center_Y_Pos + Radius) * CsConfig::Global_Scale);

	InvalidateRect(CsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(CsConfig::Hwnd, &Ball_Rect, FALSE);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
