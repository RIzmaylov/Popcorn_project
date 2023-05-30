#include "Engine.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CsEngine::CsEngine() :
	Game_State(EGame_States::Play_Level)
{}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Init_Engine(HWND hwnd)
{	//Настройка игры при старте

	CsConfig::Hwnd = hwnd;

	// создание кистей и карандашей фона и кирпичей
	CActive_Brick::Setup_Colors();

	// инициализация габаритов игрового уровня
	Level.Init();
	Platform.Init();
	Ball.Init();
	Border.Init();

	CBall::Add_Hit_Checker(&Border);
	CBall::Add_Hit_Checker(&Level);
	CBall::Add_Hit_Checker(&Platform);

	Level.Set_Current_Level(CLevel::Level_01);

	Ball.Set_State(EBall_State::Normal, Platform.X_Pos + Platform.Width / 2);
	// инициализация выкатывания платформы
	Platform.Set_State(EPlatform_State::Normal);
	// очищение области платформы при первом запуске (чтобы не оставался след при первом перемещении платформы)
	Platform.Redraw();

	// установка таймера
	SetTimer(CsConfig::Hwnd, Timer_Id, 1000 / CsConfig::FPS, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CsEngine::Draw_Frame(HDC hdc, RECT& paint_area) 
{	// Отрисовка экрана игры
	SetGraphicsMode(hdc, GM_ADVANCED);

	Level.Draw(hdc, paint_area);

	Border.Draw(hdc, paint_area);

	Platform.Draw(hdc, paint_area);

	Ball.Draw(hdc, paint_area);

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

		Platform.Redraw();
		break;


	case EKey_Type::Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos >= CsConfig::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = CsConfig::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw();
		break;


	case EKey_Type::Space:
		if (Platform.Get_State() == EPlatform_State::Ready)
		{
			Ball.Set_State(EBall_State::Normal, Platform.X_Pos + Platform.Width / 2);
			Platform.Set_State(EPlatform_State::Normal);
		}
		break;
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int CsEngine::On_Timer()
{
	++CsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGame_States::Test_Ball:
		Ball.Set_For_Test();
		Game_State = EGame_States::Play_Level;
		break;


	case EGame_States::Play_Level:
		Ball.Move();

		if (Ball.Get_State() == EBall_State::Lost)
		{
			Game_State = EGame_States::Lost_Ball;
			Platform.Set_State(EPlatform_State::Meltdown);
		}

		if (Ball.Is_Test_Finished())
			Game_State = EGame_States::Test_Ball;
		break;


	case EGame_States::Lost_Ball:
		if (Platform.Get_State() == EPlatform_State::Missing)
		{
			Game_State = EGame_States::Restart_Level;
			Platform.Set_State(EPlatform_State::Roll_In);
		}
		break;


	case EGame_States::Restart_Level:
		if (Platform.Get_State() == EPlatform_State::Ready)
		{
			Game_State = EGame_States::Play_Level;
			Ball.Set_State(EBall_State::On_Platform, Platform.X_Pos + Platform.Width / 2);
		}
		break;
	}

	Platform.Act();

	Level.Act();

	//if (CsConfig::Current_Timer_Tick % 10 == 0)

	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
