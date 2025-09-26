#pragma once
#include "ElectricalSystem.h"

class ConsoleUI {
private:
	ElectricalSystem& elec;

	void clearScreen() const;
	std::string onOff(bool on) const;
	std::string coloredStatus(bool powered, const std::string& source) const;

public:
	ConsoleUI(ElectricalSystem& system);

	void drawPanel() const;
	void showMenu();
};