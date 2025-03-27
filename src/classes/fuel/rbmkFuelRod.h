// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/items/machine/ItemRBMKRod.java

#pragma once

#include <string>

#include "../neutron/neutronStream.h"

enum EnumBurnFunc {
    BURNFUNC_PASSIVE = 0,
    BURNFUNC_LOG_TEN = 1,
    BURNFUNC_PLATEU = 2,
    BURNFUNC_ARCH = 3,
    BURNFUNC_SIGMOID = 4,
    BURNFUNC_SQUARE_ROOT = 5,
    BURNFUNC_LINEAR = 6,
    BURNFUNC_QUADRATIC = 7,
    BURNFUNC_EXPERIMENTAL = 8
};
enum EnumDepleteFunc {
    DEPFUNC_LINEAR = 0, 		//old function
    DEPFUNC_RAISING_SLOPE = 1,	//for breeding fuels such as MEU, maximum of 110% at 28% depletion
    DEPFUNC_BOOSTED_SLOPE = 2,	//for strong breeding fuels such Th232, maximum of 132% at 64% depletion
    DEPFUNC_GENTLE_SLOPE = 3,	//recommended for most fuels, maximum barely over the start, near the beginning
    DEPFUNC_STATIC = 4			//for arcade-style neutron sources
};

// There is likely a better way to do this but im probably not going to

class RBMKFuelRod {
    public:
        RBMKFuelRod(std::string m_fullName);
        RBMKFuelRod(RBMKFuelRod *fuel);

        // FUNCTIONS
        void reset();

        double burn(double inFlux);

        void updateHeat(double mod);
        double provideHeat(double heat, double mod);

        double reactivityFunc(double in, double enrichment);
        double reactivityModByEnrichment(double enrichment);

        double xenonGenFunc(double flux);
        double xenonBurnFunc(double flux);

        double getEnrichment();
        double getPoisonLevel();

        // special flux curve handling exists here
        // since nothing really uses it yet, don't bother

        std::string getFuncDescription();

        // SETTERS
        RBMKFuelRod *setYield(double yield);

        RBMKFuelRod *setStats(double funcEnd);
        RBMKFuelRod *setStats(double funcEnd, double selfRate);

        RBMKFuelRod *setFunction(EnumBurnFunc func);
        RBMKFuelRod *setDepletionFunction(EnumDepleteFunc func);

        RBMKFuelRod *setXenon(double gen, double burn);
        RBMKFuelRod *setHeat(double heat);
        RBMKFuelRod *setDiffusion(double diffusion);
        RBMKFuelRod *setMeltingPoint(double meltingPoint);

        RBMKFuelRod *setNeutronTypes(NType nType, NType rType);

        // VARIABLES
        std::string itemName = "";
        std::string fullName = "";

        bool active = false;

        double reactivity;
        double selfRate;

        EnumBurnFunc function = BURNFUNC_LOG_TEN;
        EnumDepleteFunc depFunc = DEPFUNC_GENTLE_SLOPE;

        double xGen = 0.5;
        double xBurn = 50;
        double heat = 1;

        double yield;
        double meltingPoint = 1000;
        double diffusion = 0.2;

        NType nType = NTYPE_SLOW;
        NType rType = NTYPE_FAST;

        // ITEM VARIABLES
        double itemYield = 0.0;
        double itemXenon = 0.0;
        double itemCoreHeat = 20.0;
        double itemHullHeat = 20.0;

};