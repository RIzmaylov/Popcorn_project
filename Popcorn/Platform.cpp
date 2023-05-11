#include "Platform.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::CsPlatform() :
	X_Pos(CsConfig::Border_X_Offset),						// положение платформы по оси x
	X_Step(CsConfig::Global_Scale * 2),					// смещение платформы по оси x
	Width(Normal_Width),												// ширина всей платформы (иеняется в зависимости от ситуации в игре)
	Inner_Width(Normal_Platform_Inner_Width),		// ширина платформы между шариками
	Rolling_Step(0),
	Platform_State(EPlatform_State::Normal),
	Platform_Circle_pen(0),
	Platform_Inner_pen(0),
	Hightlight_Pen(0),
	Platform_Circle_brush(0),
	Platform_Inner_brush(0),
	Platform_Rect{},
	Prev_Platform_Rect{}
{
	X_Pos = (CsConfig::Max_X_Pos - Width) / 2;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Init()
{
	Hightlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	CsConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_pen, Platform_Circle_brush);
	CsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_pen, Platform_Inner_brush);
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
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
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

	Arc(hdc, x + CsConfig::Global_Scale, y + CsConfig::Global_Scale, x + (Circle_Size - 1) * CsConfig::Global_Scale, y + (Circle_Size - 1) * CsConfig::Global_Scale, 
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

	Ellipse(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Circle_Size) * CsConfig::Global_Scale, (y + Circle_Size) * CsConfig::Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Inner_Width + Circle_Size) * CsConfig::Global_Scale, (y + Circle_Size) * CsConfig::Global_Scale);

	// 2. Рисуем блик на левом шарике
	Draw_Circle_Highlight(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale);

	// 3. Рисуем платформу между шариками
	SelectObject(hdc, Platform_Inner_pen);
	SelectObject(hdc, Platform_Inner_brush);

	RoundRect(hdc, (x + 4) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * CsConfig::Global_Scale, (y + 1 + 5) * CsConfig::Global_Scale, 4 * CsConfig::Global_Scale, 4 * CsConfig::Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{	// Отрисовка расплавляющейся платформы

	int area_width, area_height;												// области перерисовки платформы с учетом масштаба
	int x, y, y_offset;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(CsConfig::BG_Color.R, CsConfig::BG_Color.G, CsConfig::BG_Color.B);

	area_width = Width * CsConfig::Global_Scale;
	area_height = Height * CsConfig::Global_Scale + 1;

	for (int i = 0; i < area_width; ++i)
	{
		y_offset = CsConfig::Rand(Meltdown_Speed) + 1;
		x = Platform_Rect.left + i;

		for (int j = 0; j < area_height; ++j)
		{
			y = Meltdown_Platform_Y_Pos[i] - j;

			pixel = GetPixel(hdc, x, y);
			SetPixel(hdc, x, y + y_offset, pixel);
		}

		for (int j = 0; j < y_offset; ++j)
		{
			y = Meltdown_Platform_Y_Pos[i] - area_height + 1 + j;
			SetPixel(hdc, x, y, bg_pixel);
		}

		Meltdown_Platform_Y_Pos[i] += y_offset;
	}
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
	SetGraphicsMode(hdc, GM_ADVANCED);

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

	Rectangle(hdc, -CsConfig::Global_Scale / 2, -roller_size / 2, CsConfig::Global_Scale / 2, roller_size / 2);

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
		Platform_State = EPlatform_State::Normal;
		Redraw();
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
