#pragma once
#include <wtypes.h>

class interface_t {
public:
	void setup_fonts(HDC hdc);
	void delete_fonts();
	void create_user();
	void check_already_name();
	bool check_change_name();
	bool create_button(HDC hdc, const wchar_t* text, int x, int y);
	void make_table(HWND hwnd, HDC hdc);
	void main_menu(HWND hwnd, HDC hdc);
};

extern interface_t* interfaces;