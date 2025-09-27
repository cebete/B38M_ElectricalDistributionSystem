#include "ElectricalSystem.h"
#include <iostream>

ElectricalSystem::ElectricalSystem()
	: extPwr(SourceType::External),
	apuGen(SourceType::APUGen),
	eng1Gen(SourceType::Eng1Gen),
	eng2Gen(SourceType::Eng2Gen),
	battery(SourceType::Battery),
	ac1(BusName::AC1),
	ac2(BusName::AC2),
	dc1(BusName::DC1),
	dc2(BusName::DC2),
	standby(BusName::Standby),
	btb1("BTB1"),
	btb2("BTB2")
{
	battery.initBattery(100.0, 1.0, 2.0);
	battery.setAvailable(true);

	// By default: engines/APU physically exist but not running
	apuGen.setAvailable(true);
	eng1Gen.setAvailable(true);
	eng2Gen.setAvailable(true);

	// external power may or may not be available
	extPwr.setAvailable(true);
}

void ElectricalSystem::setExtPower(bool available, bool online)
{
	extPwr.setAvailable(available);
	extPwr.setOnline(online);
}

void ElectricalSystem::setAPUGen(bool available, bool online)
{
	apuGen.setAvailable(available);
	apuGen.setOnline(online);
}

void ElectricalSystem::setEng1Gen(bool available, bool online)
{
	eng1Gen.setAvailable(available);
	eng1Gen.setOnline(online);
}

void ElectricalSystem::setEng2Gen(bool available, bool online)
{
	eng2Gen.setAvailable(available);
	eng2Gen.setOnline(online);
}

void ElectricalSystem::setBattery(bool available, bool online)
{
	battery.setAvailable(available);
	battery.setOnline(online);
}

void ElectricalSystem::updateBattery(double deltaSeconds)
{
    bool standbyOnBattery = (standby.isPowered() && standby.getPoweredBy() == "BATTERY");
    bool recharge = (ac1.isPowered() || ac2.isPowered() || extPwr.isOnline() || apuGen.isOnline());

    // Always tick the battery if either discharging OR recharging
    battery.tickBattery(standbyOnBattery, recharge, deltaSeconds);

    if (battery.getCharge() <= 0.0 && standbyOnBattery) {
        standby.setPowered(false, "");
        emit("BATTERY DISCHARGED - STANDBY LOST");
    }
}

void ElectricalSystem::tickSources(double deltaSeconds)
{
	apuGen.tickStartup(deltaSeconds);
	eng1Gen.tickStartup(deltaSeconds);
	eng2Gen.tickStartup(deltaSeconds);
}

void ElectricalSystem::recalculate()
{
    // --- Reset all buses first to avoid stale state ---
    ac1.setPowered(false, "");
    ac2.setPowered(false, "");
    dc1.setPowered(false, "");
    dc2.setPowered(false, "");
    standby.setPowered(false, "");

    // --- Global availability flags ---
    bool idg1On = eng1Gen.isOnline();
    bool idg2On = eng2Gen.isOnline();
    bool apuOn = apuGen.isOnline();
    bool extOn = extPwr.isOnline();

    bool anyIDG = idg1On || idg2On;

    // EXT/APU rule:
    // - IDG has priority
    // - If no IDG → APU can power both
    // - If no IDG and no APU → EXT can power both
    bool allowAPU = apuOn && !anyIDG;
    bool allowExt = extOn && !anyIDG && !apuOn;

    // --- AC Bus 1 ---
    if (idg1On) {
        ac1.setPowered(true, eng1Gen.name());
    }
    else if (allowAPU) {
        ac1.setPowered(true, apuGen.name());
    }
    else if (allowExt) {
        ac1.setPowered(true, extPwr.name());
    }
    else if (btb1.isClosed() && ac2.isPowered()) {
        ac1.setPowered(true, ac2.getPoweredBy());
    }

    // --- AC Bus 2 ---
    if (idg2On) {
        ac2.setPowered(true, eng2Gen.name());
    }
    else if (allowAPU) {
        ac2.setPowered(true, apuGen.name());
    }
    else if (allowExt) {
        ac2.setPowered(true, extPwr.name());
    }
    else if (btb2.isClosed() && ac1.isPowered()) {
        ac2.setPowered(true, ac1.getPoweredBy());
    }

    // --- DC Buses (fed from AC via TRUs) ---
    dc1.setPowered(ac1.isPowered(), ac1.getPoweredBy());
    dc2.setPowered(ac2.isPowered(), ac2.getPoweredBy());

    // --- Standby bus ---
    if (ac1.isPowered()) {
        standby.setPowered(true, ac1.getPoweredBy());
    }
    else if (battery.isOnline()) {
        standby.setPowered(true, battery.name());
    }
    else {
        standby.setPowered(false, "");
    }
}

void ElectricalSystem::printStatus() const
{
	std::cout << ac1.getName() << " -> " << (ac1.isPowered() ? "ON" : "OFF")
		<< " (by " << ac1.getPoweredBy() << ")\n";
	std::cout << ac2.getName() << " -> " << (ac2.isPowered() ? "ON" : "OFF")
		<< " (by " << ac2.getPoweredBy() << ")\n";
	std::cout << dc1.getName() << " -> " << (dc1.isPowered() ? "ON" : "OFF")
		<< " (by " << dc1.getPoweredBy() << ")\n";
	std::cout << dc2.getName() << " -> " << (dc2.isPowered() ? "ON" : "OFF")
		<< " (by " << dc2.getPoweredBy() << ")\n";
	std::cout << standby.getName() << " -> " << (standby.isPowered() ? "ON" : "OFF")
		<< " (by " << standby.getPoweredBy() << ")\n";
}