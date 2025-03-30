// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/NeutronStream.java
// https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/handler/neutron/RBMKNeutronHandler.java

// im kind of taking a different approach here compared to how it's actually implemented
// in the actual mod, there is a "nodespace" sort of thing
// i decided that it'd be easier to just create streams, insert them into some sort of array
// then run the interactions after all columns have been updated
// and clear that array at the end of the tick

// maybe im just making an excuse and finding areas to simplify and lose performance in
// maybe this is optimal enough
// whatever. all im hoping for is to not create double the problems that microwave had, especially in a unsafe environment like this

// i'll come back if this is problematic

#include "raylib.h"
#include "raymath.h"

#include "neutronStream.h"

#include "../dials.h"
#include "../../main.h"

// THE COLUMN HORDE (again...)
#include "../columns/columnFuelRod.h"
#include "../columns/columnControlRod.h"
#include "../columns/columnModerator.h"
#include "../columns/columnAbsorber.h"
#include "../columns/columnReflector.h"
#include "../columns/columnOutgasser.h"

NeutronStream::NeutronStream(Vector2 m_origin, Vector2 m_vector, double flux, double ratio) {
    origin = m_origin;
    vector = m_vector;
    fluxQuantity = flux;
    fluxRatio = ratio;
    active = true;
}

void NeutronStream::runStreamInteraction() {
    if (fluxQuantity == 0) return;

    int moderatedCount = 0;
    int fluxRange = rbmkDials.dialFluxRange;

    // god i do sure hope this doesn't cause problems
    ColumnFluxReceiver* originColumn = (ColumnFluxReceiver*) rbmk->getColumn(rbmk->indexFromPos(origin));

    for (int i = 0; i < fluxRange; i++) {
        if (fluxQuantity == 0) return;

        Vector2 pos = Vector2Add(origin, Vector2Scale(vector, 1+(float)i));
        ColumnBase* column = rbmk->getColumn(rbmk->indexFromPos(pos));

        // there's some code here that handles the blocking of the neutron stream via covering with blocks
        // for the sake of simplicity im just gonna make it throw out radiation if there isn't a column there
        if (column->active == false) {
            // irradiate!
            rbmk->emitRadiation(fluxQuantity * 0.05);
            continue;
        }

        // i don't trust c++ enough to do this
        // but im doing it anyway
        // if this causes segfaults feel free to yell
        switch (column->type) {
            case COLUMN_FUEL_SIM:
            case COLUMN_FUEL: {
                ColumnFuelRod* rod = (ColumnFuelRod*) column;
                if (rod->hasRod == true) {
                    rod->receiveFlux(this);
                    return;
                }
                return;
            }
            
            case COLUMN_OUTGASSER: {
                ColumnOutgasser* outgasser = (ColumnOutgasser*) column;
                if (outgasser->canProcess()) {
                    outgasser->receiveFlux(this);
                    return;
                }
                return;
            }

            case COLUMN_CONTROL:
            case COLUMN_CONTROL_AUTO: {
                ColumnControlRod* control = (ColumnControlRod*) column;
                if (control->level > 0.0) {
                    fluxQuantity *= control->getMulti();
                    continue;
                }
                return;
            }

            case COLUMN_REFLECTOR: {
                //ColumnReflector* reflector = (ColumnReflector*) column;
                if (originColumn->moderated == true) moderatedCount++;

                if (fluxRatio > 0 && moderatedCount > 0) {
                    for (int i = 0; i < moderatedCount; i++) {
                        moderateStream();
                    }
                }

                if (rbmkDials.dialReflectorEfficiency != 1.0) {
                    fluxQuantity *= rbmkDials.dialReflectorEfficiency;
                    continue;
                }

                originColumn->receiveFlux(this);
                return;
            }

            case COLUMN_ABSORBER: {
                if(rbmkDials.dialAbsorberEfficiency == 1) return;
                fluxQuantity *= rbmkDials.dialAbsorberEfficiency;
                return;
            }
        }
    }

    // there's probably that control rod bug...
}
void NeutronStream::moderateStream() {
    fluxRatio *= (1.0 - rbmkDials.dialModeratorEfficiency);
}