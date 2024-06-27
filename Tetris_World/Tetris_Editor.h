#pragma once
#include <string>

class editor_t {
public:
	void init();
	void presets_on_floor(std::string preset);
};

extern editor_t* editor;