#pragma once
#include "Active_Brick.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class ELetter_Type
{
	None,
	O
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CFalling_Letter : public CGraphics_Object
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
