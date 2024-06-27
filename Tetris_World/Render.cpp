#include "Render.h"
#include "Tetris_Logic.h"
#include "sdk.h"
#include "Interface.h"
#include "Tetris_Editor.h"

editor_t* editor;
interface_t* interfaces;
c_logic* logic;

void c_render::OnUpdate() {

    editor->init();
    logic->lose_complete();

    if (!sdk_game.predict_timer_stop) {
        sdk_game.m_flfalltimer_predict += 0.5f;
    }

    if (sdk_game.m_flfalltimer == 0) {
        sdk_game.predict_timer_stop = false;
        sdk_game.m_flfalltimer_predict = 0;
    }

    logic->predict_fall_figures();

   

    if (!sdk_game.m_fllock_fall) {
        if (sdk_game.m_flfallboost ? sdk_game.timer > 1 : sdk_game.timer > logic->velocity_fall()) {
            sdk_game.m_flfalltimer++;

            sdk_game.timer = 0;
        }
        else {
            sdk_game.timer++;
        }
    }

    if (sdk_game.m_flfalltimer >= 0) {
        sdk_game.m_fllock_fall = false;
    }

    if (sdk_game.control_set.m_flcontrol_down)
        sdk_game.m_fltype_figure++;

    if (sdk_game.m_fltype_figure > 3)
        sdk_game.m_fltype_figure = 0;

    if (sdk_game.control_set.m_flcontrol_left) {
        sdk_game.m_flmovefigure--;
    }

    if (sdk_game.control_set.m_flcontrol_right) {
        sdk_game.m_flmovefigure++;
    }

    int finded_massive = -1;
    int finded_massive_b = -1;
    int finded_massive_bottom = -1;

    static bool left_check = false;
    static bool right_check = false;
    static bool between_fantoms_mode = false;

    if (finded_massive == -1 || finded_massive_b == -1) {
        sdk_game.m_flmoveleft_block = false;
        sdk_game.m_flrotate_block = false;
        sdk_game.m_flmoveright_block = false;

        right_check = false;
        left_check = false;
        sdk_game.m_flrotate_block = false;
    }

    logic->init();

    for (int y = 0; y < 20; y++) {

        for (int x = 0; x < 10; x++) {

            if ((sdk_game.m_flmassive[y != 0 ? y - 1 : y][x] == sdk_game.m_flbottomline[y][x])) {
                sdk_game.m_flfallboost = false;
            }

            if (sdk_game.m_flbottomline[y][x] == 2) {
                if ((sdk_game.m_flmassive[y][x] == sdk_game.m_flbottomline[y][x])) {
                    logic->time_to_stack_setup(&sdk_game.m_fllock_fall);
                }
            }

            if (sdk_game.m_flfallenfigures[y][x] == 2) {
                if ((sdk_game.m_flmassive[y != 0 ? y - 1 : y][x] == sdk_game.m_flfallenfigures[y][x]) && sdk_game.m_flfalltimer > 0) {
                    logic->time_to_stack_setup(&sdk_game.m_fllock_fall);
                }
            }

            if ((sdk_game.pre_figure_fall[y][x] == sdk_game.m_flbottomline[y][x])
                || (sdk_game.pre_figure_fall[y != 0 ? y - 1 : y][x] == sdk_game.m_flfallenfigures[y][x])) {

                sdk_game.predict_timer_stop = true;
            }
        }

        if (sdk_game.m_flmassive[y][0] == sdk_game.m_floutmassive_left[y][0]
            || sdk_game.m_flmassive[y][9] == sdk_game.m_floutmassive_right[y][9]) {
            finded_massive = y;
        }

        if (finded_massive > -1) {
            if (sdk_game.m_flmassive[finded_massive][0] == sdk_game.m_floutmassive_left[y][0]) {
                sdk_game.m_flmoveleft_block = true;
            }
            else if (sdk_game.m_flmassive[finded_massive][9] == sdk_game.m_floutmassive_right[y][9]) {
                sdk_game.m_flmoveright_block = true;
            }
            else {
                finded_massive = -1;
            }
        }


        if (sdk_game.m_flmassiveblock[y][0] == sdk_game.m_floutmassive_left[y][0]
            || sdk_game.m_flmassiveblock[y][9] == sdk_game.m_floutmassive_right[y][9]) {
            finded_massive_b = y;
        }

        if (finded_massive_b > -1) {
            if (sdk_game.m_flmassiveblock[finded_massive_b][0] == sdk_game.m_floutmassive_left[y][0]) {
                left_check = true;
            }
            else if (sdk_game.m_flmassiveblock[finded_massive_b][9] == sdk_game.m_floutmassive_right[y][9]) {
                right_check = true;
            }
            else {
                finded_massive_b = -1;
            }
        }

     
    }

 

    if (!(sdk_game.m_flmoveleft_block && sdk_game.m_flmoveright_block)) {
        if (sdk_game.m_flmoveright_block) {
            sdk_game.last_pos = "right";
        }
        else if (sdk_game.m_flmoveleft_block) {
            sdk_game.last_pos = "left";
        }
    }

    if (left_check && right_check) {
        sdk_game.m_flrotate_block = true;
    }

    logic->post_predict(sdk_game.m_flmassive, sdk_game.m_flfallenfigures);
}

types_massive c_render::FigureOptionPrev(std::string type_figure) {

  

    if (sdk_game.m_flnextfigure == 1) {
        sdk_game.m_flmassiveprev[0][1] = SetupFigures;
        sdk_game.m_flmassiveprev[1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[2][0] = SetupFigures;
        sdk_game.m_flmassiveprev[2][1] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 2) {
        sdk_game.m_flmassiveprev[0][0] = SetupFigures;
        sdk_game.m_flmassiveprev[1][0] = SetupFigures;
        sdk_game.m_flmassiveprev[2][0] = SetupFigures;
        sdk_game.m_flmassiveprev[2][1] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 3) {
        sdk_game.m_flmassiveprev[0][1] = SetupFigures;
        sdk_game.m_flmassiveprev[1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[2][1] = SetupFigures;
        sdk_game.m_flmassiveprev[3][1] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 4) {
        sdk_game.m_flmassiveprev[0 + 1][0] = SetupFigures;
        sdk_game.m_flmassiveprev[0 + 1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[1 + 1][0] = SetupFigures;
        sdk_game.m_flmassiveprev[1 + 1][1] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 5) {
        sdk_game.m_flmassiveprev[0][0] = SetupFigures;
        sdk_game.m_flmassiveprev[1][0] = SetupFigures;
        sdk_game.m_flmassiveprev[1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[2][0] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 6) {
        sdk_game.m_flmassiveprev[1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[1][2] = SetupFigures;
        sdk_game.m_flmassiveprev[2][0] = SetupFigures;
        sdk_game.m_flmassiveprev[2][1] = SetupFigures;
    }
    else if (sdk_game.m_flnextfigure == 7) {
        sdk_game.m_flmassiveprev[1][0] = SetupFigures;
        sdk_game.m_flmassiveprev[1][1] = SetupFigures;
        sdk_game.m_flmassiveprev[2][1] = SetupFigures;
        sdk_game.m_flmassiveprev[2][2] = SetupFigures;

    }

    return SetupFigures;
}

types_massive c_render::FigureOption() {

    if (sdk_game.m_flsetnextfigure == 1) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 2) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 3) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[3 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[3 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[3 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[3 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 4) {

        sdk_game.m_flmassive[1 + 0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassive[1 + 0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassive[1 + 1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassive[1 + 1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;



        sdk_game.m_flmassiveblock[1 + 0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassiveblock[1 + 0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassiveblock[1 + 1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.m_flmassiveblock[1 + 1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

    }
    else if (sdk_game.m_flsetnextfigure == 5) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 6) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }

    }
    else if (sdk_game.m_flsetnextfigure == 7) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 1: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 2: {
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.m_flmassive[0 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassive[2 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[1 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.m_flmassiveblock[2 + sdk_game.m_flfalltimer][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }

    }
    //predict
    if (sdk_game.m_flsetnextfigure == 1) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 2) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 3) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[3 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[3 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 3 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 4) {

        sdk_game.pre_figure_fall[1 + 0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.pre_figure_fall[1 + 0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.pre_figure_fall[1 + 1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
        sdk_game.pre_figure_fall[1 + 1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;




    }
    else if (sdk_game.m_flsetnextfigure == 5) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        }
    }
    else if (sdk_game.m_flsetnextfigure == 6) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            break;
        }
        }

    }
    else if (sdk_game.m_flsetnextfigure == 7) {
        switch (sdk_game.m_fltype_figure) {
        case 0: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 1: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 2: {
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 2 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        case 3: {
            sdk_game.pre_figure_fall[0 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[1 + (int)sdk_game.m_flfalltimer_predict][3 + 1 + sdk_game.m_flmovefigure] = SetupFigures;
            sdk_game.pre_figure_fall[2 + (int)sdk_game.m_flfalltimer_predict][3 + 0 + sdk_game.m_flmovefigure] = SetupFigures;

            break;
        }
        }

    }

    return SetupFigures;
}

void c_render::next_figures(HWND hwnd, HDC hdc) {
    logic->predict_figures();
    Rectangle(hdc, 410, 120, 480, 125 + 110);
    
    MoveToEx(hdc, 410, 135, NULL);
    LineTo(hdc, 480, 135);

    for (int a = 0; a < 10; a++) {
        for (int b = 0; b < 10; b++) {

            sdk_game.m_flmassiveprev[a][b] = Field;

            if (sdk_game.m_flmassiveprev[a][b] == FigureOptionPrev("JShape")) {
        //        Rectangle(hdc, 420 + b * 17, 155 + a * 17, 420 + b * 17 + 15, 155 + a * 17 + 15);

                RoundRect(hdc, 420 + b * 17, 155 + a * 17, 420 + b * 17 + 15, 155 + a * 17 + 15, 3, 3);

            }
        }
    }

}

void c_render::score_render(HWND hwnd, HDC hdc) {
    RECT text_rect = { 120, 40, 120 + 278, 100 };

    DrawText(hdc, std::to_wstring(sdk_game.m_flscore).c_str(), -1, &text_rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

COLORREF c_render::AnimatedColor() {

    static int timered_scale = 0;

  

    static int r = 255;
    static int g = 255;
    static int b = 255;
   

    if (!sdk_game.animation_set.anim_reset) {

        if (timered_scale > 2) {
            sdk_game.animation_set.color_scale -= 15;
            timered_scale = 0;
        }
        else {
            timered_scale++;
        }

        r = sdk_game.animation_set.color_scale;
        g = sdk_game.animation_set.color_scale;
        b = sdk_game.animation_set.color_scale;

  

        if (sdk_game.animation_set.color_scale <= 0) {
            sdk_game.animation_set.anim_reset = true;
        }

        return RGB(r, g, b);
    }
    else {
        sdk_game.animation_set.color_scale = 0;
        r = 0;
        g = 0;
        b = 0;
    }
}

void c_render::native_rendered_predict(HDC hdc, HPEN hpen, int a, int b) {
    
    if (sdk_game.m_flmassive[a][b] == 2) {
        if (sdk_game.pre_figure_fall[a][b] == sdk_game.m_flmassive[a][b])
            return;
    }

    if (sdk_game.pre_figure_fall[a][b] == FigureOption() && sdk_game.predict_timer_stop && sdk_game.m_flfalltimer > 0) {
        SelectObject(hdc, hpen);
        RoundRect(hdc, 125 + b * 27, 125 + a * 27, 125 + b * 27 + 25, 125 + a * 27 + 25, 5, 5);
        MoveToEx(hdc, 125 + b * 27, 125 + a * 27, NULL);
        LineTo(hdc, 125 + b * 27 + 25, 125 + a * 27 + 25);
        DeleteObject(hpen);
    }
}

void c_render::main_render(HWND hwnd, HDC hdc) {

    score_render(hwnd, hdc);
    next_figures(hwnd, hdc);

    HPEN hpen;
    hpen = CreatePen(PS_SOLID, 1, AnimatedColor());

    Rectangle(hdc, 120, 120, 120 + 278, 125 + 545);

 

    //main render function
    for (int a = 0; a < 20; a++) {
        for (int b = 0; b < 10; b++) {

        

            sdk_game.m_flmassive[a][b] = Field;

            sdk_game.pre_figure_fall[a][b] = Field;

            sdk_game.m_flmassiveblock[a][b] = Field;

            sdk_game.m_floutmassive_left[a][0] = SetupFigures;

            sdk_game.m_floutmassive_right[a][9] = SetupFigures;

            sdk_game.m_flbottomline[19][b] = SetupFigures;

    

            if (sdk_game.m_flfallenfigures[a][b] == FigureOption()) {
                RoundRect(hdc, 125 + b * 27, 125 + a * 27, 125 + b * 27 + 25, 125 + a * 27 + 25, 5, 5);
            }


            if (sdk_game.m_flmassive[a][b] == FigureOption()) {
                RoundRect(hdc, 125 + b * 27, 125 + a * 27, 125 + b * 27 + 25, 125 + a * 27 + 25, 5, 5);
            }
        }
    }

    for (int a = 0; a < 20; a++) {
        for (int b = 0; b < 10; b++) {
            native_rendered_predict(hdc, hpen, a, b);
        }
    }
}



void c_render::draw_background(HDC hdc, RECT rect) {
    HBRUSH hBrush;
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hBrush);

    FillRect(hdc, &rect, hBrush);

    DeleteObject(hBrush);
}

void c_render::fixed_wmpaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    int dc_width, dc_height;
    HDC mem_dc;
    HBITMAP mem_bitmap;
    RECT rect;

    GetClientRect(hwnd, &rect);

    dc_width = rect.right - rect.left;
    dc_height = rect.bottom - rect.top;

    mem_dc = CreateCompatibleDC(hdc);
    mem_bitmap = CreateCompatibleBitmap(hdc, dc_width, dc_height);
    SelectObject(mem_dc, mem_bitmap);

    draw_background(mem_dc, rect);

    //main_render(hwnd, mem_dc);
    if (sdk_game.lose_game && !sdk_game.start_game) {
        interfaces->main_menu(hwnd, mem_dc);
    }
    else if (sdk_game.start_game) {
        main_render(hwnd, mem_dc);
    }

    BitBlt(hdc, 0, 0, dc_width, dc_height, mem_dc, 0, 0, SRCCOPY);

    DeleteObject(mem_bitmap);
    DeleteObject(mem_dc);

    EndPaint(hwnd, &ps);
}