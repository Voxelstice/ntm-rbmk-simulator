#include "raylib.h"
#include "raymath.h"

#include "itemSlot.h"

#include "../../utils.h"
#include "../../audio.h"
#include "../../tooltip.h"
#include "../../textureCache.h"

ItemSlot::ItemSlot(Vector2 m_position) {
    position = m_position;
    pickerItems.clear();
}

void ItemSlot::setItem(Item* m_item) {
    if (hasItem == true) removeItem();
    
    hasItem = true;
    item = m_item;
    item->position = position;
}
void ItemSlot::removeItem() {
    delete item;
    hasItem = false;
}

void ItemSlot::changePosition(Vector2 m_position) {
    position = m_position;
    if (hasItem == true) {
        item->position = m_position;
    }
}
void ItemSlot::setGetItems(std::function<std::vector<PickerItem>()> newFunc) {
    getItemsReady = true;
    getItems = newFunc;
}

void ItemSlot::update() {
    if (hasItem == true && itemPicker == false)
        item->update();

    if (itemPicker == false) {
        Rectangle rect = {position.x, position.y, 16 * 4, 16 * 4};
        if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Audio_PlaySound(AUDIOSAMPLE_CLICK);
            itemPicker = true;
            itemPickerOffset = 0;

            if (getItemsReady == true) {
                pickerItems = getItems();
            }
        }
    } else {
        Rectangle pickRect = {position.x - 16*4, position.y - 16*4, 48*4, 48*4};

        if (!CheckCollisionPointRec(GetMousePosition(), pickRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Audio_PlaySound(AUDIOSAMPLE_CLICK);
            itemPicker = false;
            pickerItems.clear();
        }

        if (CheckCollisionPointRec(GetMousePosition(), pickRect)) {
            int scroll = (int)GetMouseWheelMove();
            itemPickerOffset -= scroll;
            if (itemPickerOffset < 0) itemPickerOffset = 0;
            if (itemPickerOffset > (int)std::round(pickerItems.size()/3)) itemPickerOffset = (int)std::round(pickerItems.size()/3);
        }

        if (pickerItems.size() == 0) return;
        for (int i2 = 0; i2 < 9; i2++) {
            int i = i2 + (itemPickerOffset*3);
            if (i > pickerItems.size()-1) return;

            Vector2 pos = Vector2Add({pickRect.x, pickRect.y}, {float(i2 % 3) * 16 * 4, float(i2 / 3) * 16 * 4});
            PickerItem item = pickerItems[i];

            Rectangle itemRect = {pos.x, pos.y, 16 * 4, 16 * 4};
            if (CheckCollisionPointRec(GetMousePosition(), itemRect)) {
                DrawRectangleRec(itemRect, Fade(WHITE, 0.5f));
                SetTooltip(item.tooltip);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Audio_PlaySound(AUDIOSAMPLE_CLICK);
                }
            }
        }
    }
}

void ItemSlot::draw() {
    if (hasItem == true)
        item->draw();

    if (itemPicker == false) {
        Rectangle rect = {position.x, position.y, 16 * 4, 16 * 4};
        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            DrawRectangleRec(rect, Fade(WHITE, 0.5f));
        }
    } else {
        Rectangle pickRect = {position.x - 16*4, position.y - 16*4, 48*4, 48*4};
        DrawRectangleRec(pickRect, Fade(BLACK, 0.5f));

        if (pickerItems.size() == 0) return;
        for (int i2 = 0; i2 < 9; i2++) {
            int i = i2 + (itemPickerOffset*3);
            if (i > pickerItems.size()-1) return;

            Vector2 pos = Vector2Add({pickRect.x, pickRect.y}, {float(i2 % 3) * 16 * 4, float(i2 / 3) * 16 * 4});

            PickerItem item = pickerItems[i];
            DrawTextureS(item.tex, {0, 0}, {(float)item.tex.width, (float)item.tex.height}, pos, {16 * 4, 16 * 4}, 1);

            Rectangle itemRect = {pos.x, pos.y, 16 * 4, 16 * 4};
            if (CheckCollisionPointRec(GetMousePosition(), itemRect)) {
                DrawRectangleRec(itemRect, Fade(WHITE, 0.5f));
            }
        }
    }
}