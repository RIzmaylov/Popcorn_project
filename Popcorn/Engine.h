#pragma once

#include <Windows.h>


enum class EKey_Type
{
	Left,
	Right,
	Space
};

void Init_Engine(HWND hwnd);
void Draw_Frame(HDC hdc ,RECT& paint_area);
int On_Key_Down(EKey_Type key_type);