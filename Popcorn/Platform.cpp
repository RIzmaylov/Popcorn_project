#include "Platform.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::CsPlatform() :
	X_Pos(CsConfig::Border_X_Offset),						// положение платформы по оси x
	X_Step(CsConfig::Global_Scale * 2),					// смещение платформы по оси x
	Width(Normal_Width),												// ширина всей платформы (иеняется в зависимости от ситуации в игре)
	Inner_Width(Normal_Platform_Inner_Width),		// ширина платформы между шариками
	Rolling_Step(0),
	Normal_Platform_Image_Width(0),
	Normal_Platform_Image_Height(0),
	Normal_Platform_Image(0),
	Platform_State(EPlatform_State::Normal),
	Platform_Circle_pen(0),
	Platform_Inner_pen(0),
	Hightlight_Pen(0),
	Platform_Circle_brush(0),
	Platform_Inner_brush(0),
	Platform_Rect{},
	Prev_Platform_Rect{},
	Platform_Circle_pen_Color(151, 0, 0),
	Platform_Inner_pen_Color(0, 128, 192),
	Hightlight_Pen_Color(255, 255, 255)
{
	X_Pos = (CsConfig::Max_X_Pos - Width) / 2;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::~CsPlatform()
{
	delete[] Normal_Platform_Image;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsPlatform::Check_Hit(double next_x_pos, double next_y_pos, CBall* ball)
{
	double inner_left_x, inner_right_x;																// вспомогательные координаты по оси х левого и правого края внутренней части платформы
	double inner_top_y, inner_low_y;																	// вспомогательные координаты по оси у верхнейго и нижнего края внутренней части платформы
	double inner_y;
	double reflection_pos;																						// вспомогательная переменная для передачи в функцию (заглушка)

	if (next_y_pos + ball->Radius < CsConfig::Platform_Y_Pos)
		return false;

	inner_top_y = static_cast<double>(CsConfig::Platform_Y_Pos + 1);
	inner_low_y = static_cast<double>(CsConfig::Platform_Y_Pos + Height - 1);
	inner_left_x = static_cast<double>(X_Pos + Circle_Size - 1);
	inner_right_x = static_cast<double>(X_Pos + Width - (Circle_Size - 1));

	// 1 Отражения от шарика платформы
	if (Reflect_On_Circle(next_x_pos, next_y_pos, 0, ball))
		return true; // левого
	if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - Circle_Size, ball))
		return true; // правого

	// 2. Отражения от центральной части платформы
	if (ball->Is_Moving_Up())
		inner_y = inner_low_y;	// от нижней грани
	else
		inner_y = inner_top_y;	// от верхней грани

	if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->Radius, reflection_pos))
	{
		ball->Reflect(true);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Init()
{
	Hightlight_Pen = CreatePen(PS_SOLID, 0, Hightlight_Pen_Color.Get_RGB());

	CsConfig::Create_Pen_Brush(Platform_Circle_pen_Color, Platform_Circle_pen, Platform_Circle_brush);
	CsConfig::Create_Pen_Brush(Platform_Inner_pen_Color, Platform_Inner_pen, Platform_Inner_brush);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Act()
{
	switch (Platform_State)
	{
	case EPlatform_State::Meltdown:
	case EPlatform_State::Roll_In:
	case EPlatform_State::Expand_Roll_In:
		Redraw();
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
EPlatform_State CsPlatform::Get_State()
{
	return Platform_State;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Set_State(EPlatform_State new_state)
{
	if (Platform_State == new_state)
		return;
	int len; 

	switch (new_state)
	{
	case EPlatform_State::Meltdown:
		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

		for (int i = 0; i < len; ++i)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
		break;

	case EPlatform_State::Roll_In:
		X_Pos = CsConfig::Max_X_Pos - 2;
		Rolling_Step = Max_Rolling_Step - 1;
		break;
	}
	
	Platform_State = new_state;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Redraw() 
{
	Prev_Platform_Rect = Platform_Rect;

	int platform_width;

	if (Platform_State == EPlatform_State::Roll_In)
		platform_width = Circle_Size;
	else
		platform_width = Width;

	Platform_Rect.left = X_Pos * CsConfig::Global_Scale;
	Platform_Rect.top = CsConfig::Platform_Y_Pos * CsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + platform_width * CsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CsConfig::Global_Scale;


	if (Platform_State == EPlatform_State::Meltdown)
		Prev_Platform_Rect.bottom = (CsConfig::Max_Y_Pos + 1) * CsConfig::Global_Scale;

	InvalidateRect(CsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(CsConfig::Hwnd, &Platform_Rect, FALSE);
}    

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw(HDC hdc, RECT& paint_area)
{	// Отрисовка элемента игры Платформа
	RECT intersection_rect;																										// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))			// определение пересечения области рисования и перерисовки кадра
		return;

	switch (Platform_State)
	{
	case EPlatform_State::Normal:
		Draw_Normal_State(hdc, paint_area);
		break;

	case EPlatform_State::Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPlatform_State::Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;

	case EPlatform_State::Expand_Roll_In:
		Draw_Expanding_Roll_In_State(hdc, paint_area);
		break;

	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Clear_BG(HDC hdc)
{	// Заливка фоном предыдущего состояния при перемещении
	SelectObject(hdc, CsConfig::BG_Pen);
	SelectObject(hdc, CsConfig::BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{	// Отрисовка блика шарика платформы
	SelectObject(hdc, Hightlight_Pen);

	Arc(hdc, x + CsConfig::Global_Scale, y + CsConfig::Global_Scale, x + (Circle_Size - 1) * CsConfig::Global_Scale - 1, y + (Circle_Size - 1) * CsConfig::Global_Scale - 1, 
		x + (1 + 1) * CsConfig::Global_Scale, y + CsConfig::Global_Scale, x + CsConfig::Global_Scale, y + (1 + 3) * CsConfig::Global_Scale);
	//Arc(hdc, x + CsConfig::Global_Scale + 1, y + CsConfig::Global_Scale + 1, x + (Circle_Size - 1) * CsConfig::Global_Scale, y + (Circle_Size - 1) * CsConfig::Global_Scale, 
	//	x + (1 + 1) * CsConfig::Global_Scale, y + CsConfig::Global_Scale, x + CsConfig::Global_Scale, y + (1 + 3) * CsConfig::Global_Scale);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{	// Отрисовка нормального состояния платформы
	int x = X_Pos;
	int y = CsConfig::Platform_Y_Pos;

	// Заливка фоном предыдущего состояния при перемещении
	Clear_BG(hdc);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Circle_Size) * CsConfig::Global_Scale - 1, (y + Circle_Size) * CsConfig::Global_Scale) - 1;
	Ellipse(hdc, (x + Inner_Width) * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Inner_Width + Circle_Size) * CsConfig::Global_Scale - 1, (y + Circle_Size) * CsConfig::Global_Scale) - 1;

	// 2. Рисуем блик на левом шарике
	Draw_Circle_Highlight(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale);

	// 3. Рисуем платформу между шариками
	SelectObject(hdc, Platform_Inner_pen);
	SelectObject(hdc, Platform_Inner_brush);

	RoundRect(hdc, (x + 4) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * CsConfig::Global_Scale, (y + 1 + 5) * CsConfig::Global_Scale - 1, 4 * CsConfig::Global_Scale, 4 * CsConfig::Global_Scale) - 1;
	
	x *= CsConfig::Global_Scale;
	y *= CsConfig::Global_Scale;

	if (Normal_Platform_Image == 0)
	{
		int offset = 0;

		Normal_Platform_Image_Width = Width * CsConfig::Global_Scale;
		Normal_Platform_Image_Height = Height * CsConfig::Global_Scale;

		Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];
	
		for (int i = 0; i < Normal_Platform_Image_Height; ++i)
			for (int j = 0; j < Normal_Platform_Image_Width; ++j)
				Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{	// Отрисовка расплавляющейся платформы

	int x, y, y_offset;
	int stroke_len;
	int moved_column_count = 0;
	int max_platform_y;
	HPEN color_pen;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(CsConfig::BG_Color.R, CsConfig::BG_Color.G, CsConfig::BG_Color.B);

	max_platform_y = (CsConfig::Max_Y_Pos + 1) * CsConfig::Global_Scale;

	for (int i = 0; i < Normal_Platform_Image_Width; ++i)
	{
		if (Meltdown_Platform_Y_Pos[i] > max_platform_y)
			continue;
		++moved_column_count;

		y_offset = CsConfig::Rand(Meltdown_Speed) + 1;
		x = Platform_Rect.left + i;

		int j = 0;
		y = Meltdown_Platform_Y_Pos[i];

		MoveToEx(hdc, x, y, 0);

		// Отрисовка последовательности вертикальных штрихов разного цвета согласно прообразу, сохраненному в массиве
		while (Get_Platform_Image_Stroke_Color(i, j, color_pen, stroke_len))
		{
			SelectObject(hdc, color_pen);
			LineTo(hdc, x, y + stroke_len);
			y += stroke_len;
			j += stroke_len;
		}

		// стирание фоном пикселей платформы
		y = Meltdown_Platform_Y_Pos[i];
		MoveToEx(hdc, x, y, 0);
		SelectObject(hdc, CsConfig::BG_Pen);
		LineTo(hdc, x, y + y_offset);

		Meltdown_Platform_Y_Pos[i] += y_offset;
	}

	if (moved_column_count == 0)
		Platform_State = EPlatform_State::Missing;															// вся платформа сдвинулась за пределны окна, меняем состояние платформы на "пропавшую"
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{	// Отрисовка выкатывающегося состояния платформы
	int x = X_Pos * CsConfig::Global_Scale;
	int y = CsConfig::Platform_Y_Pos * CsConfig::Global_Scale;
	int roller_size = Circle_Size * CsConfig::Global_Scale;
	double alpha;
	XFORM xform, old_xform;

	Clear_BG(hdc);

	// 1. Выкатывающийся шарик платформы
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x, y, x + roller_size, y + roller_size);

	// 2. Разделительная линия
	alpha = -2.0 * M_PI / static_cast<double>(Max_Rolling_Step) * static_cast<double>(Rolling_Step);

	xform.eM11 = static_cast<float>(cos(alpha));
	xform.eM12 = static_cast<float>(sin(alpha));
	xform.eM21 = static_cast<float>(-sin(alpha));
	xform.eM22 = static_cast<float>(cos(alpha));
	xform.eDx = static_cast<float>(x + roller_size / 2);
	xform.eDy = static_cast<float>(y + roller_size / 2);

	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &xform);

	SelectObject(hdc, CsConfig::BG_Pen);
	SelectObject(hdc, CsConfig::BG_Brush);

	Rectangle(hdc, -CsConfig::Global_Scale / 2, -roller_size / 2, CsConfig::Global_Scale / 2 - 1, roller_size / 2) - 1;

	SetWorldTransform(hdc, &old_xform);

	// 3. Блик
	Draw_Circle_Highlight(hdc, x, y);

	++Rolling_Step;

	if (Rolling_Step >= Max_Rolling_Step)
		Rolling_Step -= Max_Rolling_Step;

	X_Pos -= Rolling_Platform_Speed;

	if (X_Pos <= Roll_In_Platform_End_X_Pos)
	{
		X_Pos += Rolling_Platform_Speed;
		Platform_State = EPlatform_State::Expand_Roll_In;
		Inner_Width = 1;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT& paint_area)
{	// Отрисовка расширяющегося состояния после выкатывания платформы
	Draw_Normal_State(hdc, paint_area);

	--X_Pos;
	Inner_Width += 2;

	if (Inner_Width >= Normal_Platform_Inner_Width)
	{
		Inner_Width = Normal_Platform_Inner_Width;
		Platform_State = EPlatform_State::Ready;
		Redraw();
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, CBall* ball)
{
	double dx, dy;																										// дельты по х и у между центрами шарика платформы и мячика
	double platform_ball_x, platform_ball_y, platform_ball_radius;
	double alpha, beta, gamma;																				// углы для расчета угла отражения мячика от шарика платформы
	double related_ball_direction;																		// повернутый угол отражения шарика для определения отражениямячика от шарика
	double distance, two_radiuses;																		// расстояние от мячика до шарика платформы и два их радиуса

	const double pi_2 = 2.0 * M_PI;

	platform_ball_radius = static_cast<double>(Circle_Size) / 2.0;
	platform_ball_x = static_cast<double>(X_Pos) + platform_ball_radius + platform_ball_x_offset;
	platform_ball_y = static_cast<double>(CsConfig::Platform_Y_Pos) + platform_ball_radius;

	// 1.1 От левого

	dx = next_x_pos - platform_ball_x;
	dy = next_y_pos - platform_ball_y;

	distance = sqrt(dx * dx + dy * dy);
	two_radiuses = platform_ball_radius + ball->Radius;

	if (fabs(distance - two_radiuses) < CsConfig::Moving_Step_Size)
	{// шарики соприкоснулись
		beta = atan2(-dy, dx);					// -dy, потому что у отсчитывается сверху вниз, а  не наоборот, как в математике. dx будет отрицательный и его надо отразить

		related_ball_direction = ball->Get_Direction();
		related_ball_direction -= beta;

		if (related_ball_direction > pi_2)
			related_ball_direction -= pi_2;

		if (related_ball_direction < 0.0)
			related_ball_direction += pi_2;

		if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
		{
			alpha = beta + M_PI - ball->Get_Direction();
			gamma = alpha + beta;

			ball->Set_Direction(gamma);
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, HPEN& color_pen, int& stroke_len)
{	// Вычисление длины очередного штриха при расплавлении платформы
	int offset = y * Normal_Platform_Image_Width + x;					// позиция в массиве Normal_PLatform_Image, соответствующая позиции x, y)
	int color;
	stroke_len = 0;

	if (y >= Normal_Platform_Image_Height)
		return false;

	for (int i = y; i < Normal_Platform_Image_Height; ++i)
	{
		if (i == y)
		{
			color = Normal_Platform_Image[offset];
			stroke_len = 1;
		}
		else
		{
			if (color == Normal_Platform_Image[offset])
				++stroke_len;
			else
				break;
		}

		offset += Normal_Platform_Image_Width;													// Переход на строку ниже
	}

	if (color == Hightlight_Pen_Color.Get_RGB())
		color_pen = Hightlight_Pen;
	else if (color == Platform_Circle_pen_Color.Get_RGB())
		color_pen = Platform_Circle_pen;
	else if (color == Platform_Inner_pen_Color.Get_RGB())
		color_pen = Platform_Inner_pen;
	else if (color == CsConfig::BG_Color.Get_RGB())
		color_pen = CsConfig::BG_Pen;

	return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
