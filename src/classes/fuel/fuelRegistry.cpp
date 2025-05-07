// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/ModItems.java#L3310

#include <map>
#include <string>
#include <vector>

#include "../inventory/itemSlot.h"
#include "rbmkFuelRod.h"

std::map<std::string, RBMKFuelRod*> fuelRegistry;

void RegisterFuel(std::string name, RBMKFuelRod* fuel) {
    fuel->reset();
    fuelRegistry.emplace(name, fuel);
    TraceLog(LOG_INFO, TextFormat("RBMK: - %s", name.c_str()));
}

void RegisterFuels() {
    TraceLog(LOG_INFO, "RBMK: Beginning fuel initialization");

    RBMKFuelRod *rbmk_fuel_ueu = new RBMKFuelRod("rbmk_fuel_ueu", "Unenriched Uranium");
    rbmk_fuel_ueu
        ->setYield(100000000)
        ->setStats(15)
        ->setFunction(BURNFUNC_LOG_TEN)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(0.65)
        ->setMeltingPoint(2865)
        ->setItemName("NU RBMK Rod");

    RBMKFuelRod *rbmk_fuel_meu = new RBMKFuelRod("rbmk_fuel_meu", "Medium Enriched Uranium-235");
    rbmk_fuel_meu
        ->setYield(100000000)
        ->setStats(20)
        ->setFunction(BURNFUNC_LOG_TEN)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(0.65)
        ->setMeltingPoint(2865)
        ->setItemName("MEU RBMK Rod");

    RBMKFuelRod *rbmk_fuel_heu233 = new RBMKFuelRod("rbmk_fuel_heu233", "Highly Enriched Uranium-233");
    rbmk_fuel_heu233
        ->setYield(100000000)
        ->setStats(27.5)
        ->setFunction(BURNFUNC_LINEAR)
        ->setHeat(1.25)
        ->setMeltingPoint(2865)
        ->setItemName("HEU-233 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_heu235 = new RBMKFuelRod("rbmk_fuel_heu235", "Highly Enriched Uranium-235");
    rbmk_fuel_heu235
        ->setYield(100000000)
        ->setStats(50)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setMeltingPoint(2865)
        ->setItemName("HEU-235 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_thmeu = new RBMKFuelRod("rbmk_fuel_thmeu", "Thorium with MEU Driver Fuel");
    rbmk_fuel_thmeu
        ->setYield(100000000)
        ->setStats(20)
        ->setFunction(BURNFUNC_PLATEU)
        ->setDepletionFunction(DEPFUNC_BOOSTED_SLOPE)
        ->setHeat(0.65)
        ->setMeltingPoint(3350)
        ->setItemName("ThMEU RBMK Rod");

    RBMKFuelRod *rbmk_fuel_lep = new RBMKFuelRod("rbmk_fuel_lep", "Low Enriched Plutonium-239");
    rbmk_fuel_lep
        ->setYield(100000000)
        ->setStats(35)
        ->setFunction(BURNFUNC_LOG_TEN)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(0.75)
        ->setMeltingPoint(2744)
        ->setItemName("LEP-239 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_mep = new RBMKFuelRod("rbmk_fuel_mep", "Medium Enriched Plutonium-239");
    rbmk_fuel_mep
        ->setYield(100000000)
        ->setStats(35)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setMeltingPoint(2744)
        ->setItemName("MEP-239 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hep = new RBMKFuelRod("rbmk_fuel_hep", "Highly Enriched Plutonium-239");
    rbmk_fuel_hep
        ->setYield(100000000)
        ->setStats(30)
        ->setFunction(BURNFUNC_LINEAR)
        ->setHeat(1.25)
        ->setMeltingPoint(2744)
        ->setItemName("HEP-239 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hep241 = new RBMKFuelRod("rbmk_fuel_hep241", "Highly Enriched Plutonium-241");
    rbmk_fuel_hep241
        ->setYield(100000000)
        ->setStats(40)
        ->setFunction(BURNFUNC_LINEAR)
        ->setHeat(1.75)
        ->setMeltingPoint(2744)
        ->setItemName("HEP-241 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_lea = new RBMKFuelRod("rbmk_fuel_lea", "Low Enriched Americium-242");
    rbmk_fuel_lea
        ->setYield(100000000)
        ->setStats(60, 10)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(1.5)
        ->setMeltingPoint(2386)
        ->setItemName("LEA RBMK Rod");

    RBMKFuelRod *rbmk_fuel_mea = new RBMKFuelRod("rbmk_fuel_mea", "Medium Enriched Americium-242");
    rbmk_fuel_mea
        ->setYield(100000000)
        ->setStats(35, 20)
        ->setFunction(BURNFUNC_ARCH)
        ->setHeat(1.75)
        ->setMeltingPoint(2386)
        ->setItemName("MEA RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hea241 = new RBMKFuelRod("rbmk_fuel_hea241", "Highly Enriched Americium-241");
    rbmk_fuel_hea241
        ->setYield(100000000)
        ->setStats(65, 15)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setHeat(1.85)
        ->setMeltingPoint(2386)
        ->setNeutronTypes(NTYPE_FAST, NTYPE_FAST)
        ->setItemName("HEA-241 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hea242 = new RBMKFuelRod("rbmk_fuel_hea242", "Highly Enriched Americium-242");
    rbmk_fuel_hea242
        ->setYield(100000000)
        ->setStats(45)
        ->setFunction(BURNFUNC_LINEAR)
        ->setHeat(2)
        ->setMeltingPoint(2386)
        ->setItemName("HEA-242 RBMK Rod");

    RBMKFuelRod *rbmk_fuel_men = new RBMKFuelRod("rbmk_fuel_men", "Medium Enriched Neptunium-237");
    rbmk_fuel_men 
        ->setYield(100000000)
        ->setStats(30)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setHeat(0.75)
        ->setMeltingPoint(2800)
        ->setNeutronTypes(NTYPE_ANY, NTYPE_FAST)
        ->setItemName("MEN RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hen = new RBMKFuelRod("rbmk_fuel_hen", "Highly Enriched Neptunium-237");
    rbmk_fuel_hen
        ->setYield(100000000)
        ->setStats(40)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setMeltingPoint(2800)
        ->setNeutronTypes(NTYPE_ANY, NTYPE_FAST)
        ->setItemName("HEN RBMK Rod");

    RBMKFuelRod *rbmk_fuel_mox = new RBMKFuelRod("rbmk_fuel_mox", "Mixed MEU & LEP Oxide");
    rbmk_fuel_mox
        ->setYield(100000000)
        ->setStats(40)
        ->setFunction(BURNFUNC_LOG_TEN)
        ->setDepletionFunction(DEPFUNC_RAISING_SLOPE)
        ->setMeltingPoint(2815)
        ->setItemName("MOX RBMK Rod");

    RBMKFuelRod *rbmk_fuel_les = new RBMKFuelRod("rbmk_fuel_les", "Low Enriched Schrabidium-326");
    rbmk_fuel_les
        ->setYield(100000000)
        ->setStats(50)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setHeat(1.25)
        ->setMeltingPoint(2500)
        ->setNeutronTypes(NTYPE_SLOW, NTYPE_SLOW)
        ->setItemName("LES RBMK Rod");

    RBMKFuelRod *rbmk_fuel_mes = new RBMKFuelRod("rbmk_fuel_mes", "Medium Enriched Schrabidium-326");
    rbmk_fuel_mes
        ->setYield(100000000)
        ->setStats(75)
        ->setFunction(BURNFUNC_ARCH)
        ->setHeat(1.5)
        ->setMeltingPoint(2750)
        ->setItemName("MES RBMK Rod");

    RBMKFuelRod *rbmk_fuel_hes = new RBMKFuelRod("rbmk_fuel_hes", "Highly Enriched Schrabidium-326");
    rbmk_fuel_hes
        ->setYield(100000000)
        ->setStats(90)
        ->setFunction(BURNFUNC_LINEAR)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setHeat(1.75)
        ->setMeltingPoint(3000)
        ->setItemName("HES RBMK Rod");

    RBMKFuelRod *rbmk_fuel_leaus = new RBMKFuelRod("rbmk_fuel_leaus", "Low Enriched Australium (Tasmanite)");
    rbmk_fuel_leaus
        ->setYield(100000000)
        ->setStats(30)
        ->setFunction(BURNFUNC_SIGMOID)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.05, 50)
        ->setHeat(1.5)
        ->setItemName("LEAus RBMK Rod");

    RBMKFuelRod *rbmk_fuel_heaus = new RBMKFuelRod("rbmk_fuel_heaus", "Highly Enriched Australium (Ayerite)");
    rbmk_fuel_heaus
        ->setYield(100000000)
        ->setStats(35)
        ->setFunction(BURNFUNC_LINEAR)
        ->setXenon(0.05, 50)
        ->setHeat(1.5)
        ->setItemName("HEAus RBMK Rod");

    RBMKFuelRod *rbmk_fuel_po210be = new RBMKFuelRod("rbmk_fuel_po210be", "Polonium-210 & Beryllium Neutron Source");
    rbmk_fuel_po210be
        ->setYield(25000000)
        ->setStats(0, 50)
        ->setFunction(BURNFUNC_PASSIVE)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setHeat(0.1)
        ->setDiffusion(0.05)
        ->setMeltingPoint(1287)
        ->setNeutronTypes(NTYPE_SLOW, NTYPE_SLOW)
        ->setItemName("Po210Be RBMK Neutron Source");

    RBMKFuelRod *rbmk_fuel_ra226be = new RBMKFuelRod("rbmk_fuel_ra226be", "Radium-226 & Beryllium Neutron Source");
    rbmk_fuel_ra226be
        ->setYield(100000000)
        ->setStats(0, 20)
        ->setFunction(BURNFUNC_PASSIVE)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setHeat(0.035)
        ->setDiffusion(0.5)
        ->setMeltingPoint(700)
        ->setNeutronTypes(NTYPE_SLOW, NTYPE_SLOW)
        ->setItemName("Ra226Be RBMK Neutron Source");

    RBMKFuelRod *rbmk_fuel_pu238be = new RBMKFuelRod("rbmk_fuel_pu238be", "Plutonium-238 & Beryllium Neutron Source");
    rbmk_fuel_pu238be
        ->setYield(50000000)
        ->setStats(40, 40)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setHeat(0.1)
        ->setDiffusion(0.05)
        ->setMeltingPoint(1287)
        ->setNeutronTypes(NTYPE_SLOW, NTYPE_SLOW)
        ->setItemName("Pu238Be RBMK Neutron Source");

    RBMKFuelRod *rbmk_fuel_balefire_gold = new RBMKFuelRod("rbmk_fuel_balefire_gold", "Antihydrogen in a Magnetized Gold-198 Lattice");
    rbmk_fuel_balefire_gold
        ->setYield(100000000)
        ->setStats(50, 10)
        ->setFunction(BURNFUNC_ARCH)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setMeltingPoint(2000)
        ->setItemName("Flashgold RBMK Fuel Rod");

    RBMKFuelRod *rbmk_fuel_flashlead = new RBMKFuelRod("rbmk_fuel_flashlead", "Antihydrogen confined by a Magnetized Gold-198 and Lead-209 Lattice");
    rbmk_fuel_flashlead
        ->setYield(250000000)
        ->setStats(40, 50)
        ->setFunction(BURNFUNC_ARCH)
        ->setDepletionFunction(DEPFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setMeltingPoint(2050)
        ->setItemName("Flashlead RBMK Fuel Rod");

    RBMKFuelRod *rbmk_fuel_balefire = new RBMKFuelRod("rbmk_fuel_balefire", "Draconic Flames");
    rbmk_fuel_balefire
        ->setYield(100000000)
        ->setStats(100, 35)
        ->setFunction(BURNFUNC_LINEAR)
        ->setXenon(0.0, 50)
        ->setHeat(3)
        ->setMeltingPoint(3652)
        ->setItemName("Balefire RBMK Fuel Rod");

    RBMKFuelRod *rbmk_fuel_zfb_bismuth = new RBMKFuelRod("rbmk_fuel_zfb_bismuth", "Zirconium Fast Breeder - LEU/HEP-241#Bi");
    rbmk_fuel_zfb_bismuth
        ->setYield(50000000)
        ->setStats(20)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setHeat(1.75)
        ->setMeltingPoint(2744)
        ->setItemName("Bismuth RBMK ZFB Rod");

    RBMKFuelRod *rbmk_fuel_zfb_pu241 = new RBMKFuelRod("rbmk_fuel_zfb_pu241", "Zirconium Fast Breeder - HEU-235/HEP-240#Pu-241");
    rbmk_fuel_zfb_pu241
        ->setYield(50000000)
        ->setStats(20)
        ->setFunction(BURNFUNC_SQUARE_ROOT)
        ->setMeltingPoint(2865)
        ->setItemName("Pu-241 RBMK ZFB Rod");

    RBMKFuelRod *rbmk_fuel_zfb_am_mix = new RBMKFuelRod("rbmk_fuel_zfb_am_mix", "Zirconium Fast Breeder - HEP-241#MEA");
    rbmk_fuel_zfb_am_mix
        ->setYield(50000000)
        ->setStats(20)
        ->setFunction(BURNFUNC_LINEAR)
        ->setHeat(1.75)
        ->setMeltingPoint(2744)
        ->setItemName("Fuel Grade Americium RBMK ZFB Rod");

    RBMKFuelRod *rbmk_fuel_drx = new RBMKFuelRod("rbmk_fuel_drx", "can't you hear, can't you hear the thunder?");
    rbmk_fuel_drx
        ->setYield(10000000)
        ->setStats(1000, 10)
        ->setFunction(BURNFUNC_QUADRATIC)
        ->setHeat(0.1)
        ->setMeltingPoint(100000)
        ->setItemName("Digamma RBMK Rod");

    TraceLog(LOG_INFO, "RBMK: Configurations defined, registering fuels");

    RegisterFuel("rbmk_fuel_ueu", rbmk_fuel_ueu);
    RegisterFuel("rbmk_fuel_meu", rbmk_fuel_meu);
    RegisterFuel("rbmk_fuel_heu233", rbmk_fuel_heu233);
    RegisterFuel("rbmk_fuel_heu235", rbmk_fuel_heu235);
    RegisterFuel("rbmk_fuel_thmeu", rbmk_fuel_thmeu);
    RegisterFuel("rbmk_fuel_lep", rbmk_fuel_lep);
    RegisterFuel("rbmk_fuel_mep", rbmk_fuel_mep);
    RegisterFuel("rbmk_fuel_hep", rbmk_fuel_hep);
    RegisterFuel("rbmk_fuel_hep241", rbmk_fuel_hep241);
    RegisterFuel("rbmk_fuel_lea", rbmk_fuel_lea);
    RegisterFuel("rbmk_fuel_mea", rbmk_fuel_mea);
    RegisterFuel("rbmk_fuel_hea241", rbmk_fuel_hea241);
    RegisterFuel("rbmk_fuel_hea242", rbmk_fuel_hea242);
    RegisterFuel("rbmk_fuel_men", rbmk_fuel_men);
    RegisterFuel("rbmk_fuel_hen", rbmk_fuel_hen);
    RegisterFuel("rbmk_fuel_mox", rbmk_fuel_mox);
    RegisterFuel("rbmk_fuel_les", rbmk_fuel_les);
    RegisterFuel("rbmk_fuel_mes", rbmk_fuel_mes);
    RegisterFuel("rbmk_fuel_hes", rbmk_fuel_hes);
    RegisterFuel("rbmk_fuel_leaus", rbmk_fuel_leaus);
    RegisterFuel("rbmk_fuel_heaus", rbmk_fuel_heaus);
    RegisterFuel("rbmk_fuel_po210be", rbmk_fuel_po210be);
    RegisterFuel("rbmk_fuel_ra226be", rbmk_fuel_ra226be);
    RegisterFuel("rbmk_fuel_pu238be", rbmk_fuel_pu238be);
    RegisterFuel("rbmk_fuel_balefire_gold", rbmk_fuel_balefire_gold);
    RegisterFuel("rbmk_fuel_flashlead", rbmk_fuel_flashlead);
    RegisterFuel("rbmk_fuel_balefire", rbmk_fuel_balefire);
    RegisterFuel("rbmk_fuel_zfb_bismuth", rbmk_fuel_zfb_bismuth);
    RegisterFuel("rbmk_fuel_zfb_pu241", rbmk_fuel_zfb_pu241);
    RegisterFuel("rbmk_fuel_zfb_am_mix", rbmk_fuel_zfb_am_mix);
    RegisterFuel("rbmk_fuel_drx", rbmk_fuel_drx);

    TraceLog(LOG_INFO, TextFormat("RBMK: Fuel registry size: %i", fuelRegistry.size()));
    TraceLog(LOG_INFO, "RBMK: Finishing fuel initialization");
}

RBMKFuelRod* PrepareFuel(std::string name) {
    RBMKFuelRod* fuel = new RBMKFuelRod(fuelRegistry[name]);
    fuel->active = true;
    return fuel;
}

auto GetFuelItems = [](){
    std::vector<PickerItem> items;
    for (const auto& pair : fuelRegistry) {
        items.push_back({pair.second->internalName, pair.second->getTooltip(), pair.second->tex});
    }
    return items;
};
std::function<std::vector<PickerItem>()> GetFuelItemsLambda() {
    return GetFuelItems;
}