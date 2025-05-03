#pragma once

#include "raylib.h"
#include "raymath.h"

#include "item.h"

class ItemSlot {
    public:
        ItemSlot(Vector2 m_position);

        void setItem(Item* m_item);
        void removeItem();

        void changePosition(Vector2 m_position);

        void update();
        void draw();

        Vector2 position = Vector2Zero();

        bool hasItem = false;
        Item* item;
};