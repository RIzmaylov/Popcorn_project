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
class CFalling_Letter
{
public:
	CFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	void Act();
	void Draw(HDC hdc , RECT& paint_area);
	bool Is_Finished();

	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;
	bool Got_Hit;

private:
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc);

	int X, Y;
	int Rotation_Step;
	int Next_Rotation_Tick;
	RECT Letter_Cell, Prev_Letter_Cell;

	static const int Ticks_Per_Step = 4;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CLevel : public CHit_Checker
{
public:
	CLevel();

	bool Check_Hit(double next_x_pos, double next_y_pos, CBall* ball);

	void Init();
	void Set_Current_Level(char level[CsConfig::Level_Height][CsConfig::Level_Width]);
	void Act();
	void Draw(HDC hdc , RECT& paint_area);

	//CActive_Brick Active_Brick;
	static char Level_01[CsConfig::Level_Height][CsConfig::Level_Width];												// массив первого игрового уровня
	static char Test_Level[CsConfig::Level_Height][CsConfig::Level_Width];											// массив тестового игрового уровня

private:
	void On_Hit(int brick_x, int brick_y);
	bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
	void Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, CBall* ball, double& reflection_pos);
	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);

	RECT Level_Rect;

	double Current_Brick_Left_X, Current_Brick_Right_X;			// вспомогательные координаты отрезка, представляющуего собой нижнюю грань очередного кирпича
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	char Current_Level[CsConfig::Level_Height][CsConfig::Level_Width];												// массив игрового уровня
	int Active_Bricks_Count;
	CActive_Brick* Active_Bricks[CsConfig::Max_Active_Bricks_Count];
	int Falling_Letter_Count;
	CFalling_Letter* Falling_Letters[CsConfig::Max_Falling_Letters_Count];
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------