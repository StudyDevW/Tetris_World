#include "Tetris_Editor.h"
#include "sdk.h"
#include "Tetris_Logic.h"

void editor_t::init() {
	//presets_on_floor("second_preset");
}

void editor_t::presets_on_floor(std::string preset) {
	static int m_flfloor_presets[20][10];
	
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 10; x++) {
			if (preset == "first_preset") {

				m_flfloor_presets[19][0] = 2;
				m_flfloor_presets[19][1] = 2;
				m_flfloor_presets[19][2] = 2;

				m_flfloor_presets[19][9] = 2;
				m_flfloor_presets[19][8] = 2;
				m_flfloor_presets[19][7] = 2;

				m_flfloor_presets[18][0] = 2;
				m_flfloor_presets[18][1] = 2;

				m_flfloor_presets[18][9] = 2;
				m_flfloor_presets[18][8] = 2;

				m_flfloor_presets[17][0] = 2;
				m_flfloor_presets[17][9] = 2;

			
			}
			else if (preset == "second_preset") {
				m_flfloor_presets[19][0] = 2;
				m_flfloor_presets[19][1] = 2;
				m_flfloor_presets[19][2] = 2;
				m_flfloor_presets[19][3] = 2;

				m_flfloor_presets[19][6] = 2;
				m_flfloor_presets[19][7] = 2;
				m_flfloor_presets[19][8] = 2;
				m_flfloor_presets[19][9] = 2;

				m_flfloor_presets[18][0] = 2;
				m_flfloor_presets[18][1] = 2;
				m_flfloor_presets[18][2] = 2;
				m_flfloor_presets[18][3] = 2;

				m_flfloor_presets[18][9] = 2;
				m_flfloor_presets[18][8] = 2;
				m_flfloor_presets[18][7] = 2;
				m_flfloor_presets[18][6] = 2;

			}
			else if (preset == "third_preset") {

			}
			else if (preset == "custom_preset") {

			}



			if (m_flfloor_presets[y][x] == 2 && sdk_game.editor_s.need_to_stack == true) {
				if (sdk_game.m_flfallenfigures[y][x] != m_flfloor_presets[y][x]) {
					logic->copy_to_dest(m_flfloor_presets, sdk_game.m_flfallenfigures, y, x);
					//if (sdk_game.m_flfalltimer > 3) {
					//	sdk_game.editor_s.need_to_stack = false;
				//	}
				}
			}

		}
	}
}