#include "Engine.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsEngine::CsEngine() :
	Hwnd(0)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Init_Engine(HWND hwnd)
{	//Настройка игры при старте

	Hwnd = hwnd;

	// создание кистей и карандашей фона и кирпичей
	CActive_Brick::Setup_Colors();

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

	Platform.Draw(hdc, paint_area);
	
	Ball.Draw(hdc, paint_area);

	Border.Draw(hdc, paint_area);

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
	++CsConfig::Current_Timer_Tick;

	Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);

	Level.Active_Brick.Act(Hwnd);

	if (CsConfig::Current_Timer_Tick % 10 == 0)
		Platform.Act(Hwnd);

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
