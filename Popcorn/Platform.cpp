#include "Platform.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsPlatform::CsPlatform() :
	Inner_Width(21),														// ширина платформы между шариками
	X_Pos(CsConfig::Border_X_Offset),						// положение платформы по оси x
	X_Step(CsConfig::Global_Scale * 2),					// смещение платформы по оси x
	Width(28),																	// ширина всей платформы (иеняется в зависимости от ситуации в игре)
	Platform_Circle_pen(0),
	Platform_Inner_pen(0),
	Hightlight_Pen(0),
	Platform_Circle_brush(0),
	Platform_Inner_brush(0),
	Platform_Rect{},
	Prev_Platform_Rect{}
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Init()
{
	Hightlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	CsConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_pen, Platform_Circle_brush);
	CsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_pen, Platform_Inner_brush);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Redraw(HWND hwnd) 
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = (X_Pos) * CsConfig::Global_Scale;
	Platform_Rect.top = CsConfig::Platform_Y_Pos * CsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * CsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * CsConfig::Global_Scale;

	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
}    

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsPlatform::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{	// Отрисовка элемента игры Платформа
	RECT intersection_rect;																										// вспомогательный прямоугольник для определения пересечения областей перерисовки

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))			// определение пересечения области рисования и перерисовки кадра
		return;

	int x = X_Pos;
	int y = CsConfig::Platform_Y_Pos;

	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

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
