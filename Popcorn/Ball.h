#pragma once

#include "Config.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBall;
enum class EBall_State
{
	Normal,
	Lost,
	On_Platform
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, CBall* ball) = 0;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBall
{
public:
	CBall();
	void Init();

	void Draw(HDC hdc, RECT& paint_area);
	void Move();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos);

	double Ball_Direction;																												// направление смещения шарика(M_PI_4 - число ПИ / 4 т.е. 45 градусов)

	static void Add_Hit_Checker(CHit_Checker* hit_checker);

	static const double Radius;

private:
	void Redraw_Ball();

	EBall_State Ball_State;

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Center_X_Pos;																													// положение центра шарика по оси Х
	double Center_Y_Pos;																													// положение центра шарика по оси У
	double Ball_Speed;																														// скорость смещения шарика
	double Rest_Distance;																													// оставшееся расстояние шарика, необходима для правильного обсчета перемещения

	RECT Ball_Rect, Prev_Ball_Rect;

	static const double Start_Ball_Y_Pos;
	static int Hit_Checkers_Count;
	static CHit_Checker* Hit_Checkers[3];
};