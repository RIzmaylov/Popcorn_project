#pragma once

#include "Config.h"

class CsPlatform
{
public:
	CsPlatform();
	void Init();

	void Redraw(HWND hwnd); 
	void Draw(HDC hdc, RECT& paint_area);

	int X_Pos;																																// положение платформы по оси x
	int X_Step;																																// смещение платформы по оси x
	int Width;																																// ширина всей платформы (иеняется в зависимости от ситуации в игре)

private:

	int Inner_Width;																													// ширина платформы между шариками


	HPEN Platform_Circle_pen, Platform_Inner_pen, Hightlight_Pen;
	HBRUSH Platform_Circle_brush, Platform_Inner_brush;
	RECT Platform_Rect, Prev_Platform_Rect;

	static const int Circle_Size = 7;																					// размер шарика платформы
	static const int Height = 7;																							// высота всей платформы (не меняется)

};