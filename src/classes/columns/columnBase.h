#pragma once

#include "raylib.h"

#include <vector>

enum ColumnType {
    NONE = -1,
    BLANKK = 0, // God damn it raylib
    FUEL = 10,
    FUEL_SIM = 90,
    CONTROL = 20,
    CONTROL_AUTO = 30,
    BOILER = 40,
    MODERATOR = 50,
    ABSORBER = 60,
    REFLECTOR = 70,
    OUTGASSER = 80,
    BREEDER = 100,
    STORAGE = 110,
    COOLER = 120,
    HEATEX = 130
};

class ColumnBase {
    public:
        ColumnBase();

        virtual void update();
        virtual void draw(Vector2 columnSize, Vector2 destPos);

        virtual void init();

        void moveHeat();
        void boilWater();
        void coolPassively();

        virtual void reset();

        // its so tempting to use floats...
        // but for the sake of accuracy we're gonna remain type-accurate
        double heat = 20.0;
        double maxHeat = 1500.0;

        // realishit sim!
        int water = 0;
        int steam = 0;

        int maxWater = 16000;
        int maxSteam = 16000;

        bool active = false;
        ColumnType type = NONE;

        Vector2 pos = {0, 0};

        std::vector<ColumnBase*> neighbors;
        std::vector<ColumnBase*> rec;
};