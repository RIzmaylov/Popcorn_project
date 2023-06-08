#pragma once

#include "Falling_Letter.h"
#include "Ball.h"

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
	void Draw_Brick(HDC hdc, RECT& brick_rect, EBrick_Type brick_type);
	void Draw_Objects(HDC hdc, RECT& paint_area, CGraphics_Object** objects_array, int objects_max_count);
	void Act_Objects(CGraphics_Object** objects_array, int objects_max_count);

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