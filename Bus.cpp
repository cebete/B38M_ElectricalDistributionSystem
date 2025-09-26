#include "Bus.h"

Bus::Bus(BusName n) : name(n), powered(false), poweredBy("None") { }

void Bus::setPowered(bool p, const std::string& sourceName)
{
	powered = p;
	poweredBy = p ? sourceName : "None";
}

bool Bus::isPowered() const
{
	return powered;
}

std::string Bus::getPoweredBy() const
{
	return poweredBy;
}

std::string Bus::getName() const
{
	switch (name)
	{
		case BusName::AC1: return "AC BUS 1";
		case BusName::AC2: return "AC BUS 2";
		case BusName::DC1: return "DC BUS 1";
		case BusName::DC2: return "DC BUS 2";
		case BusName::Standby: return "STANDBY";
	}
	return "UNKNOWN";
}