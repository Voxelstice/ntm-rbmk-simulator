// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/RBMKDials.java

#pragma once

typedef struct RBMKDials {
    // ntm dials!
    double dialPassiveCooling = 1.0;
    double dialColumnHeatFlow = 0.2;
    double dialDiffusionMod = 1.0;
    double dialHeatProvision = 0.2;
    int dialColumnHeight = 4;
    double dialBoilerHeatConsumption = 0.1;
    double dialControlSpeed = 1.0;
    double dialReactivityMod = 1.0;
    double dialOutgasserSpeedMod = 1.0;
    double dialControlSurgeMod = 1.0;
    int dialFluxRange = 5;
    int dialReasimRange = 10;
    int dialReasimCount = 6;
    bool dialReasimBoilers = false;
    double dialReasimBoilerSpeed = 0.05;
    bool dialDisableMeltdowns = false;
    double dialModeratorEfficiency = 1.0;
    double dialAbsorberEfficiency = 1.0;
    double dialReflectorEfficiency = 1.0;
    bool dialDisableDepletion = false;
    bool dialDisableXenon = false;

    // program vars
    bool varsEmbedded = false;
} RBMKDials;

extern RBMKDials rbmkDials;