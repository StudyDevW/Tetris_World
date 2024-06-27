#pragma once
#include <wtypes.h>
#include <string>
#include <algorithm>

class c_logic {
public:
	enum in_logic { PRE_RANDOM, POST_RANDOM };
	bool not_empty_line(int massive[20][10], int line);
	bool not_empty_1ch(int arr[7]);
	void clear_elements(int arr[20][10], int line);
	void clear_elements_with_animation(int arr[20][10], int line);
	void backup_elements(int arr[20][10], int line);

	bool line_resolver(int line);


	void restore_pre_opperand(int arr[20][10]);
	void restore_elements(int arr[20][10]);
	void copy_to_dest(int in[20][10], int to[20][10], int y, int x);
	void init_fallen_elements();
	void copy_to_dest_custom(int in[20][10], int to[20][10], int y, int x, int b, std::string options);
	void post_predict(int first[20][10], int second[20][10]);
	int velocity_fall();
	void reset();
	void write_on_server();
	void lose_complete();
	void predict_fall_figures();
	void time_to_stack_setup(bool* lock_fall);
	void init();
	void sort_table_leaders();
	void predict_figures();
};

extern c_logic* logic;