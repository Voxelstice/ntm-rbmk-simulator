#include "raylib.h"
#include "raymath.h"

#include <exception>
#include <fstream>
#include <vector>
#include <cmath>

#include "dials.h"
#include "rbmk.h"

#include "../main.h"
#include "../audio.h"
#include "../utils.h"
#include "../tooltip.h"

#include "fuel/fuelRegistry.h"

#include "../libs/tinyfiledialogs.h"
#include "../libs/json.hpp"

// THE COLUMN HORDE
#include "columns/columnBlank.h"
#include "columns/columnFuelRod.h"
#include "columns/columnControlRod.h"
#include "columns/columnControlRodAuto.h"
#include "columns/columnBoiler.h"
#include "columns/columnModerator.h"
#include "columns/columnAbsorber.h"
#include "columns/columnReflector.h"
#include "columns/columnOutgasser.h"
#include "columns/columnStorage.h"
#include "columns/columnCooler.h"
#include "columns/columnHeatExchanger.h"

RBMK::RBMK() {
    if (rbmkDials.varsEmbedded == false) columnGridPosition = {86, 11};
    else columnGridPosition = {0, 0};

    streams.clear();
    columns.clear();

    for (int i = 0; i < 15 * 15; i++) {
        ColumnBase *column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    placeColumn({7, 7}, new ColumnBlank()); // center

    radiationEmitted = 0;
}

// main
void RBMK::updateControl() {
    bool pressedUp = IsKeyPressed(KEY_UP);
    bool pressedDown = IsKeyPressed(KEY_DOWN);

    for (int i = 0; i < 15*15; i++) {
        if (columns[i]->active == false) continue;

        if (columns[i]->type == COLUMN_CONTROL) {
            ColumnControlRod *rod = (ColumnControlRod *)columns[i];
            if (pressedUp) rod->setTarget(rod->targetLevel+0.1);
            if (pressedDown) rod->setTarget(rod->targetLevel-0.1);
        }
    }
}
void RBMK::update() {
    if (state == RUNNING) {
        // reset stuff
        radiationEmitted = 0;

        // update cooling loop
        coolingLoop->update();

        // update columns
        double flux = 0;
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->update();
            if (columns[i]->doBaseUpdate == true) columns[i]->baseUpdate();

            if (columns[i]->type == COLUMN_FUEL) {
                ColumnFuelRod* rod = (ColumnFuelRod*) columns[i];
                flux += rod->lastFluxQuantity;
            }
        }
        printf("total flux: %.1f\n", flux);

        // update streams
        for (NeutronStream* stream : streams) {
            stream->runStreamInteraction();
        }

        // update flux
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;
            if (columns[i]->type == COLUMN_FUEL) {
                ColumnFuelRod* rod = (ColumnFuelRod*) columns[i];
                rod->_receiveFlux();
            }
        }

        streams.clear();
    } else if (state == MELTED) {

    } else {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseReset();
            columns[i]->reset();
        }
    }
}
void RBMK::draw() {
    for (int i = 0; i < 15*15; i++) {
        ColumnBase *column = columns[i];

        int x = i % 15;
        int y = i / 15;

        if (column->active == true) {
            Vector2 columnSize = {10, 10};
            Vector2 destPos = Vector2Add(columnGridPosition, {columnSize.x * float(x), columnSize.y * float(y)});

            // column itself
            column->draw(columnSize, destPos);

            // heat
            float heatAlpha = Clamp((float) ((column->heat-20.0) / column->maxHeat), 0.0f, 1.0f);
            float heatY = std::round(10.0f * heatAlpha);
            DrawTextureS(controlPanel->ui, {0, 182+columnSize.x-heatY}, {10, heatY}, Vector2Add(destPos, {0, columnSize.y-heatY}), {10, heatY}, 4);

            if (state != OFFLINE && indexFromPos(rbmkBuilder->getSelectedPosition()) == i && rbmkBuilder->submenuActive == false) {
                std::string tooltipData = "";
                tooltipData += rbmkBuilder->getStringFromType(column->type);
                tooltipData += "\n";
                tooltipData += "Column Temperature: " + std::string(TextFormat("%.1f", column->heat)) + " C";

                // for the moron (that is me by the way)
                // this is where you get all the info
                // https://github.com/HbmMods/Hbm-s-Nuclear-Tech-GIT/blob/master/src/main/java/com/hbm/tileentity/machine/rbmk/TileEntityRBMKConsole.java#L405
                std::vector<std::string> info = column->getInfo();
                for (std::string str : info) {
                    tooltipData += "\n";
                    tooltipData += str;
                }

                if (column->moderated == true) {
                    tooltipData += "\n";
                    tooltipData += "Moderated";
                }
                
                SetTooltip(tooltipData.c_str());
            }
        }
    }
}
void RBMK::reset() {
    radiationEmitted = 0;
    streams.clear();
    columns.clear();

    for (int i = 0; i < 15*15; i++) {
        ColumnBase* column = new ColumnBase();
        column->active = false;
        column->pos = posFromIndex(i);

        columns.push_back(column);
    }

    columns[112]->active = true;
}

// neutrons
void RBMK::addStream(NeutronStream* stream) {
    streams.push_back(stream);
}

// radiation
void RBMK::emitRadiation(double rad) {
    radiationEmitted += rad;
}

// states
void RBMK::changeState(RBMKState newState) {
    if (newState == RUNNING) {
        coolingLoop->reset();

        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;
            if (columns[i]->type == COLUMN_NONE) {
                columns[i]->active = false;
                continue;
            }

            // reset just incase
            columns[i]->baseReset();
            columns[i]->reset();

            // then init
            columns[i]->baseInit();
            columns[i]->init();
        }
    } else if (newState == MELTED) {

    } else if (newState == OFFLINE) {
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;

            columns[i]->baseReset();
            columns[i]->reset();
        }
    }

    state = newState;
}
void RBMK::meltdown() {
    Audio_PlaySound(AUDIOSAMPLE_EXPLOSION);
    changeState(MELTED);
}

// other
void RBMK::az5() {
    if (state == RUNNING) {
        Audio_PlaySound(AUDIOSAMPLE_SHUTDOWN);
        for (int i = 0; i < 15*15; i++) {
            if (columns[i]->active == false) continue;
            if (columns[i]->type == COLUMN_CONTROL) {
                ColumnControlRod* rod = (ColumnControlRod*) columns[i];
                rod->setTarget(0.0f);
            }
        }
    }
}
void RBMK::cycleCompressors() {
    for (int i = 0; i < 15*15; i++) {
        if (columns[i]->active == false) continue;
        if (columns[i]->type == COLUMN_BOILER) {
            ColumnBoiler* boiler = (ColumnBoiler*) columns[i];
            boiler->cycleCompressor();
        }
    }
}
void RBMK::selectControlRods() {
    for (int i = 0; i < 15*15; i++) {
        if (columns[i]->active == false) continue;
        if (columns[i]->type == COLUMN_CONTROL)
            rbmkBuilder->setSelectorTile(i, true);
    }
}
void RBMK::selectGroup(ColumnColor col) {
    for (int i = 0; i < 15*15; i++) {
        if (columns[i]->active == false) continue;
        if (columns[i]->type == COLUMN_CONTROL && columns[i]->col == col)
            rbmkBuilder->setSelectorTile(i, true);
    }
}
void RBMK::assignGroup(ColumnColor col) {
    for (int i = 0; i < 15*15; i++) {
        if (rbmkBuilder->getSelectorTile(i) == false) continue;
        if (columns[i]->active == false) continue;
        if (columns[i]->type == COLUMN_CONTROL)
            columns[i]->col = col;
    }
}

// columns
ColumnBase* RBMK::placeColumn(Vector2 pos, ColumnBase* column) {
    column->active = true;
    column->pos = pos;
    columns[indexFromPos(pos)] = column;
    return column;
}
ColumnBase* RBMK::makeColumnFromType(ColumnType type) {
    switch (type) {
        case COLUMN_BLANK:          return new ColumnBlank();
        case COLUMN_FUEL:           return new ColumnFuelRod();
        case COLUMN_CONTROL:        return new ColumnControlRod();
        case COLUMN_CONTROL_AUTO:   return new ColumnControlRodAuto();
        case COLUMN_BOILER:         return new ColumnBoiler();
        case COLUMN_MODERATOR:      return new ColumnModerator();
        case COLUMN_ABSORBER:       return new ColumnAbsorber();
        case COLUMN_REFLECTOR:      return new ColumnReflector();
        case COLUMN_OUTGASSER:      return new ColumnOutgasser();
        case COLUMN_STORAGE:        return new ColumnStorage();
        case COLUMN_COOLER:         return new ColumnCooler();
        case COLUMN_HEATEX:         return new ColumnHeatExchanger();

        default:                    return new ColumnBase();
    }
}
ColumnBase* RBMK::getColumn(int i) {
    return columns[i];
}

// utils
Vector2 RBMK::posFromIndex(int i) {
    int x = i % 15;
    int y = i / 15;
    return {float(x), float(y)};
}
int RBMK::indexFromPos(Vector2 pos) {
    return int(pos.y * 15 + pos.x);
}

// saving
#define RBMKVS_MAGIC "rbmkvs03"
#define RBMKVS_VERSION 1
typedef struct RBMKVSHeader {
    char magic[9];
    uint8_t version;
    uint32_t dataSize;
} RBMKVSHeader;

void RBMK::designExport() {
    const char* lFilterPatterns[1] = { "*.rbmkvs" }; // RBMK voxel sim
    const char* selection = tinyfd_saveFileDialog(
        "Select file",   // title
        NULL,          // optional initial directory
        1,               // number of filter patterns
        lFilterPatterns, // char const * lFilterPatterns[2] = { "*.txt", "*.jpg" };
        NULL            // optional filter description
    );

    std::ofstream file;
    file.open(selection, std::ios::binary);

    RBMKVSHeader header = { RBMKVS_MAGIC };
    header.version = RBMKVS_VERSION;
    header.dataSize = 69420;

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    file.close();

    printf("%s\n", selection);
}
void RBMK::designImport() {
    // consider https://github.com/btzy/nativefiledialog-extended instead?
    const char* lFilterPatterns[2] = { "*.json", "*.rbmkvs" }; // RBMK voxel sim
    const char* selection = tinyfd_openFileDialog(
        "Select file",   // title
        NULL,          // optional initial directory
        2,               // number of filter patterns
        lFilterPatterns, // char const * lFilterPatterns[2] = { "*.txt", "*.jpg" };
        NULL,            // optional filter description
        0                // forbids multiple selections
    );

    if (FileExists(selection)) {
        std::ifstream file;
        file.open(selection);

        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            unsigned char* data = (unsigned char*)malloc(fileSize-6); // there's garbage data at the end. i can't tell how to deal with this properly
            file.read((char*)data, fileSize);

            file.close();

            // get header
            unsigned char* dataPtr = data; // idk why this is necessary
            RBMKVSHeader* dataHeader = (RBMKVSHeader*)dataPtr;

            // we got the file. check its format.
            if (memcmp(data, (unsigned char*)"{\"version\":\"rbmk_2\"", 19) == 0) {
                // we're using the web NTM RBMK Simulator save data
                // so lets load json instead
                try {
                    nlohmann::json json = nlohmann::json::parse(data);
                    std::string version = "unknown";

                    if (json["version"] != NULL) version = ((std::string)json["version"]);
                    else throw std::exception("\"version\" not found");

                    if (json["version"] == NULL) throw std::exception("\"data\" not found");
                    if (json["rbmk"] == NULL) throw std::exception("\"rbmk\" not found");

                    if (version != "rbmk_2")
                        TraceLog(LOG_WARNING, "RBMK: Non-standard version (%s) for file %s, tread with caution", version, selection);

                    if ((int)json["rbmk"]["width"] != 15 || (int)json["rbmk"]["height"] != 15) {
                        int answer = tinyfd_messageBox("NTM RBMK Simulator", TextFormat("Non-standard RBMK size (%ix%i), import anyway?", (int)json["rbmk"]["width"], (int)json["rbmk"]["height"]), "yesno", "warning", 2);
                        if (answer == 0) {
                            throw std::exception("Non-standard RBMK size, user refused to import");
                        }
                    }
                    
                    // parse column data
                    reset();
                    int columnIndex = 0;
                    for (auto& el : json["data"].items()) {
                        auto column = el.value();
                        if (!column["class"].is_null()) {
                            std::string className = (std::string)column["class"];
                            ColumnType type = COLUMN_NONE;

                            // i apologize for this mess. but you kind of can't do switch block with std::string

                            if (className == "blank")            type = COLUMN_BLANK;
                            else if (className == "fuel")        type = COLUMN_FUEL;
                            else if (className == "control")     type = COLUMN_CONTROL;
                            else if (className == "controlauto") type = COLUMN_CONTROL_AUTO;
                            else if (className == "boiler")      type = COLUMN_BOILER;
                            else if (className == "moderator")   type = COLUMN_MODERATOR;
                            else if (className == "absorber")    type = COLUMN_ABSORBER;
                            else if (className == "reflector")   type = COLUMN_REFLECTOR;
                            else if (className == "storage")     type = COLUMN_STORAGE;
                            else if (className == "cooler")      type = COLUMN_COOLER;

                            ColumnBase* newColumn = makeColumnFromType(type);
                            ColumnBase* simColumn = placeColumn(posFromIndex(columnIndex), newColumn);

                            // change some more variables
                            if (column["variables"]["moderated"] == true) simColumn->moderated = true;
                            
                            if (type == COLUMN_BOILER) {
                                ((ColumnBoiler*)simColumn)->setCompressionLevel((int)column["variables"]["steamType"]);
                            } else if (type == COLUMN_FUEL) {
                                ColumnFuelRod* rod = (ColumnFuelRod*) simColumn;

                                std::string fuelConst = (std::string)column["variables"]["fuel"]["construct"];
                                if (fuelConst != "NONE") {
                                    std::string fuelInternal = "rbmk_fuel_" + fuelConst;
                                    std::transform(fuelInternal.begin(), fuelInternal.end(), fuelInternal.begin(), [](unsigned char c){ return std::tolower(c); });

                                    rod->fuel = PrepareFuel(fuelInternal);
                                    rod->fuel->reset();

                                    rod->itemSlot->setItem(new Item(rod->itemSlot->position, rod->fuel->internalName, rod->fuel->tex));
                                    rod->itemSlot->item->setTooltip(rod->fuel->getTooltip());
                                }
                            }
                        }
                        columnIndex++;
                    }

                    // parse dials

                    // done!
                    tinyfd_messageBox("NTM RBMK Simulator", "Old RBMK data has been imported, please check your fuels", "ok", "info", 0);
                } catch (const nlohmann::json::exception& err) {
                    TraceLog(LOG_ERROR, "RBMK: An error occurred parsing JSON data:\n%s", err.what());
                    tinyfd_messageBox("NTM RBMK Simulator", TextFormat("An error occurred parsing JSON data:\n%s", err.what()), "ok", "error", 0);
                } catch (const std::exception& err) {
                    TraceLog(LOG_ERROR, "RBMK: An error occurred parsing RBMK data:\n%s", err.what());
                    tinyfd_messageBox("NTM RBMK Simulator", TextFormat("An error occurred parsing RBMK data:\n%s", err.what()), "ok", "error", 0);
                }
            } else if (memcmp(dataHeader->magic, RBMKVS_MAGIC, sizeof(RBMKVS_MAGIC)) == 0) {
                if (dataHeader->version != RBMKVS_VERSION) {
                    tinyfd_messageBox("NTM RBMK Simulator", TextFormat("Unsupported RBMKVS version (%i)", dataHeader->version), "ok", "error", 0);
                    return;
                }
                // new format
                printf("Correct! %i\n", dataHeader->dataSize);
            } else {
                tinyfd_messageBox("NTM RBMK Simulator", TextFormat("Unknown format of file %s", selection), "ok", "error", 0);
            }
        } else {
            tinyfd_messageBox("NTM RBMK Simulator", TextFormat("Couldn't open %s", selection), "ok", "error", 0);
            file.close();
        }

        printf("%s\n", selection);
    }
}