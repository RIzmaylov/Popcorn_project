#pragma once

#include "Config.h"

enum class EPlatform_State
{
	Missing,
	Normal,
	Meltdown	
};

class CsPlatform
{
public:
	CsPlatform();
	void Init();
	void Act(HWND hwnd);
	void Redraw(HWND hwnd); 
	void Draw(HDC hdc, RECT& paint_area);

	int X_Pos;																																// положение платформы по оси x
	int X_Step;																																// смещение платформы по оси x
	int Width;																																// ширина всей платформы (иеняется в зависимости от ситуации в игре)

private:
	void Draw_Normal_State(HDC hdc, RECT& paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);

	EPlatform_State Platform_State;
	int Inner_Width;																													// ширина платформы между шариками

	static const int Normal_Width = 28;

	int Meltdown_Platform_Y_Pos[Normal_Width * CsConfig::Global_Scale];				// положение Y расплавляющейся платформы


	HPEN Platform_Circle_pen, Platform_Inner_pen, Hightlight_Pen;
	HBRUSH Platform_Circle_brush, Platform_Inner_brush;
	RECT Platform_Rect, Prev_Platform_Rect;

	static const int Circle_Size = 7;																					// размер шарика платформы
	static const int Height = 7;																							// высота всей платформы (не меняется)
	static const int Meltdown_Speed = 3;
};