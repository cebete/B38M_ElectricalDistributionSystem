#pragma once
#include "Bus.h"
#include "BusTieBreaker.h"
#include "PowerSource.h"
#include <functional>
#include <vector>

class ElectricalSystem
{
private:
    // --- Internal helpers ---
    void emit(const std::string& msg) const { if (sink) sink(msg); }
    std::function<void(const std::string&)> sink; // event sink for UI/logging

    // --- Power Sources ---
    PowerSource extPwr;
    PowerSource apuGen;
    PowerSource eng1Gen;
    PowerSource eng2Gen;
    PowerSource battery;

    // --- Buses ---
    Bus ac1;
    Bus ac2;
    Bus dc1;
    Bus dc2;
    Bus standby;

    // --- Bus Tie Breakers ---
    BusTieBreaker btb1; // AC1 <-> AC2
    BusTieBreaker btb2;

public:
    // --- Constructor ---
    ElectricalSystem();

    // --- Event sink ---
    void setEventSink(std::function<void(const std::string&)> s) { sink = std::move(s); }

    // --- Source configuration ---
    void setExtPower(bool available, bool online);
    void setAPUGen(bool available, bool online);
    void setEng1Gen(bool available, bool online);
    void setEng2Gen(bool available, bool online);
    void setBattery(bool available, bool online);

    // --- Source toggles (instant ON/OFF) ---
    void toggleExtPower() { extPwr.setOnline(!extPwr.isOnline()); }
    void toggleAPUGen() { apuGen.setOnline(!apuGen.isOnline()); }
    void toggleEng1Gen() { eng1Gen.setOnline(!eng1Gen.isOnline()); }
    void toggleEng2Gen() { eng2Gen.setOnline(!eng2Gen.isOnline()); }
    void toggleBattery() { battery.setOnline(!battery.isOnline()); }

    // --- Startup procedures (with delays) ---
    void startAPU() { apuGen.beginStartup(5.0); }
    void startEng1() { eng1Gen.beginStartup(7.0); }
    void startEng2() { eng2Gen.beginStartup(7.0); }

    // --- Startup states ---
    bool isAPUStarting()  const { return apuGen.isStarting(); }
    bool isEng1Starting() const { return eng1Gen.isStarting(); }
    bool isEng2Starting() const { return eng2Gen.isStarting(); }

    // --- Bus Tie Breakers ---
    void toggleBTB1() { btb1.setClosed(!btb1.isClosed()); }
    void toggleBTB2() { btb2.setClosed(!btb2.isClosed()); }
    bool getBTB1Closed() const { return btb1.isClosed(); }
    bool getBTB2Closed() const { return btb2.isClosed(); }

    // --- System updates ---
    void recalculate();                  // recalc bus states
    void tickSources(double deltaSeconds); // advance startup timers
    void updateBattery(double deltaSeconds);

    // --- Monitoring ---
    double getBatteryCharge() const { return battery.getCharge(); }
    void printStatus() const;

    // --- Quick getters (power source states) ---
    bool getExtPowerOnline() const { return extPwr.isOnline(); }
    bool getAPUGenOnline()   const { return apuGen.isOnline(); }
    bool getEng1GenOnline()  const { return eng1Gen.isOnline(); }
    bool getEng2GenOnline()  const { return eng2Gen.isOnline(); }
    bool getBatteryOnline()  const { return battery.isOnline(); }

    // --- Bus accessors ---
    const Bus& getAC1() const { return ac1; }
    const Bus& getAC2() const { return ac2; }
    const Bus& getDC1() const { return dc1; }
    const Bus& getDC2() const { return dc2; }
    const Bus& getStandby() const { return standby; }
};
