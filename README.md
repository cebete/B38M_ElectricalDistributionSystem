# 🛫 B38M Electrical Distribution System Simulator

A C++ console-based simulation of the **Boeing 737 MAX (B38M) electrical distribution system**.  
The goal is to replicate the aircraft’s electrical logic (power sources, bus priorities, standby systems, failures) in a study-level but lightweight environment.

---

## ✨ Current Features
- **Power Sources**
  - External Power (EXT PWR)
  - APU Generator (APU GEN) with startup delay
  - Engine Generators (ENG1 GEN, ENG2 GEN) with startup delay
  - Battery with charging/discharging logic

- **Bus System**
  - AC Bus 1 & AC Bus 2
  - DC Bus 1 & DC Bus 2 (fed via simplified TRUs from AC buses)
  - Standby Bus (powered from AC1 or battery if AC lost)

- **Battery Simulation**
  - Customizable start %, discharge rate, recharge rate
  - Recharges automatically when AC power is available
  - Discharges when feeding standby bus
  - Standby loss message when battery depleted

- **Startup Logic**
  - APU and Engine generators spool up over time
  - Online only after startup completes

- **Console UI**
  - ASCII-style electrical panel
  - Color-coded bus status (green = powered, yellow = battery, red = no power)
  - Event log (latest 5 actions)
  - Menu options to toggle/start sources interactively

---

## 🎯 Purpose of Development
- Build a **realistic yet lightweight model** of the 737 MAX electrical system.  
- Help understand bus priority, standby systems, and failure effects.  
- Provide a base for **future features** like bus ties, TRU failures, load shedding, and full failure simulation.  
- Educational & training-oriented project.

---

## 🛠️ Roadmap

### Tier 1 – Core Distribution (foundation) (CURRENTLY HERE)
1. Add **Bus Tie Breakers (BTBs)** → AC1 & AC2 cross-feed  
2. Fix **source priority** → IDG > APU > EXT > Opposite IDG (via BTB)  
3. Make **TRUs** (TRU1 & TRU2) real objects  

### Tier 2 – Standby & Battery
4. Add **Hot Battery Bus** (always powered)  
5. Add **DC Standby Bus** (normally DC1, fallback = Hot Batt Bus)  
6. Add **Standby Inverter** (AC Standby from DC Standby if AC lost)  

### Tier 3 – Load Management
7. Implement **Load Shedding** (drop non-essential buses on single gen)  
8. Add **Generator Load % tracking** (overload → failure)  

### Tier 4 – Advanced
9. **Automatic Transfers** (APU off when ENG gens online, EXT auto-disconnect)  
10. **EICAS Alerts / Annunciators** (ELEC GEN OFF BUS, ELEC DC FAIL, etc.)  
11. **Environmental Effects** (cold weakens battery, TRU cooling overheat)  

### Tier 5 – Failures (final stage)
12. Simulate **Source Failures** (APU fail, IDG disconnect, TRU overheat)  
13. Add **Bus Faults** (shorts / breaker trips)  
14. **Battery Degradation** (capacity loss, over-discharge)  

---

## 📌 Notes
- Developed for **learning and training**, not for operational flight use.  
- Contributions and improvements are welcome!  
