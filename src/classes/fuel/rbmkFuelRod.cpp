// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/machine/ItemRBMKRod.java

#include <string>
#include <cmath>
#include <algorithm>

#include "rbmkFuelRod.h"
#include "../../utils.h"
#include "../../mathHelper.h"
#include "../../textureCache.h"
#include "../dials.h"

RBMKFuelRod::RBMKFuelRod(std::string m_internalName, std::string m_fullName) {
    internalName = m_internalName;
    fullName = m_fullName;
    tex = TexCache_Get(TextFormat("assets/fuel/%s.png", m_internalName.c_str()));
}
RBMKFuelRod::RBMKFuelRod(RBMKFuelRod* fuel) {
    // This constructor is specifically to just "clone" the fuel
    internalName = fuel->internalName;
    itemName = fuel->itemName;
    fullName = fuel->fullName;
    tex = TexCache_Get(TextFormat("assets/fuel/%s.png", fuel->internalName.c_str()));

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

// special flux curve handling exists here
// since nothing really uses it yet, don't bother

// TYPES TO STRINGS
const char* RBMKFuelRod::nTypeString(NType type, bool digamma) {
    if (digamma == false) {
        switch (type) {
            case NTYPE_ANY:     return "All Neutrons";
            case NTYPE_SLOW:    return "Slow Neutrons";
            case NTYPE_FAST:    return "Fast Neutrons";
        }
    } else if (digamma == true) {
        switch (type) {
            case NTYPE_ANY:     return "All non-euclidean shapes";
            case NTYPE_SLOW:    return "Hyperbolic non-euclidean shapes";
            case NTYPE_FAST:    return "Elliptic non-euclidean shapes";
        }
    }

    return "ERROR";
}
const char* RBMKFuelRod::fluxTypeString(EnumBurnFunc func) {
    switch (func) {
        case BURNFUNC_PASSIVE:      return "SAFE / PASSIVE";
        case BURNFUNC_LOG_TEN:      return "MEDIUM / LOGARITHMIC";
        case BURNFUNC_PLATEU:       return "SAFE / EULER";
        case BURNFUNC_ARCH:         return "DANGEROUS / NEGATIVE-QUADRATIC";
        case BURNFUNC_SIGMOID:      return "SAFE / SIGMOID";
        case BURNFUNC_SQUARE_ROOT:  return "MEDIUM / SQUARE ROOT";
        case BURNFUNC_LINEAR:       return "DANGEROUS / LINEAR";
        case BURNFUNC_QUADRATIC:    return "DANGEROUS / QUADRATIC";
        case BURNFUNC_EXPERIMENTAL: return "EXPERIMENTAL / SINE SLOPE";

        default:                    return "ERROR";
    }
}

// GETTERS
double RBMKFuelRod::getEnrichment() {
    return itemYield / yield;
}
double RBMKFuelRod::getPoisonLevel() {
    return itemXenon / 100.0;
}

std::string RBMKFuelRod::getFuncDescription() {
    std::string func = "";

    switch (function) {
        case BURNFUNC_PASSIVE:      func = "%1s";
            break;
        case BURNFUNC_LOG_TEN:      func = "log10(%1s + 1) * 0.5 * %2s";
            break;
        case BURNFUNC_PLATEU:       func = "(1 - e^(-%1s / 25)) * %2s";
            break;
        case BURNFUNC_ARCH:         func = "(%1s - %1s^2 / 10000) / 100 * %2s [0;inf]";
            break;
        case BURNFUNC_SIGMOID:      func = "%2s / (1 + e^(-(%1s - 50) / 10))";
            break;
        case BURNFUNC_SQUARE_ROOT:  func = "sqrt(%1s) * %2s / 10";
            break;
        case BURNFUNC_LINEAR:       func = "%1s / 100 * %2s";
            break;
        case BURNFUNC_QUADRATIC:    func = "%1s^2 / 10000 * %2s";
            break;
        case BURNFUNC_EXPERIMENTAL: func = "%1s * (sin(%1s) + 1) * %2s";
            break;
        default:                    func = "ERROR";
            break;
    }

    std::string selfRateStr = selfRate > 0 ? TextFormat("%.1f", selfRate) : "x";
    std::string reactivityStr = TextFormat("%.1f", reactivity);

    double enrichment = getEnrichment();
    if (enrichment < 1) {
        enrichment = reactivityModByEnrichment(enrichment);
        reactivityStr = TextFormat("%.1f", reactivity * enrichment);
    }

    func = ReplaceString(func, "%1s", selfRateStr);
    func = ReplaceString(func, "%2s", reactivityStr);

    return func;
}
std::string RBMKFuelRod::getTooltip() {
    std::vector<std::string> strs;

    strs.push_back(itemName);
    strs.push_back(fullName); // this should be in grey but i don't have a formatting system

    // due to raylib text limitations
    // the fractional 1/2, small top 2 and infinity has to be regular characters instead

    if (internalName == "rbmk_fuel_drx") {
        if (itemHullHeat >= 50 || itemCoreHeat >= 50) strs.push_back("Cool in a Spent Fuel Pool Drum");
        if (selfRate > 0 || this->function == BURNFUNC_SIGMOID) strs.push_back("Self-combusting");

        // begins the big long list
        strs.push_back(TextFormat("Crustyness: %.3f%%", (1.0-getEnrichment())*100.0));
        strs.push_back(TextFormat("Lead poison: %.3f%%", getPoisonLevel()*100.0f));
        strs.push_back(TextFormat("Arrives from: %s", nTypeString(nType, false)));
        strs.push_back(TextFormat("Departs to: %s", nTypeString(rType, false)));
        strs.push_back(TextFormat("Doom function: %s", getFuncDescription().c_str()));
        strs.push_back(TextFormat("Function specification: %s", fluxTypeString(function)));
        strs.push_back(TextFormat("Lead creation function: x * %.2f", xGen));
        strs.push_back(TextFormat("Lead destruction function: x^2 / %.1f", xBurn));
        strs.push_back(TextFormat("Crust per tick at full power: %.3f C", heat));
        strs.push_back(TextFormat("Flow: %.2f 1/2", diffusion));
        strs.push_back(TextFormat("Skin entropy: %.1fm", itemHullHeat));
        strs.push_back(TextFormat("Core entropy: %.1fm", itemCoreHeat));
        strs.push_back(TextFormat("Crush depth: %.1fm", meltingPoint));
    } else {
        if (itemHullHeat >= 50 || itemCoreHeat >= 50) strs.push_back("Cool in a Spent Fuel Pool Drum");
        if (selfRate > 0 || this->function == BURNFUNC_SIGMOID) strs.push_back("Self-igniting");

        // begins the big long list
        strs.push_back(TextFormat("Depletion: %.3f%%", (1.0-getEnrichment())*100.0));
        strs.push_back(TextFormat("Xenon poison: %.3f%%", getPoisonLevel()*100.0f));
        strs.push_back(TextFormat("Splits with: %s", nTypeString(nType, false)));
        strs.push_back(TextFormat("Splits into: %s", nTypeString(rType, false)));
        strs.push_back(TextFormat("Flux function: %s", getFuncDescription().c_str()));
        strs.push_back(TextFormat("Function type: %s", fluxTypeString(function)));
        strs.push_back(TextFormat("Xenon gen function: x * %.2f", xGen));
        strs.push_back(TextFormat("Xenon burn function: x^2 / %.1f", xBurn));
        strs.push_back(TextFormat("Heat per flux: %.3f C", heat));
        strs.push_back(TextFormat("Diffusion: %.2f 1/2", diffusion));
        strs.push_back(TextFormat("Skin temp: %.1f C", itemHullHeat));
        strs.push_back(TextFormat("Core temp: %.1f C", itemCoreHeat));
        strs.push_back(TextFormat("Melting point: %.1f C", meltingPoint));
    }

    return StringConcat(strs, "\n");
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

RBMKFuelRod *RBMKFuelRod::setItemName(std::string itemName) {
    this->itemName = itemName;
    return this;
}