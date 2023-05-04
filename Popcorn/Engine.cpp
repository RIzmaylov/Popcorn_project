#include "Engine.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsEngine::CsEngine() :
	Hwnd(0), BG_Pen(0), BG_Brush(0)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Init_Engine(HWND hwnd)
{	//Настройка игры при старте

	Hwnd = hwnd;

	// создание кистей и карандашей
	CsConfig::Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
	Border.Init();
	Platform.Init();
	Ball.Init();
	// инициализация габаритов игрового уровня
	Level.Init();

	// очищение области платформы при первом запуске (чтобы не оставался след при первом перемещении платформы)
	Platform.Redraw(Hwnd);

	// установка таймера
	SetTimer(Hwnd, Timer_Id, 1000 / CsConfig::FPS, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Draw_Frame(HDC hdc, RECT& paint_area) 
{	// Отрисовка экрана игры

	Level.Draw(Hwnd, hdc, paint_area);

	Platform.Draw(hdc, paint_area, BG_Pen, BG_Brush);
	
	Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

	Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);

	//for (int i = 0; i < 16; ++i) {
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBrick_Type::Blue, ELetter_Type::O, i);
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBrick_Type::Red, ELetter_Type::O, i);
	//}
}    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsEngine::On_Key_Down(EKey_Type key_type) 
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= CsConfig::Border_X_Offset)
			Platform.X_Pos = CsConfig::Border_X_Offset;

		Platform.Redraw(Hwnd);
		break;

	case EKey_Type::Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos >= CsConfig::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = CsConfig::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw(Hwnd);
		break;

	case EKey_Type::Space:
		break;
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsEngine::On_Timer()
{
	Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);

	Level.Active_Brick.Act(Hwnd);

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
