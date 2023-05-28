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
	void Set_Current_Level(char level[CsConfig::Level_Height][CsConfig::Level_Width]);
	void Draw(HDC hdc , RECT& paint_area);

	//CActive_Brick Active_Brick;
	static char Level_01[CsConfig::Level_Height][CsConfig::Level_Width];												// массив первого игрового уровня
	static char Test_Level[CsConfig::Level_Height][CsConfig::Level_Width];											// массив тестового игрового уровня

private:
	bool Is_Check_Horizontal_First(double next_x_pos, double next_y_pos);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos);
	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, EBrick_Type brick_type, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);


	HPEN Brick_Red_pen, Brick_Blue_pen, Letter_Pen;
	HBRUSH Brick_Red_brush, Brick_Blue_brush;
	RECT Level_Rect;

	double Current_Brick_Left_X, Current_Brick_Right_X;			// вспомогательные координаты отрезка, представляющуего собой нижнюю грань очередного кирпича
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	char Current_Level[CsConfig::Level_Height][CsConfig::Level_Width];												// массив игрового уровня

};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------