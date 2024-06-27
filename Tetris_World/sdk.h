#pragma once
#include <wtypes.h>
#include <string>

class sdk {
public:

	struct animation {
		int color_scale = 255;
		bool anim_reset = false;
	};

	struct controls {
		int m_flcontrol_mouse_x;
		int m_flcontrol_mouse_y;
		bool m_flmouse_reset = false;
		bool m_flcontrol_down = false;
		bool m_flcontrol_space = false;
		bool m_flcontrol_right = false;
		bool m_flcontrol_left = false;
	};

	struct info_players {
		std::string score;
		std::string player;
	};

	struct editor {
		bool need_to_stack = false;
		bool in_setup = false;
	};

	editor editor_s;

	struct server_connect {
		const char* host = "sql11.freemysqlhosting.net";
		const char* user = "sql11647000";
		const char* pass = "lIwVdp6wDk";
		bool write = false;
		bool check = false;
		uint64_t max_rows = 0;

		info_players info_player[256];

		bool need_resort = false;
		int sorted_player[256];
		int sorted_score[256];

		std::string write_player = "none";
		int write_score = 0;
		bool losed_game = false;
	};

	std::string m_flscore_table[2][10];
	bool start_game = false;
	bool lose_game = true;
	bool next_create_nick = false;
	bool name_changed = false;
	bool created_nick = false;
	std::string name_player = "";
	std::string name = "введите_ник";
	char* biba[128];
	std::string name_1 = "";

	animation animation_set;
	controls control_set;
	server_connect server;
	int pre_massive_center[20][10];
	int pre_massive_left[20][10];
	int pre_massive_right[20][10];
	int pre_figure_fall[20][10];
    int timer = 0;
	float timer_fall_predict = 0.0f;
	float m_flfalltimer_predict = 0.0f;
	bool predict_timer_stop = false;
	int line = -1;
	int m_flpos_timer = 0;
	int collect_help_numbers = 0;
	bool stop_timer = false;
	
	bool m_flmoveleft_block = false;
	bool m_flmoveright_block = false;
	bool m_flfallboost = false;
	bool m_flmoveleft_block_1 = false;
	bool m_flmoveright_block_1 = false;
	bool m_flreturn_block = false;
	bool m_flrotate_block = false;
	bool m_flcustomupdated = false;
	bool checked = false;
	int y_line_clear = -1;
	int m_flmassive[20][10];
	int m_flmassiveprev[20][10];
	
	int m_flmassiveblock[20][10];
	bool start_randomize = true;
	int y_lines_down = 0;
	int m_flmassivephantom[20][10];
	int m_flmassivephantomcopy[20][10];
	int m_flmassivephantomcopycopy[20][10];
	int m_flchecklinehelper[20][10];
	int m_flcenterfield[20][10];
	int m_floutmassive_left[20][10];
	int m_floutmassive_right[20][10];
	int m_flcheckmassive[20][10];
	int m_flmassivepreviouspos[20][10];
	int m_flmassivephantom_bold[20][10];
	int m_flmassivecollision[20][10];
	int m_flscore = 0;
	bool m_fllock_fall = false;
	int m_flcurrencyindex;
	int m_fltype_figure = 0;
	int m_flmovefigure = 0;
	int m_fltimer_figure[10];
	int m_flfalltimer = 0;
	int m_flbottomline[20][10];
	int m_flfallenfigures[20][10];
	int m_flbackup_value[20][10];
	int m_flpredict_figures[20][10];
	int m_flfig_ccc[5][7];
	int m_flnextfigure = -1;
	int m_flsetnextfigure = -1;
	bool m_flnextfigure_s = false;

	const char* last_pos = "none";


	void reset_controls() {
		control_set.m_flcontrol_down = false;
		control_set.m_flcontrol_space = false;
		control_set.m_flcontrol_right = false;
		control_set.m_flcontrol_left = false;
	}
};

extern sdk sdk_game;