#pragma once

#include "Active_Brick.h"
#include "Ball.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class ELetter_Type
{
	None,
	O
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CLevel : public CHit_Checker
{
public:
	CLevel();

	bool Check_Hit(double next_x_pos, double next_y_pos, CBall* ball);

	void Init();
	void Draw(HDC hdc , RECT& paint_area);

	CActive_Brick Active_Brick;

private:
	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);


	HPEN Brick_Red_pen, Brick_Blue_pen, Letter_Pen;
	HBRUSH Brick_Red_brush, Brick_Blue_brush;
	RECT Level_Rect;

	static char Level_01[CsConfig::Level_Height][CsConfig::Level_Width];												// массив игрового уровня
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------