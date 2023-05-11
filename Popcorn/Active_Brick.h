#pragma once

#include "Config.h"

enum class EBrick_Type
{
	None,
	Red,
	Blue
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

class CActive_Brick
{
public:
	CActive_Brick(EBrick_Type brick_type);

	void Act();
	void Draw(HDC hdc , RECT& paint_area);

	static void Setup_Colors();

private:
	RECT Brick_Rect;
	EBrick_Type Brick_Type;
	int Fade_Step;

	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const CColor& color, int step, HPEN& pen, HBRUSH& brush);

	// Максимальный шаг затухания кирпича
	static const int Max_Fade_Step = 40;

	// Создание массивов карандашей и кистей для затухающих кирпичей
	static HPEN Fading_Red_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Red_Brick_Brushes[Max_Fade_Step];
	static HPEN Fading_Blue_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Max_Fade_Step];
};