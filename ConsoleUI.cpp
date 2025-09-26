#include "ConsoleUI.h"
#include <chrono>
#include <conio.h>
#include <iostream>
#include <thread>

// ANSI colors
namespace term
{
	constexpr const char* reset = "\x1b[0m";
	constexpr const char* green = "\x1b[32m";
	constexpr const char* red   = "\x1b[31m";
	constexpr const char* yellow = "\x1b[33m";
}

ConsoleUI::ConsoleUI(ElectricalSystem& system) : elec(system) 
{
	elec.setEventSink([this](const std::string& m) { this -> pushLog(m); });
}

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

std::string ConsoleUI::batteryColor(double charge) const
{
	if (charge > 50) return std::string(term::green);
	if (charge > 20) return std::string(term::yellow);
	return std::string(term::red);
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
	double charge = elec.getBatteryCharge();
	if (charge < 0) charge = 0;
	if (charge > 100) charge = 100;

	std::cout << " │ BATTERY   : "
		<< onOff(elec.getBatteryOnline())
		<< batteryColor(charge) << "(" << (int)charge << "%)" << term::reset << "\n";

	std::cout << " ├──────────────────────────────┤\n";

	// Bus states
	std::cout << " │ AC BUS 1:   " << coloredStatus(elec.getAC1().isPowered(), elec.getAC1().getPoweredBy()) << "\n";
	std::cout << " │ AC BUS 2:   " << coloredStatus(elec.getAC2().isPowered(), elec.getAC2().getPoweredBy()) << "\n";
	std::cout << " │ DC BUS 1:   " << coloredStatus(elec.getDC1().isPowered(), elec.getDC1().getPoweredBy()) << "\n";
	std::cout << " │ DC BUS 2:   " << coloredStatus(elec.getDC2().isPowered(), elec.getDC2().getPoweredBy()) << "\n";
	std::cout << " │ STANDBY:    " << coloredStatus(elec.getStandby().isPowered(), elec.getStandby().getPoweredBy()) << "\n";


	std::cout <<
		" ├──────────────────────────────┤\n"
		" │ STATUS LOG                   │\n";

	for (const auto& line : log) {
		std::string clipped = line.substr(0, 28); // keep inside panel width
		std::cout << " │ " << clipped;
		// pad to width
		if (clipped.size() < 28) std::cout << std::string(28 - clipped.size(), ' ');
		std::cout << " │\n";
	}

	std::cout << " └──────────────────────────────┘\n\n";
}

#include <conio.h>  // Windows only (_kbhit, _getch)

void ConsoleUI::showMenu()
{
	bool running = true;

	while (running)
	{
		// Update electrical logic + battery every second
		elec.recalculate();
		elec.updateBattery(1.0);


		drawPanel();

		std::cout <<
			" 1. Toggle External Power\n"
			" 2. Toggle APU Gen\n"
			" 3. Toggle Engine 1 Gen\n"
			" 4. Toggle Engine 2 Gen\n"
			" 5. Toggle Battery\n"
			" 0. Exit\n"
			"Press a number to toggle, or wait...\n";

		// Sleep one second to simulate real time
		std::this_thread::sleep_for(std::chrono::seconds(1));

		// Non-blocking input handling
		if (_kbhit()) {
			int key = _getch();
			switch (key) {
				case '1': elec.toggleExtPower();  pushLog(std::string("EXT PWR -> ") + (elec.getExtPowerOnline() ? "ON" : "OFF")); break;
				case '2': elec.toggleAPUGen();    pushLog(std::string("APU GEN -> ") + (elec.getAPUGenOnline() ? "ON" : "OFF"));   break;
				case '3': elec.toggleEng1Gen();   pushLog(std::string("ENG1 GEN -> ") + (elec.getEng1GenOnline() ? "ON" : "OFF"));  break;
				case '4': elec.toggleEng2Gen();   pushLog(std::string("ENG2 GEN -> ") + (elec.getEng2GenOnline() ? "ON" : "OFF"));  break;
				case '5': elec.toggleBattery();   pushLog(std::string("BATTERY -> ") + (elec.getBatteryOnline() ? "ON" : "OFF"));  break;
				case '0': running = false; break;
			}
		}
	}
}

