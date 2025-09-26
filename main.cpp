#include "ElectricalSystem.h"
#include "ConsoleUI.h"

int main() 
{
    ElectricalSystem elec;
    ConsoleUI ui(elec);

    elec.recalculate();
    ui.showMenu();

    return 0;
}
