#pragma once
#include "Bus.h"
#include "PowerSource.h"
#include <functional>
#include <vector>



class ElectricalSystem
{
private:
	void emit(const std::string& msg) const { if (sink) sink(msg); }

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

	std::function<void(const std::string&)> sink; // event target (UI)

public:
	void setEventSink(std::function<void(const std:: string&)> s) { sink = std::move(s); }

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

	void updateBattery(double deltaSeconds);
	double getBatteryCharge() const { return battery.getCharge(); }

	void tickSources(double deltaSeconds);

	void startAPU() { apuGen.beginStartup(5.0); }
	void startEng1() { eng1Gen.beginStartup(7.0); }
	void startEng2() { eng2Gen.beginStartup(7.0); }

	bool isAPUStarting() const { return apuGen.isStarting(); }
	bool isEng1Starting() const { return eng1Gen.isStarting(); }
	bool isEng2Starting() const { return eng2Gen.isStarting(); }

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
