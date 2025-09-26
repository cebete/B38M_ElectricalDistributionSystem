#include "PowerSource.h";

PowerSource::PowerSource(SourceType t)
	: type(t), available(false), online(false) { }

void PowerSource::setAvailable(bool a) { available = a; }
void PowerSource::setOnline(bool o) { online = o; }

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