#include "Engine.h"

enum EBrick_Type
{
	None,
	Red,
	Blue
};

HPEN Hightlight_Pen, Brick_Red_pen, Brick_Blue_pen, Platform_Circle_pen, Platform_Inner_pen;
HBRUSH Brick_Red_brush, Brick_Blue_brush, Platform_Circle_brush, Platform_Inner_brush;

const int Global_Scale = 4;											// ���������� ������� ���� ��������� ����
const int Brick_Width = 15;											// ������ �������� ���� ������
const int Brick_Height = 7;											// ������ �������� ���� ������
const int Cell_Width = Brick_Width + 1;					// ������ ������ (������ + 1 ���������� ������)
const int Cell_Height = Brick_Height + 1;				// ������ ������ (������ + 1 ���������� ������)
const int Level_X_Offset = 8;										// �������� �������� ������ �� ��� X �� ������ ��������� ������
const int Level_Y_Offset = 6;										// �������� �������� ������ �� ��� Y �� ������ ��������� ������
const int Circle_Size = 7;											// ������ ������ ���������

int Inner_Width = 21;														// ������ ��������� ����� ��������

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
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Init()
{	//��������� ���� ��� ������
	Hightlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	Create_Pen_Brush(255, 60, 55, Brick_Red_pen, Brick_Red_brush);
	Create_Pen_Brush(85, 250, 155, Brick_Blue_pen, Brick_Blue_brush);
	Create_Pen_Brush(151, 0, 0, Platform_Circle_pen, Platform_Circle_brush);
	Create_Pen_Brush(0, 128, 192, Platform_Inner_pen, Platform_Inner_brush);
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
void Draw_Platform(HDC hdc, int x, int y)
{	//��������� �������� ���� ���������
	
	// 1. ������ ������� ������ ���������
	SelectObject(hdc, Platform_Circle_pen);
	SelectObject(hdc, Platform_Circle_brush);

	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);

	// 2. ������ ���� �� ����� ������
	SelectObject(hdc, Hightlight_Pen);

	Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale, 
					 (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);
	Arc(hdc, (x + 1) * Global_Scale + 1, (y + 1) * Global_Scale + 1, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale, 
					 (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);

	// 3. ������ ��������� ����� ��������
	SelectObject(hdc, Platform_Inner_pen);
	SelectObject(hdc, Platform_Inner_brush);

	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 4 * Global_Scale, 4 * Global_Scale);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc) 
{	//��������� ������ ����
	Draw_Level(hdc);
	Draw_Platform(hdc, 50, 100);
}  
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
