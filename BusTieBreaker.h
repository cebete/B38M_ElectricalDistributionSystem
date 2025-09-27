#pragma once
#include <string>

class BusTieBreaker
{
private:
	bool closed;          // breaker closed = buses connected
	std::string name;     // e.g. "BTB1" or "BTB2

public:
	BusTieBreaker(const std::string& n);

	void setClosed(bool c);
	bool isClosed() const;

	std::string getName() const;
};
