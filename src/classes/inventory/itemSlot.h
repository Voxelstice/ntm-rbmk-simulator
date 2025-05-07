#pragma once

#include "raylib.h"
#include "raymath.h"

#include <functional>

#include "item.h"

typedef struct PickerItem {
    std::string internal;
    std::string tooltip;
    Texture2D tex;
} PickerItem;

class ItemSlot {
    public:
        ItemSlot(Vector2 m_position);

        void setItem(Item* m_item);
        void removeItem();

        void changePosition(Vector2 m_position);
        void setGetItems(std::function<std::vector<PickerItem>()> newFunc);
        void setItemPicked(std::function<void(std::string)> newFunc);

        void update();
        void draw();

        Vector2 position = Vector2Zero();

        bool hasItem = false;
        Item* item;

        bool itemPicker = false;
        int itemPickerOffset = 0;
        std::vector<PickerItem> pickerItems;

        // lambdas
        bool getItemsReady = false;
        std::function<std::vector<PickerItem>()> getItems;

        bool itemPickedReady = false;
        std::function<void(std::string)> itemPicked;
};