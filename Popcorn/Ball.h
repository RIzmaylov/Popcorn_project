#pragma once

#include "Config.h"
#include "Level.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBall
{
public:
	CBall();
	void Init();

	void Draw(HDC hdc, RECT& paint_area);
	void Move(HWND hwnd, CLevel* level, int platform_x_pos, int platform_width);

private:
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Ball_Direction;																										// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)

	int Ball_X_Pos;																														// положение шарика по оси Х
	int Ball_Y_Pos;																														// положение шарика по оси У
	double Ball_Speed;																												// скорость смещения шарика

	RECT Ball_Rect, Prev_Ball_Rect;

};