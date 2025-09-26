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
	  standby(BusName::Standby) 
{
	battery.initBattery(100.0, 1.0, 2.0);
	battery.setAvailable(true);   // always physically present
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

    // 🔑 Always tick the battery if either discharging OR recharging
    battery.tickBattery(standbyOnBattery, recharge, deltaSeconds);

    if (battery.getCharge() <= 0.0 && standbyOnBattery) {
        standby.setPowered(false, "");
        emit("BATTERY DISCHARGED - STANDBY LOST");
    }
}

void ElectricalSystem::recalculate()
{
	// Simplified priority: EXT > APU > Engine1/Engine2 > Battery

	// --- snapshot previous states
	const bool prev_ac1 = ac1.isPowered(); const std::string prev_ac1_by = ac1.getPoweredBy();
	const bool prev_ac2 = ac2.isPowered(); const std::string prev_ac2_by = ac2.getPoweredBy();
	const bool prev_dc1 = dc1.isPowered(); const std::string prev_dc1_by = dc1.getPoweredBy();
	const bool prev_dc2 = dc2.isPowered(); const std::string prev_dc2_by = dc2.getPoweredBy();
	const bool prev_stb = standby.isPowered(); const std::string prev_stb_by = standby.getPoweredBy();

	// AC Bus 1
	if (extPwr.isOnline()) ac1.setPowered(true, extPwr.name());
	else if (apuGen.isOnline()) ac1.setPowered(true, apuGen.name());
	else if (eng1Gen.isOnline()) ac1.setPowered(true, eng1Gen.name());
	else ac1.setPowered(false, "");

	// AC Bus 2
	if (extPwr.isOnline()) ac2.setPowered(true, extPwr.name());
	else if (apuGen.isOnline()) ac2.setPowered(true, apuGen.name());
	else if (eng2Gen.isOnline()) ac2.setPowered(true, eng2Gen.name());
	else ac2.setPowered(false, "");

	// DC Buses via TRUs
	dc1.setPowered(ac1.isPowered(), ac1.getPoweredBy());
	dc2.setPowered(ac2.isPowered(), ac2.getPoweredBy());

	// Standby bus: if AC1 powered -> follow AC1, else if battery -> battery, else off
	if (ac1.isPowered()) standby.setPowered(true, ac1.getPoweredBy());
	else if (battery.isOnline()) standby.setPowered(true, battery.name());
	else standby.setPowered(false, "");
}

void ElectricalSystem::printStatus() const
{
	std::cout << ac1.getName() << " -> " << (ac1.isPowered() ? "ON" : "OFF")
		<< " (by " << ac1.getPoweredBy() << ")\n";
	std::cout << ac2.getName() << " -> " << (ac2.isPowered() ? "ON" : "OFF")
		<< " (by " << ac2.getPoweredBy() << ")\n";
	std::cout << dc1.getName() << " -> " << (dc1.isPowered() ? "ON" : "OFF")
		<< " (by " << dc1.getPoweredBy() << ")\n";
	std::cout << dc2.getName() << " -> " << (ac2.isPowered() ? "ON" : "OFF")
		<< " (by " << dc2.getPoweredBy() << ")\n";
	std::cout << standby.getName() << " -> " << (standby.isPowered() ? "ON" : "OFF")
		<< " (by " << standby.getPoweredBy() << ")\n";
}