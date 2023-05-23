#pragma once

#include "Config.h"
#include "Level.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class EBall_State
{
	Normal,
	Lost,
	On_Platform
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBall
{
public:
	CBall();
	void Init();

	void Draw(HDC hdc, RECT& paint_area);
	void Move(CLevel* level, int platform_x_pos, int platform_width);
	EBall_State Get_State();
	void Set_State(EBall_State new_state, int x_pos);

private:
	void Redraw_Ball();

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Ball_Direction;																												// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)
	double Ball_Speed;																														// скорость смещения шарика
	double Ball_X_Pos;																														// положение шарика по оси Х
	double Ball_Y_Pos;																														// положение шарика по оси У

	RECT Ball_Rect, Prev_Ball_Rect;

	static const double Start_Ball_Y_Pos;
};