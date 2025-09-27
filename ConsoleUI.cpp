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
    std::cout << " │ BTB1:       " << (elec.getBTB1Closed() ? "[CLOSED]" : "[OPEN]") << "\n";
    std::cout << " │ BTB2:       " << (elec.getBTB2Closed() ? "[CLOSED]" : "[OPEN]") << "\n";


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
        // Progress sources + battery each second
        elec.tickSources(1.0);     // ⬅️ NEW: advance startup timers
        elec.recalculate();
        elec.updateBattery(1.0);

        // Draw updated panel
        drawPanel();

        std::cout <<
            " 1. Toggle External Power\n"
            " 2. Start/Stop APU Gen\n"
            " 3. Start/Stop Engine 1 Gen\n"
            " 4. Start/Stop Engine 2 Gen\n"
            " 5. Toggle Battery\n"
            " 6. Toggle BTB1\n"
            " 7. Toggle BTB2\n"
            " 0. Exit\n"
            "Press a number to toggle, or wait...\n";

        // Sleep one second to simulate real time
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Non-blocking input handling
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case '1':
                    elec.toggleExtPower();
                    pushLog(std::string("EXT PWR -> ") + (elec.getExtPowerOnline() ? "ON" : "OFF"));
                    break;

                case '2':
                    if (!elec.getAPUGenOnline() && !elec.isAPUStarting()) {
                        elec.startAPU();    // wrapper → apuGen.beginStartup(5s)
                        pushLog("APU starting...");
                    }
                    else if (elec.getAPUGenOnline()) {
                        elec.toggleAPUGen();
                        pushLog("APU GEN OFF");
                    }
                    break;

                case '3':
                    if (!elec.getEng1GenOnline() && !elec.isEng1Starting()) {
                        elec.startEng1();   // wrapper → eng1Gen.beginStartup(7s)
                        pushLog("ENG1 spooling up...");
                    }
                    else if (elec.getEng1GenOnline()) {
                        elec.toggleEng1Gen();
                        pushLog("ENG1 GEN OFF");
                    }
                    break;

                case '4':
                    if (!elec.getEng2GenOnline() && !elec.isEng2Starting()) {
                        elec.startEng2();   // wrapper → eng2Gen.beginStartup(7s)
                        pushLog("ENG2 spooling up...");
                    }
                    else if (elec.getEng2GenOnline()) {
                        elec.toggleEng2Gen();
                        pushLog("ENG2 GEN OFF");
                    }
                    break;

                case '5':
                    elec.toggleBattery();
                    pushLog(std::string("BATTERY -> ") + (elec.getBatteryOnline() ? "ON" : "OFF"));
                    break;

                case '6':
                    elec.toggleBTB1();
                    pushLog(std::string("BTB1 -> ") + (elec.getBTB1Closed() ? "CLOSED" : "OPEN"));
                    break;

                case '7':
                    elec.toggleBTB2();
                    pushLog(std::string("BTB2 -> ") + (elec.getBTB2Closed() ? "CLOSED" : "OPEN"));
                    break;

                case '0':
                    running = false;
                    break;
            }
        }
    }
}

