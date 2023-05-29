#pragma once

#include "Ball.h"

enum class EPlatform_State
{
	Missing,
	Ready,
	Normal,
	Meltdown,
	Roll_In,
	Expand_Roll_In
};

class CsPlatform : public CHit_Checker
{
public:
	CsPlatform();

	bool Check_Hit(double next_x_pos, double next_y_pos, CBall* ball);

	void Init();
	void Act();
	EPlatform_State Get_State();
	void Set_State(EPlatform_State new_state);
	void Redraw(); 
	void Draw(HDC hdc, RECT& paint_area);

	int X_Pos;																																// положение платформы по оси x
	int X_Step;																																// смещение платформы по оси x
	int Width;																																// ширина всей платформы (иеняется в зависимости от ситуации в игре)

private:
	void Clear_BG(HDC hdc);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc, RECT& paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
	void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
	void Draw_Expanding_Roll_In_State(HDC hdc, RECT& paint_area);
	bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, CBall* ball);

	EPlatform_State Platform_State;
	int Inner_Width;																													// ширина платформы между шариками
	int Rolling_Step;																													// шаг поворота шарика платформы во время вкатывания

	static const int Normal_Width = 28;

	int Meltdown_Platform_Y_Pos[Normal_Width * CsConfig::Global_Scale];				// положение Y расплавляющейся платформы


	HPEN Platform_Circle_pen, Platform_Inner_pen, Hightlight_Pen;
	HBRUSH Platform_Circle_brush, Platform_Inner_brush;
	RECT Platform_Rect, Prev_Platform_Rect;

	static const int Circle_Size = 7;																						// размер шарика платформы
	static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;	// нормальная ширина средней части платформы
	static const int Height = 7;																								// высота всей платформы (не меняется)
	static const int Meltdown_Speed = 3;																				// скорость расплавления платформы
	static const int Max_Rolling_Step = 16;																			// максимальный шаг вращения шарика платформы
	static const int Roll_In_Platform_End_X_Pos = 98;														// позиция шарика после выката
	static const int Rolling_Platform_Speed = 3;																// скорость выкатывания шарика платформы
};