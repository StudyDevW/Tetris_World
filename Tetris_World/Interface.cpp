#include "Interface.h"
#include "sdk.h"
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <algorithm>

void interface_t::create_user() {
	FILE* fout;
	CreateDirectory(L"c:\\Tetris", NULL);

	fopen_s(&fout, "c:\\Tetris\\create_user.txt", "wt");

	if (fout == NULL) {
		return;
	}

	fprintf(fout, "%s", "введите_ник");
	fclose(fout);


	system("Start c:\\Tetris\\create_user.txt");
	sdk_game.next_create_nick = true;
}

void interface_t::check_already_name() {
	std::string name;
	std::ifstream ifs("c:\\Tetris\\create_user.txt", std::ifstream::in);
	ifs >> name;

	if ((name != "введите_ник") && (name != "")) {
		sdk_game.name = name;
		sdk_game.created_nick = true;
		sdk_game.name_changed = true;
	}
}

bool interface_t::create_button(HDC hdc, const wchar_t* text, int x, int y) {
	RECT button_box = { 0 + x, 0 + y, 150 + x, 40 + y };
	RECT text_pos = { button_box.left, button_box.top, button_box.right, button_box.bottom };
	Rectangle(hdc, button_box.left, button_box.top, button_box.right, button_box.bottom);

	DrawText(hdc, (LPCWSTR)text, -1, &text_pos, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (sdk_game.control_set.m_flcontrol_mouse_x < button_box.right
		&& sdk_game.control_set.m_flcontrol_mouse_x > button_box.left
		&& sdk_game.control_set.m_flcontrol_mouse_y < button_box.bottom
		&& sdk_game.control_set.m_flcontrol_mouse_y > button_box.top) {
		sdk_game.control_set.m_flmouse_reset = true;
		return true;
	}

	return false;
}

void interface_t::make_table(HWND hwnd, HDC hdc) {
	sdk_game.server.check = true;
	int last_y = 0;
	Rectangle(hdc, 120, 150, 120 + 278, 125 + 325);

	int pre_score[10];


	for (int i = 1; i < 10; i++) {
		RECT id_text = { 125, 155 + last_y, 150, 175 + last_y };
		RECT player_text = { 150, 155 + last_y, 398, 175 + last_y };
		RECT score_text = { 320, 155 + last_y, 398, 175 + last_y };

		std::string sorted_score_completed[256];
		

		sorted_score_completed[i] = std::to_string(sdk_game.server.sorted_score[i]);

		std::string empty_message[256];

		if (sdk_game.server.sorted_player[i] > 0) {

			static const std::string last_player_on_top = sdk_game.server.info_player[sdk_game.server.sorted_player[1]].player;

			if (sdk_game.server.info_player[sdk_game.server.sorted_player[1]].player == last_player_on_top) {
				sdk_game.server.info_player[sdk_game.server.sorted_player[1]].player += " [лучший игрок]";
			}

			DrawTextA(hdc, sdk_game.server.info_player[sdk_game.server.sorted_player[i]].player.c_str(), -1, &player_text, DT_SINGLELINE);
			DrawTextA(hdc, std::to_string(i).c_str(), -1, &id_text, DT_SINGLELINE | DT_CENTER);


			if (sorted_score_completed[i] != "0")
				DrawTextA(hdc, sorted_score_completed[i].c_str(), -1, &score_text, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


			MoveToEx(hdc, 120, 180 + last_y, NULL);
			LineTo(hdc, 120 + 278, 180 + last_y);
			last_y += 30;
		}
		else if (sdk_game.server.sorted_player[1] == 0) {
			

			empty_message[3] = "Таблица лидеров пуста";
			empty_message[4] = "Займите топ первыми!";

			if (empty_message[i] != "")
				DrawTextA(hdc, empty_message[i].c_str(), -1, &player_text, DT_SINGLELINE);

			last_y += 30;
		}
	}
	return;
}

void interface_t::main_menu(HWND hwnd, HDC hdc) {

	if (!sdk_game.next_create_nick && !sdk_game.name_changed) {
		if (create_button(hdc, L"Введите никнейм", 185, 200)) {
			create_user();
		}
	}



	RECT title_text_rect = { 120, 40, 398, 100 };

	DrawText(hdc, L"The Creation", -1, &title_text_rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);


	if (sdk_game.created_nick) {

		make_table(hwnd, hdc);

		if (create_button(hdc, L"Играть", 185, 500)) {
			sdk_game.start_game = true;
			sdk_game.lose_game = false;
	
		}

		RECT title_text = { 120, 25 + 40, 398, 25 + 100 };

		DrawTextA(hdc, sdk_game.name.c_str(), -1, &title_text, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

}
