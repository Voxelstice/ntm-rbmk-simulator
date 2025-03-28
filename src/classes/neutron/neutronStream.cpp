// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/NeutronStream.java
// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/RBMKNeutronHandler.java

#include "neutronStream.h"

NeutronStream::NeutronStream(Vector2 m_origin, Vector2 m_vector, double flux, double ratio) {
    origin = m_origin;
    vector = m_vector;
    fluxQuantity = flux;
    fluxRatio = ratio;
}