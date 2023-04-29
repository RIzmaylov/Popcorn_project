﻿#pragma once

#include <Windows.h>


const int Timer_Id = WM_USER + 1;								// ID процесса юзера

enum class EKey_Type
{
	Left,
	Right,
	Space
};

void Init_Engine(HWND hwnd);
void Draw_Frame(HDC hdc ,RECT& paint_area);
int On_Key_Down(EKey_Type key_type);
int On_Timer();