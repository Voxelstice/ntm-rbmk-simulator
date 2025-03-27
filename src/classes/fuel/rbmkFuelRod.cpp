// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/machine/ItemRBMKRod.java

#include <string>
#include <cmath>
#include <algorithm>

#include "rbmkFuelRod.h"
#include "../../mathHelper.h"
#include "../dials.h"

RBMKFuelRod::RBMKFuelRod(std::string m_fullName) {
    fullName = m_fullName;
}
RBMKFuelRod::RBMKFuelRod(RBMKFuelRod* fuel) {
    // This constructor is specifically to just "clone" the fuel
    itemName = fuel->itemName;
    fullName = fuel->fullName;

    reactivity = fuel->reactivity;
    selfRate = fuel->selfRate;

    function = fuel->function;
    depFunc = fuel->depFunc;

    xGen = fuel->xGen;
    xBurn = fuel->xBurn;
    heat = fuel->heat;

    yield = fuel->yield;
    meltingPoint = fuel->meltingPoint;
    diffusion = fuel->diffusion;

    nType = fuel->nType;
    rType = fuel->rType;
}

// FUNCTIONS
void RBMKFuelRod::reset() {
    itemYield = yield;
    itemXenon = 0.0;
    itemCoreHeat = 20.0;
    itemHullHeat = 20.0;
}

double RBMKFuelRod::burn(double inFlux) {
    inFlux += selfRate;

    if (rbmkDials.dialDisableXenon == false) {
        itemXenon -= xenonBurnFunc(inFlux);
        inFlux *= (1.0 - getPoisonLevel());
        itemXenon += xenonGenFunc(inFlux);

        if (itemXenon < 0.0) itemXenon = 0.0;
        if (itemXenon > 100.0) itemXenon = 100.0;
    }

    double outFlux = reactivityFunc(inFlux, getEnrichment() * rbmkDials.dialReactivityMod);

    if (rbmkDials.dialDisableDepletion == false) {
        itemYield -= inFlux;

        if (itemYield < 0.0) itemYield = 0.0;
    }

    itemCoreHeat += outFlux * heat;

    //printf("\ninFlux: %i\noutFlux: %i\nitemYield: %i\ncoreHeat: %i\nhullHeat: %i\n", (int)inFlux, (int)outFlux, (int)itemYield, (int)itemCoreHeat, (int)itemHullHeat);

    return outFlux;
}

double rectify(double num) {
    if(num > 1000000.0) num = 1000000.0;
    if(num < 20.0 || isnan(num)) num = 20.0;
    return num;
}

void RBMKFuelRod::updateHeat(double mod) {
    if (itemCoreHeat > itemHullHeat) {
        double mid = (itemCoreHeat - itemHullHeat) / 2;

        itemCoreHeat -= mid * diffusion * rbmkDials.dialDiffusionMod * mod;
        itemHullHeat += mid * diffusion * rbmkDials.dialDiffusionMod * mod;

        itemCoreHeat = rectify(itemCoreHeat);
        itemHullHeat = rectify(itemHullHeat);
    }
}
double RBMKFuelRod::provideHeat(double heat, double mod) {
    if (itemHullHeat > meltingPoint) {
        double avg = (heat + itemHullHeat + itemCoreHeat) / 3.0;
        itemCoreHeat = avg;
        itemHullHeat = avg;
        return avg - heat;
    }

    if (itemHullHeat <= heat)
        return 0.0;

    double ret = (itemHullHeat - heat) / 2.0;
    ret *= rbmkDials.dialHeatProvision * mod;

    itemHullHeat -= ret;
    return ret;
}

double RBMKFuelRod::reactivityFunc(double in, double enrichment) {
    double flux = in * reactivityModByEnrichment(enrichment);
    
    switch(function) {
        case BURNFUNC_PASSIVE: return selfRate * enrichment;
        case BURNFUNC_LOG_TEN: return std::log10(flux + 1.0) * 0.5 * reactivity;
        case BURNFUNC_PLATEU: return (1.0 - std::pow(math_euler, -flux / 25.0)) * reactivity;
        case BURNFUNC_ARCH: return std::max((flux - (flux * flux / 10000.0)) / 100.0 * reactivity, 0.0);
        case BURNFUNC_SIGMOID: return reactivity / (1.0 + std::pow(math_euler, -(flux - 50.0) / 10.0));
        case BURNFUNC_SQUARE_ROOT: return std::sqrt(flux) * reactivity / 10;
        case BURNFUNC_LINEAR: return flux / 100.0 * reactivity;
        case BURNFUNC_QUADRATIC: return flux * flux / 10000.0 * reactivity;
        case BURNFUNC_EXPERIMENTAL: return flux * (std::sin(flux) + 1.0) * reactivity;
    }

    return 0;
}
double RBMKFuelRod::reactivityModByEnrichment(double enrichment) {
    switch (depFunc) {
		default:
		case DEPFUNC_LINEAR: return enrichment;
		case DEPFUNC_STATIC: return 1.0;
		case DEPFUNC_BOOSTED_SLOPE: return enrichment + std::sin((enrichment - 1.0) * (enrichment - 1.0) * math_pi);
		case DEPFUNC_RAISING_SLOPE: return enrichment + (std::sin(enrichment * math_pi) / 2.0);
		case DEPFUNC_GENTLE_SLOPE: return enrichment + (std::sin(enrichment * math_pi) / 3.0);
	}
}

double RBMKFuelRod::xenonGenFunc(double flux) {
    return flux * xGen;
}
double RBMKFuelRod::xenonBurnFunc(double flux) {
    return (flux * flux) / xBurn;
}

double RBMKFuelRod::getEnrichment() {
    return itemYield / yield;
}
double RBMKFuelRod::getPoisonLevel() {
    return itemXenon / 100.0;
}

// special flux curve handling exists here
// since nothing really uses it yet, don't bother

std::string RBMKFuelRod::getFuncDescription() {
    return "Test";
}

// SETTERS
RBMKFuelRod *RBMKFuelRod::setYield(double yield) {
    this->yield = yield;
    return this;
}

RBMKFuelRod *RBMKFuelRod::setStats(double funcEnd) {
    return setStats(funcEnd, 0);
}
RBMKFuelRod *RBMKFuelRod::setStats(double funcEnd, double selfRate) {
    this->reactivity = funcEnd;
    this->selfRate = selfRate;
    return this;
}

RBMKFuelRod *RBMKFuelRod::setFunction(EnumBurnFunc func) {
    this->function = func;
    return this;
}
RBMKFuelRod *RBMKFuelRod::setDepletionFunction(EnumDepleteFunc func) {
    this->depFunc = func;
    return this;
}

RBMKFuelRod *RBMKFuelRod::setXenon(double gen, double burn) {
    this->xGen = gen;
    this->xBurn = burn;
    return this;
}
RBMKFuelRod *RBMKFuelRod::setHeat(double heat) {
    this->heat = heat;
    return this;
}
RBMKFuelRod *RBMKFuelRod::setDiffusion(double diffusion) {
    this->diffusion = diffusion;
    return this;
}
RBMKFuelRod *RBMKFuelRod::setMeltingPoint(double meltingPoint) {
    this->meltingPoint = meltingPoint;
    return this;
}

RBMKFuelRod *RBMKFuelRod::setNeutronTypes(NType nType, NType rType) {
    this->nType = nType;
    this->rType = rType;
    return this;
}