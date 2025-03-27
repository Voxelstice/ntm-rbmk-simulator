#pragma once

#include "raylib.h"

#include <vector>

enum ColumnType {
    COLUMN_NONE = -1,
    COLUMN_BLANK = 0,
    COLUMN_FUEL = 10,
    COLUMN_FUEL_SIM = 90,
    COLUMN_CONTROL = 20,
    COLUMN_CONTROL_AUTO = 30,
    COLUMN_BOILER = 40,
    COLUMN_MODERATOR = 50,
    COLUMN_ABSORBER = 60,
    COLUMN_REFLECTOR = 70,
    COLUMN_OUTGASSER = 80,
    COLUMN_BREEDER = 100,
    COLUMN_STORAGE = 110,
    COLUMN_COOLER = 120,
    COLUMN_HEATEX = 130
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
        ColumnType type = COLUMN_NONE;

        Vector2 pos = {0, 0};

        std::vector<ColumnBase*> neighbors;
        std::vector<ColumnBase*> rec;
};