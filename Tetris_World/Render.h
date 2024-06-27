#pragma once
#include <wtypes.h>
#include <string>
#include <algorithm>

enum types_massive {
	Field = 1,
	SetupFigures,
	FigureJ_Bottom,
	FigureJ_Top,
	FigureJ_Right,
	FigureL_Left,
	FigureL_Bottom,
	FigureL_Top,
	FigureL_Right,
	Other
};

class c_render {
	
public:
	void OnUpdate();
	types_massive FigureOptionPrev(std::string type_figure);
	types_massive FigureOption();
	void next_figures(HWND hwnd, HDC hdc);
	void score_render(HWND hwnd, HDC hdc);
	COLORREF AnimatedColor();
	void native_rendered_predict(HDC hdc, HPEN hpen, int a, int b);
	void main_render(HWND hwnd, HDC hdc);
	void draw_background(HDC hdc, RECT rect);
	void fixed_wmpaint(HWND hwnd);
};

extern c_render* render;