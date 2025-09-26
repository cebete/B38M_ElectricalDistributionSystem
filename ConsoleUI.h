#pragma once
#include <deque>
#include <string>
#include "ElectricalSystem.h"

class ConsoleUI {
private:
	ElectricalSystem& elec;
	std::deque<std::string> log; // last few events

	void clearScreen() const;
	std::string onOff(bool on) const;
	std::string coloredStatus(bool powered, const std::string& source) const;
	std::string batteryColor(double charge) const;

	void pushLog(const std::string& msg)
	{
		if (log.size() >= 5) log.pop_front();
		log.push_back(msg);
	}

public:
	ConsoleUI(ElectricalSystem& system);

	void drawPanel() const;
	void showMenu();
};