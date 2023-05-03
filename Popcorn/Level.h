﻿#pragma once

#include "Config.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class ELetter_Type
{
	None,
	O
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class EBrick_Type
{
	None,
	Red,
	Blue
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CLevel
{
public:
	CLevel();
	void Init();

	void Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction);
	void Draw(HDC hdc , RECT& paint_area);


private:
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	HPEN Brick_Red_pen, Brick_Blue_pen, Letter_Pen;
	HBRUSH Brick_Red_brush, Brick_Blue_brush;
	RECT Level_Rect;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------