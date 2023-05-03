﻿#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

const int Timer_Id = WM_USER + 1;								// ID процесса юзера
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class EKey_Type
{
	Left,
	Right,
	Space
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CsEngine
{
public:
	CsEngine();
	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;


private:
	CBall Ball;
	CLevel Level;
	CsPlatform Platform;
	CsBorder Border;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------