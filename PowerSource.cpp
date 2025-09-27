#include "PowerSource.h"

PowerSource::PowerSource(SourceType t)
	: type(t),
	starting(false),
	startupTime(0.0),
	elapsedStartup(0.0),
	available(false),
	online(false),
	chargePercent(0.0),
	dischargeRate(0.0),
	rechargeRate(0.0)
{
}

void PowerSource::setAvailable(bool a) { available = a; }
void PowerSource::setOnline(bool o) {
	if (o) {
		// only allow ON if available and not starting
		if (available && !starting)
			online = true;
	}
	else {
		online = false;
	}
}

bool PowerSource::isAvailable() const { return available; }
bool PowerSource::isOnline() const { return online; }

std::string PowerSource::name() const
{
	switch (type)
	{
		case SourceType::External: return "EXT PWR";
		case SourceType::APUGen: return "APU GEN";
		case SourceType::Eng1Gen: return "ENG1 GEN";
		case SourceType::Eng2Gen: return "ENG2 GEN";
		case SourceType::Battery: return "BATTERY";
	}
	return "UNKNOWN";
}

void PowerSource::initBattery(double startPercent, double drain, double recharge)
{
	if (type == SourceType::Battery)
	{
		chargePercent = startPercent;
		dischargeRate = drain;
		rechargeRate = recharge;
	}
}

void PowerSource::tickBattery(bool discharging, bool recharging, double deltaSeconds)
{
	if (type != SourceType::Battery) return;
	
	if (discharging && chargePercent > 0.0)
	{
		chargePercent -= dischargeRate * deltaSeconds;
		if (chargePercent < 0.0) chargePercent = 0.0;
	}

	if (recharging && chargePercent < 100.0)
	{
		chargePercent += rechargeRate * deltaSeconds;
		if (chargePercent > 100.0) chargePercent = 100.0;
	}
}

void PowerSource::beginStartup(double duration)
{
	if (!online && !starting)
	{
		starting = true;
		startupTime = duration;
		elapsedStartup = 0.0;
	}
}

void PowerSource::tickStartup(double deltaSeconds)
{
	if (starting)
	{
		elapsedStartup += deltaSeconds;
		if (elapsedStartup >= startupTime)
		{
			starting = false;
			online = true;
		}
	}
}