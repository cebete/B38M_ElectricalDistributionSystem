#pragma once
#include <string>

enum class SourceType { External, APUGen, Eng1Gen, Eng2Gen, Battery };

class PowerSource
{
private:
	SourceType type;
	bool available;   // physically connected/running
	bool online;      // actually selected to supply power

	double chargePercent; // 0.0 - 100.0
	double dischargeRate; // % per second when powering standby
	double rechargeRate; // % per second if AC available.
public:
	// Constructor
	PowerSource(SourceType t);

	// Control
	void setAvailable(bool a);
	void setOnline(bool o);

	// Query state
	bool isAvailable() const;
	bool isOnline() const;

	// Helpers
	std::string name() const;

	// Battery specific
	void initBattery(double startPercent = 100.0, double drain = 1.0, double recharge = 2.0);
	void tickBattery(bool discharging, bool recharging, double deltaSeconds);
	double getCharge() const { return chargePercent; }
};