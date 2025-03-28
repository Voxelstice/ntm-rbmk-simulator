// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/NeutronStream.java
// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/RBMKNeutronHandler.java

#pragma once

#include "raylib.h"

enum NType {
    NTYPE_SLOW = 0,
    NTYPE_FAST = 1,
    NTYPE_ANY = 2
};

class NeutronStream {
    public:
        NeutronStream(Vector2 m_origin, Vector2 m_vector, double flux, double ratio);

        Vector2 origin;
        Vector2 vector;

        double fluxQuantity;
        double fluxRatio;
};