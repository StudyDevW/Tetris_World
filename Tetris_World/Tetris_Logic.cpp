#include "Tetris_Logic.h"
#include "sdk.h"
#include <time.h>


bool c_logic::not_empty_line(int arr[20][10], int line) {
    for (int i = 0; i < 10; i++) 
        if (arr[line][i] == 0)
            return false;
    
    return true;
};

bool c_logic::not_empty_1ch(int arr[7]) {
    for (int i = 0; i < 7; i++)
        if (arr[i] == 0)
            return false;

    return true;
};

void c_logic::clear_elements(int arr[20][10], int line) {
    for (int y = 0; y <= line; y++)
        for (int i = 0; i < 10; i++) 
            if (arr[y][i] != 0) 
                arr[y][i] = 0;
}

void c_logic::backup_elements(int arr[20][10], int line) {
    if (line != 0)
        for (int y = 0; y < line; y++)
            for (int x = 0; x < 10; x++)
                sdk_game.m_flbackup_value[y][x] = arr[y][x];
}

bool c_logic::line_resolver(int line) {
    //проверяем заполненность линии
    if (not_empty_line(sdk_game.m_flfallenfigures, line)) {
        //если линия полностью заполнена
        if (sdk_game.line < 0) 
            sdk_game.line = line;
        
        return sdk_game.line != -1;
    }

    return false;
}

void c_logic::restore_pre_opperand(int arr[20][10]) {
    for (int y = 19; y >= 0; y--)
        for (int x = 0; x < 10; x++) 
            if (not_empty_line(sdk_game.m_flfallenfigures, y)) 
                sdk_game.collect_help_numbers = y + 1;
}

void c_logic::restore_elements(int arr[20][10]) {
    std::memset(sdk_game.m_flmassivephantom, 0, sizeof(sdk_game.m_flmassivephantom[0][0]) * 20 * 10);

    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 10; x++) 
            if (sdk_game.m_flbackup_value[y][x] == 2) 
                arr[y + (sdk_game.collect_help_numbers - sdk_game.line)][x] = sdk_game.m_flbackup_value[y][x];
   
    std::memset(sdk_game.m_flbackup_value, 0, sizeof(sdk_game.m_flbackup_value[0][0]) * 20 * 10);
   
    sdk_game.line = -1;
}

void c_logic::copy_to_dest(int in[20][10], int to[20][10], int y, int x) {
    to[y][x] = in[y][x];
}

void c_logic::init_fallen_elements() {
    for (int y = 0; y < 20; y++) 
        for (int x = 0; x < 10; x++) 
            if (sdk_game.m_flmassivephantom[y][x] == 2) 
                copy_to_dest(sdk_game.m_flmassivephantom, sdk_game.m_flfallenfigures, y, x);
}

void c_logic::copy_to_dest_custom(int in[20][10], int to[20][10], int y, int x, int b, std::string options) {
    if (options == "second_element_right") {
        to[y][x + b] = in[y][x];

        if (to[y][0] == 0) 
            to[y][0] = 1;
    }
    else if (options == "second_element_left") {
        to[y][x - b] = in[y][x];

        if (to[y][0] == 0)
            to[y][0] = 1;

        if (to[y][9] == 0)
            to[y][9] = 1;
    }
}

void c_logic::post_predict(int first[20][10], int second[20][10]) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {

            if (x < 9) 
                copy_to_dest_custom(first, sdk_game.pre_massive_right, y, x, 1, "second_element_right");

            if (x > 1) 
                copy_to_dest_custom(first, sdk_game.pre_massive_left, y, x, 1, "second_element_left");

            if (second[y][x] == 2) {

                if (sdk_game.pre_massive_right[y][x] == second[y][x]) {
                    sdk_game.m_flmoveright_block = true;
                    sdk_game.m_flrotate_block = true;
                }

                if (sdk_game.pre_massive_left[y][x] == second[y][x]) {
                    sdk_game.m_flmoveleft_block = true;
                    sdk_game.m_flrotate_block = true;
                }
            }
        }
    }
}

int c_logic::velocity_fall() {
    if (sdk_game.m_flscore < 500) {
        return 40;
    }
    else if (sdk_game.m_flscore >= 500 && sdk_game.m_flscore < 1000) {
        return 35;
    }
    else if (sdk_game.m_flscore >= 1000 && sdk_game.m_flscore < 1500) {
        return 25;
    }
    else if (sdk_game.m_flscore >= 1500 && sdk_game.m_flscore < 2000) {
        return 20;
    }
    else if (sdk_game.m_flscore >= 2000 && sdk_game.m_flscore < 4000) {
        return 15;
    }
    else if (sdk_game.m_flscore >= 4000 && sdk_game.m_flscore < 7000) {
        return 10;
    }
    else if (sdk_game.m_flscore >= 7000) {
        return 5; //unreal
    }
    else 
        return 0;
}

void c_logic::reset() {
    sdk_game.m_flscore = 0;
    std::memset(sdk_game.m_flmassive, 0, sizeof(sdk_game.m_flmassive[0][0]) * 20 * 10);
    std::memset(sdk_game.m_flmassivephantom, 0, sizeof(sdk_game.m_flmassivephantom[0][0]) * 20 * 10);
    std::memset(sdk_game.m_flfallenfigures, 0, sizeof(sdk_game.m_flfallenfigures[0][0]) * 20 * 10);

}

void c_logic::write_on_server() {
    for (int i = 1; i < 24; i++) {
        if (sdk_game.name == sdk_game.server.info_player[i].player) {
            if (sdk_game.m_flscore > std::stoi(sdk_game.server.info_player[i].score)) {
                sdk_game.server.write_player = sdk_game.name;
                sdk_game.server.write_score = sdk_game.m_flscore;
                sdk_game.server.write = true;
            }
        }
        else if (sdk_game.server.info_player[i].player != sdk_game.name) {
            if (sdk_game.m_flscore > 0) {
                sdk_game.server.write_player = sdk_game.name;
                sdk_game.server.write_score = sdk_game.m_flscore;
                sdk_game.server.write = true;
            }
        }
    }
}

void c_logic::lose_complete() {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (sdk_game.m_flmassive[y][x] == 2) {
                if (sdk_game.m_flmassive[y][x] == sdk_game.m_flfallenfigures[y][x]) {
                    write_on_server();
                    sdk_game.lose_game = true;
                    sdk_game.start_game = false;
                    reset();
                    sdk_game.server.need_resort = true;
                }
            }
        }
    }
}


void c_logic::predict_fall_figures() {

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (sdk_game.pre_figure_fall[y][x] == 2) {
                if (sdk_game.control_set.m_flcontrol_down
                    || sdk_game.control_set.m_flcontrol_left
                    || sdk_game.control_set.m_flcontrol_right) {
                    sdk_game.animation_set.color_scale = 255;
                    sdk_game.animation_set.anim_reset = false;
                    sdk_game.predict_timer_stop = false;
                    sdk_game.m_flfalltimer_predict = sdk_game.m_flfalltimer;
                }

             
                if (sdk_game.predict_timer_stop) {
                    if (sdk_game.pre_figure_fall[y][x] != sdk_game.m_flbottomline[y][x]
                        || (sdk_game.pre_figure_fall[y != 0 ? y - 1 : y][x] != sdk_game.m_flfallenfigures[y][x])) {
                        sdk_game.predict_timer_stop = false;
                    }
                }
            }
        }
    }
}

void c_logic::time_to_stack_setup(bool* lock_fall) {

    sdk_game.m_flfallboost = false;
    sdk_game.editor_s.in_setup = true;

    if (sdk_game.m_flpos_timer > 70) {

        std::memcpy(sdk_game.m_flmassivephantom, sdk_game.m_flmassive, sizeof(sdk_game.m_flmassive));
        init_fallen_elements();

        sdk_game.m_flfalltimer = 0;
        sdk_game.m_flmovefigure = 0;
        sdk_game.m_fltype_figure = 0;

        sdk_game.m_flnextfigure_s = true;
    
        

        std::memset(sdk_game.m_flmassive, 0, sizeof(sdk_game.m_flmassive[0][0]) * 20 * 10);

        sdk_game.m_flpos_timer = 0;
    } 
    else {
        *lock_fall = true;
        sdk_game.m_flpos_timer++;
    }
}

void c_logic::init() {

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (line_resolver(y)) {
                restore_pre_opperand(sdk_game.m_flfallenfigures);
                backup_elements(sdk_game.m_flfallenfigures, sdk_game.line);

                if (sdk_game.m_flfalltimer == 0) {
                    clear_elements(sdk_game.m_flfallenfigures, sdk_game.line);
                    restore_elements(sdk_game.m_flfallenfigures);
        
                    sdk_game.m_flscore += 100;
                }
            }

        }
    }

    if ((sdk_game.m_flfalltimer == 0) && !sdk_game.editor_s.in_setup) {
        sdk_game.editor_s.need_to_stack = true;
    }
    else if (sdk_game.editor_s.in_setup) {
        sdk_game.editor_s.need_to_stack = false;
    }

    if (sdk_game.m_flfalltimer < 2) {
        sdk_game.animation_set.color_scale = 255;
        sdk_game.animation_set.anim_reset = false;
    }

   
}
 
void c_logic::sort_table_leaders() {
    static int score_def[24];
    static int score_rel[24];
    static int score_int[24];
    static int score_int_rel[24];
    static int score_int_helper[24];

    //если проиграли то сбрасываем ранее записанные массивы
    if (sdk_game.server.need_resort) {
        std::memset(score_def, 0, sizeof(score_def[0]) * 24);
        std::memset(score_rel, 0, sizeof(score_rel[0]) * 24);
        std::memset(score_int, 0, sizeof(score_int[0]) * 24);
        std::memset(score_int_rel, 0, sizeof(score_int_rel[0]) * 24);
        std::memset(score_int_helper, 0, sizeof(score_int_helper[0]) * 24);
        sdk_game.server.need_resort = false;
    }

    for (int i = 1; i < 24; i++) {
        if (sdk_game.server.info_player[i].score != "") {
            score_def[i - 1] = std::stoi(sdk_game.server.info_player[i].score) + i; //прибавляем индекс (довольно костыльный метод)
            score_rel[i - 1] = std::stoi(sdk_game.server.info_player[i].score); //обычная сортировка счета без добавления индекса
        }
    }

    for (int i = 1; i < 24; i++) {
        if (score_def[i - 1] > 0 && score_int_helper[i - 1] == score_int[i - 1]) {
            score_int[i - 1] = score_def[i - 1];
            score_int_rel[i - 1] = score_rel[i - 1];
            score_int_helper[i - 1] = score_def[i - 1];
        }

        std::sort(std::begin(score_int), std::end(score_int), std::greater<>());
        std::sort(std::begin(score_int_rel), std::end(score_int_rel), std::greater<>());
    }

    for (int i = 1; i < 24; i++) {
        if (score_int_helper[i - 1] != score_int[i - 1]) {

            sdk_game.server.sorted_score[i] = score_int_rel[i - 1];

            if (score_int[i - 1] >= 100 && score_int[i - 1] < 1000) {
                sdk_game.server.sorted_player[i] = score_int[i - 1] % 100;
            }
            else if (score_int[i - 1] >= 1000 && score_int[i - 1] < 10000) {
                sdk_game.server.sorted_player[i] = score_int[i - 1] % 100;
            }
            else if (score_int[i - 1] >= 10000 && score_int[i - 1] < 100000) {
                sdk_game.server.sorted_player[i] = score_int[i - 1] % 100;
            }
        }
    }
}

void c_logic::predict_figures() {
    static int values[7] = { 1, 2, 3, 4, 5, 6, 7 };

   

    if (sdk_game.start_randomize) {
        srand(time(0));
        std::random_shuffle(std::begin(values), std::end(values));
        sdk_game.start_randomize = false;
    }

    if (sdk_game.m_flnextfigure_s) {
        for (int j = 1; j < 7; j++) 
            values[j - 1] = values[j];
  
        values[6] = 0;
        sdk_game.m_flnextfigure_s = false;
    }

    if ((values[2] == 0)) {
        int value_def = 0;
        for (int i = 2; i < 7; i++) {
            value_def++;
            
            values[i] = value_def;
        }

        srand(time(0));
        std::random_shuffle(std::begin(values) + 2, std::end(values));
    }
  
   

    sdk_game.m_flnextfigure = values[1];
    sdk_game.m_flsetnextfigure = values[0];
}