#define __LCC__ 

#include "Web_Connect.h"
#include <string>
#include "sdk.h"
#include <sstream>
#include <iostream>

/*

void web_connect_t::init() {
	MYSQL* obj = nullptr;

	obj = mysql_init(0);

	if (sdk_game.server.write) {
		write_results(obj);
	}
	else if (sdk_game.server.check) {
		check_results(obj);
	}
}

void web_connect_t::write_results(MYSQL* obj) {

	MYSQL_ROW row;
	std::stringstream ss;
	ss << "SELECT * FROM players";


	std::stringstream ss_insert;
	ss_insert << "REPLACE INTO players(player, score) VALUES('" << sdk_game.server.write_player << "', '" << sdk_game.server.write_score << "')";



	if (mysql_real_connect(obj, sdk_game.server.host, sdk_game.server.user, sdk_game.server.pass, "sql11647000", 3306, NULL, 0)) {

	//	mysql_query(obj, ss.str().c_str());

		
		mysql_query(obj, ss_insert.str().c_str());
	

	//	mysql_store_result(obj);

		mysql_close(obj);
	}

	sdk_game.server.write = false;
}

void web_connect_t::check_results(MYSQL* obj) {

	MYSQL_ROW row;	
	std::string ff = "";
	std::stringstream ss, ss1;
	ss << "SELECT * FROM players";

	if (mysql_real_connect(obj, sdk_game.server.host, sdk_game.server.user, sdk_game.server.pass, "sql11647000", 3306, NULL, 0)) {

		for (int i = 1; i < 24; i++) {

			ss1 << "SELECT * FROM players WHERE id = " << i;

			mysql_query(obj, ss1.str().c_str());

			MYSQL_RES my_sql_res = *mysql_store_result(obj);

			while ((row = mysql_fetch_row(&my_sql_res))) {
				sdk_game.server.info_player[i].player = row[1];
				sdk_game.server.info_player[i].score = row[2];
			}
	
			ss1.str(std::string());
		}

		mysql_close(obj);
	
	}

	sdk_game.server.check = false;
}*/