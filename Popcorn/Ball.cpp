#include "Ball.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
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
	Testing_Is_Active(false),
	Test_Iteration(0),
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

		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}

	if (Ball_State == EBall_State::Lost)
		return;

	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))					// определение пересечения области рисования и перерисовки кадра
	{
		// 2. Рисуем шарик
		SelectObject(hdc, Ball_Pen);
		SelectObject(hdc, Ball_Brush);

		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Move()
{
	bool got_hit;
	double next_x_pos, next_y_pos;

	if (Ball_State != EBall_State::Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;

	Rest_Distance += Ball_Speed;


	while (Rest_Distance >= CsConfig::Moving_Step_Size)
	{
		got_hit = false;
		next_x_pos = Center_X_Pos + (CsConfig::Moving_Step_Size * cos(Ball_Direction));
		next_y_pos = Center_Y_Pos - (CsConfig::Moving_Step_Size * sin(Ball_Direction));

		// корректировка позиции при отражении:
		for (int i = 0; i < Hit_Checkers_Count; ++i)
			got_hit |= Hit_Checkers[i]->Check_Hit(next_x_pos, next_y_pos, this);									// |= - накопление булевых значений с помощью операции ИЛИ

		if (!got_hit)
		{
			// Шарик продолит смещение, если не взаимодействует с другими объектами
			Rest_Distance -= CsConfig::Moving_Step_Size;

			Center_X_Pos = next_x_pos;
			Center_Y_Pos = next_y_pos;

			if (Testing_Is_Active)
				Rest_Test_Distance -= CsConfig::Moving_Step_Size;
		}
	}

	Redraw_Ball();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Set_For_Test()
{
	Testing_Is_Active = true;
	Rest_Test_Distance = 40.0;

	Set_State(EBall_State::Normal, 100 + Test_Iteration, 100);
	Ball_Direction = M_PI - M_PI_4;

	++Test_Iteration;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CBall::Is_Test_Finished()
{
	if (Testing_Is_Active)
	{
		if (Rest_Test_Distance <= 0.0)
		{
			Testing_Is_Active = false;
			Set_State(EBall_State::Lost, 0);
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
EBall_State CBall::Get_State()
{
	return Ball_State;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Set_State(EBall_State new_state, double x_pos, double y_pos)
{
	switch (new_state)
	{
	case EBall_State::Normal:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 3.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::On_Platform:
		Center_X_Pos = x_pos;
		Center_Y_Pos = y_pos;
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI_4;
		Redraw_Ball();
		break;


	case EBall_State::Lost:
		Ball_Speed = 0.0;
		break;
	}

	Ball_State = new_state;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
double CBall::Get_Direction()
{
	return Ball_Direction;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Set_Direction(double new_direction)
{
	const double pi_2 = 2.0 * M_PI;

	while (new_direction > pi_2)
		new_direction -= pi_2;

	while (new_direction < 0.0)
		new_direction += pi_2;

	Ball_Direction = new_direction;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBall::Reflect(bool from_horizontal)
{
	if (from_horizontal)
		Set_Direction(-Ball_Direction);
	else
		Set_Direction(M_PI - Ball_Direction);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CBall::Is_Moving_Up()
{
	if (Ball_Direction >= 0.0 && Ball_Direction < M_PI)
		return true;
	else
		return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CBall::Is_Moving_Left()
{
	if (Ball_Direction > M_PI_2 && Ball_Direction < M_PI + M_PI_2)
		return true;
	else
		return false;
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
