#include "Platform.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::CsPlatform() :
	X_Pos(CsConfig::Border_X_Offset),						// положение платформы по оси x
	X_Step(CsConfig::Global_Scale * 2),					// смещение платформы по оси x
	Width(Normal_Width),												// ширина всей платформы (иеняется в зависимости от ситуации в игре)
	Inner_Width(21),														// ширина платформы между шариками
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
void CsPlatform::Act(HWND hwnd)
{
	if (Platform_State != EPlatform_State::Meltdown)
	{
		Platform_State = EPlatform_State::Meltdown;

		int len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

		for (int i = 0; i < len; ++i)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
		
	}

	if (Platform_State == EPlatform_State::Meltdown)
		Redraw(hwnd);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Redraw(HWND hwnd) 
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = (X_Pos) * CsConfig::Global_Scale;
	Platform_Rect.top = CsConfig::Platform_Y_Pos * CsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * CsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CsConfig::Global_Scale;

	if (Platform_State == EPlatform_State::Meltdown)
		Prev_Platform_Rect.bottom = (CsConfig::Max_Y_Pos + 1) * CsConfig::Global_Scale;

	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
}    

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw(HDC hdc, RECT& paint_area)
{	// Отрисовка элемента игры Платформа
	switch (Platform_State)
	{
	case EPlatform_State::Normal:
		Draw_Normal_State(hdc, paint_area);
		break;

	case EPlatform_State::Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{	// Отрисовка нормального состояния платформы
	RECT intersection_rect;																										// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))			// определение пересечения области рисования и перерисовки кадра
		return;

	int x = X_Pos;
	int y = CsConfig::Platform_Y_Pos;

	SelectObject(hdc, CsConfig::BG_Pen);
	SelectObject(hdc, CsConfig::BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики платформы
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Circle_Size) * CsConfig::Global_Scale, (y + Circle_Size) * CsConfig::Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * CsConfig::Global_Scale, y * CsConfig::Global_Scale, (x + Inner_Width + Circle_Size) * CsConfig::Global_Scale, (y + Circle_Size) * CsConfig::Global_Scale);

	// 2. Рисуем блик на левом шарике
	SelectObject(hdc, Hightlight_Pen);

	Arc(hdc, (x + 1) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + Circle_Size - 1) * CsConfig::Global_Scale, (y + Circle_Size - 1) * CsConfig::Global_Scale, 
		(x + 1 + 1) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 1) * CsConfig::Global_Scale, (y + 1 + 3) * CsConfig::Global_Scale);
	Arc(hdc, (x + 1) * CsConfig::Global_Scale + 1, (y + 1) * CsConfig::Global_Scale + 1, (x + Circle_Size - 1) * CsConfig::Global_Scale, (y + Circle_Size - 1) * CsConfig::Global_Scale, 
		(x + 1 + 1) * CsConfig::Global_Scale, (y + 1) * CsConfig::Global_Scale, (x + 1) * CsConfig::Global_Scale, (y + 1 + 3) * CsConfig::Global_Scale);

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
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))			// определение пересечения области рисования и перерисовки кадра
		return;

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
