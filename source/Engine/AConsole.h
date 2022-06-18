#pragma once
#include "common.h"

class AConsole {
public:
	std::vector<std::string> m_vLines;

public:
	AConsole();
	bool Init();
	void WriteLine(std::string str);
	void Shutdown();
	void Flush();
};

extern AConsole Console;
