#include "BusTieBreaker.h"

BusTieBreaker::BusTieBreaker(const std::string& n) : closed(true), name(n) {}

void BusTieBreaker::setClosed(bool c)
{
	closed = c;
}

bool BusTieBreaker::isClosed() const
{
	return closed;
}

std::string BusTieBreaker::getName() const
{
	return name;
}