#include "Engine.h"

enum EBrick_Type
{
	None,
	Red,
	Blue
};

HPEN Brick_Red_pen, Brick_Blue_pen;
HBRUSH Brick_Red_brush, Brick_Blue_brush;

const int Global_Scale = 4;											// ���������� ������� ���� ��������� ����
const int Brick_Width = 15;											// ������ �������� ���� ������
const int Brick_Height = 7;											// ������ �������� ���� ������
const int Cell_Width = Brick_Width + 1;					// ������ ������ (������ + 1 ���������� ������)
const int Cell_Height = Brick_Height + 1;				// ������ ������ (������ + 1 ���������� ������)
const int Level_X_Offset = 8;										// �������� �������� ������ �� ��� X �� ������ ��������� ������
const int Level_Y_Offset = 6;										// �������� �������� ������ �� ��� Y �� ������ ��������� ������

char Level_01[14][12] = {												// ������ �������� ������
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Init()
{	//��������� ���� ��� ������

	Brick_Red_pen = CreatePen(PS_SOLID, 0, RGB(255, 60, 55));
	Brick_Blue_pen = CreatePen(PS_SOLID, 0, RGB(85, 250, 155));
	Brick_Red_brush = CreateSolidBrush(RGB(255, 55, 55));
	Brick_Blue_brush = CreateSolidBrush(RGB(85, 255, 255));

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{	//��������� �������� ���� - �������

	HPEN pen;
	HBRUSH brush;



	switch (brick_type)
	{
	case EBrick_Type::None: return;
	case EBrick_Type::Red:
		pen = Brick_Red_pen;
		brush = Brick_Red_brush;
		break;
	case EBrick_Type::Blue:
		pen = Brick_Blue_pen;
		brush = Brick_Blue_brush;
		break;
	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 
		2 * Global_Scale, 2 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{	//��������� ������ ����
	for (int i = 0; i < 14; ++i) 
		for (int j = 0; j < 12; ++j) 
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, static_cast<EBrick_Type>(Level_01[i][j]));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc) 
{	//��������� ������ ����
	Draw_Level(hdc);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
