#pragma once
#include <string>
#include "PowerSource.h"

enum class BusName { AC1, AC2, DC1, DC2, Standby };

class Bus
{
private:
	BusName name;
	bool powered;
	std::string poweredBy; // name of source (e.g., "EXT PWR", "ENG1 GEN")
public:
	// Constructor
	Bus(BusName n);

	// Setters
	void setPowered(bool p, const std::string& sourceName);

	// Getters
	bool isPowered() const;
	std::string getPoweredBy() const;
	std::string getName() const;
};