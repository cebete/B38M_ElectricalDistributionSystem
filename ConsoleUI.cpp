#include "ConsoleUI.h"
#include <iostream>

// ANSI colors
namespace term
{
	constexpr const char* reset = "\x1b[0m";
	constexpr const char* green = "\x1b[32m";
	constexpr const char* red   = "\x1b[31m";
	constexpr const char* yellow = "\x1b[33m";
}

ConsoleUI::ConsoleUI(ElectricalSystem& system) : elec(system) { }

void ConsoleUI::clearScreen() const
{
#if defined(_WIN32)
	system("cls");
#else
	std::cout << "\x1b[2J\x1b[H";
#endif
}

std::string ConsoleUI::onOff(bool on) const
{
	return on ? "[ON] " : "[OFF]";
}

std::string ConsoleUI::coloredStatus(bool powered, const std::string& source) const
{
	if (!powered) return std::string(term::red) + "RED (NO PWR)" + term::reset;
	if (source == "BATTERY") return std::string(term::yellow) + "YELLOW (" + source + ")" + term::reset;
	return std::string(term::green) + "GREEN (" + source + ")" + term::reset;
}

void ConsoleUI::drawPanel() const
{
	clearScreen();

	std::cout <<
	" ┌──────────────────────────────┐\n"
	" │   737 MAX ELECTRICAL PANEL   │\n"
	" ├──────────────────────────────┤\n"
	" │ [Toggle with Menu Options]   │\n"
	" ├──────────────────────────────┤\n";

	// Switch states (EXT/APU/ENG1/ENG2/BAT)
	std::cout << " │ EXT PWR   : " << onOff(elec.getExtPowerOnline()) << "\n";
	std::cout << " │ APU GEN   : " << onOff(elec.getAPUGenOnline()) << "\n";
	std::cout << " │ ENG1 GEN  : " << onOff(elec.getEng1GenOnline()) << "\n";
	std::cout << " │ ENG2 GEN  : " << onOff(elec.getEng2GenOnline()) << "\n";
	std::cout << " │ BATTERY   : " << onOff(elec.getBatteryOnline()) << "\n";

	std::cout << " ├──────────────────────────────┤\n";

	// Bus states
	std::cout << " │ AC BUS 1:   " << coloredStatus(elec.getAC1().isPowered(), elec.getAC1().getPoweredBy()) << "\n";
	std::cout << " │ AC BUS 2:   " << coloredStatus(elec.getAC2().isPowered(), elec.getAC2().getPoweredBy()) << "\n";
	std::cout << " │ DC BUS 1:   " << coloredStatus(elec.getDC1().isPowered(), elec.getDC1().getPoweredBy()) << "\n";
	std::cout << " │ DC BUS 2:   " << coloredStatus(elec.getDC2().isPowered(), elec.getDC2().getPoweredBy()) << "\n";
	std::cout << " │ STANDBY:    " << coloredStatus(elec.getStandby().isPowered(), elec.getStandby().getPoweredBy()) << "\n";

	std::cout <<
	" ├──────────────────────────────┤\n"
	" │ STATUS: Use menu to interact │\n"
	" └──────────────────────────────┘\n\n";
}

void ConsoleUI::showMenu()
{
	int choice;
	do
	{
		drawPanel();
		std::cout <<
		" 1. Toggle External Power\n"
		" 2. Toggle APU Gen\n"
		" 3. Toggle Engine 1 Gen\n"
		" 4. Toggle Engine 2 Gen\n"
		" 5. Toggle Battery\n"
		" 0. Exit\n"
		"Choice: ";
		std::cin >> choice;

		switch (choice) 
		{
			case 1: elec.toggleExtPower(); break;
			case 2: elec.toggleAPUGen(); break;
			case 3: elec.toggleEng1Gen(); break;
			case 4: elec.toggleEng2Gen(); break;
			case 5: elec.toggleBattery(); break;
		}

		elec.recalculate();
	}
	while (choice != 0);
}

