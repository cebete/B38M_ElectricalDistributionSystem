#pragma once
#include <string>

enum class SourceType { External, APUGen, Eng1Gen, Eng2Gen, Battery };

class PowerSource
{
private:
	SourceType type;
	bool available;   // physically connected/running
	bool online;      // actually selected to supply power
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
};