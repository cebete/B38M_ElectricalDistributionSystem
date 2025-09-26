#pragma once
#include <vector>
#include "PowerSource.h"
#include "Bus.h"

class ElectricalSystem
{
private:
	// Power sources
	PowerSource extPwr;
	PowerSource apuGen;
	PowerSource eng1Gen;
	PowerSource eng2Gen;
	PowerSource battery;

	// Buses
	Bus ac1;
	Bus ac2;
	Bus dc1;
	Bus dc2;
	Bus standby;
	
public:
	ElectricalSystem();

	// Toggle methods
	void setExtPower(bool available, bool online);
	void setAPUGen(bool available, bool online);
	void setEng1Gen(bool available, bool online);
	void setEng2Gen(bool available, bool online);
	void setBattery(bool available, bool online);

	// Update bus states
	void recalculate();

	// For testing
	void printStatus() const;

	// Quick getters for ConsoleUI
	bool getExtPowerOnline() const { return extPwr.isOnline(); }
	bool getAPUGenOnline() const { return apuGen.isOnline(); }
	bool getEng1GenOnline() const { return eng1Gen.isOnline(); }
	bool getEng2GenOnline() const { return eng2Gen.isOnline(); }
	bool getBatteryOnline() const { return battery.isOnline(); }

	// Direct accessors for buses
	const Bus& getAC1() const { return ac1; }
	const Bus& getAC2() const { return ac2; }
	const Bus& getDC1() const { return dc1; }
	const Bus& getDC2() const { return dc2; }
	const Bus& getStandby() const { return standby; }

	// Toggle functions (for interactive menu)
	void toggleExtPower() { extPwr.setOnline(!extPwr.isOnline()); }
	void toggleAPUGen() { apuGen.setOnline(!apuGen.isOnline()); }
	void toggleEng1Gen() { eng1Gen.setOnline(!eng1Gen.isOnline()); }
	void toggleEng2Gen() { eng2Gen.setOnline(!eng2Gen.isOnline()); }
	void toggleBattery() { battery.setOnline(!battery.isOnline()); }
};
