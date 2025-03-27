// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/2d48960e2d12e8a5b03ccb92de8849f067b85046/src/main/java/com/hbm/items/ModItems.java#L3310

#include <map>
#include <string>

#include "rbmkFuelRod.h"

std::map<std::string, RBMKFuelRod*> fuelRegistry;

void RegisterFuel(std::string name, RBMKFuelRod* fuel) {
    fuelRegistry.emplace(name, fuel);
}

void RegisterFuels() {
    // i find this kind of syntax mildly interesting

    RBMKFuelRod* rbmk_fuel_ueu = new RBMKFuelRod("Unenriched Uranium");
    rbmk_fuel_ueu
        ->setYield(100000000)
        ->setStats(15)
        ->setFunction(BURNFUNC_LOG_TEN)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(0.65)
        ->setMeltingPoint(2865);

    RegisterFuel("rbmk_fuel_ueu", rbmk_fuel_ueu);

    RBMKFuelRod *rbmk_fuel_ra226be = new RBMKFuelRod("Radium-226 & Beryllium Neutron Source");
    rbmk_fuel_ra226be
        ->setYield(100000000)
        ->setStats(0, 20)
        ->setFunction(BURNFUNC_PASSIVE)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setHeat(0.035)
        ->setDiffusion(0.5)
        ->setMeltingPoint(700)
        ->setNeutronTypes(NTYPE_SLOW, NTYPE_SLOW);

    RegisterFuel("rbmk_fuel_ra226be", rbmk_fuel_ra226be);
}

RBMKFuelRod* PrepareFuel(std::string name) {
    RBMKFuelRod* fuel = new RBMKFuelRod(fuelRegistry[name]);
    fuel->active = true;
    return fuel;
}