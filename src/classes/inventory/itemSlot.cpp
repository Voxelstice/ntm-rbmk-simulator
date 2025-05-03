#include "raylib.h"
#include "raymath.h"

#include "itemSlot.h"
#include "../../textureCache.h"

ItemSlot::ItemSlot(Vector2 m_position) {
    position = m_position;
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

void ItemSlot::update() {
    if (hasItem == true)
        item->update();

    // TODO: make it so that you can click on the item and open up some sort of item menu for selecting
}

void ItemSlot::draw() {
    if (hasItem == true)
        item->draw();

    Rectangle rect = {position.x, position.y, 16 * 4, 16 * 4};
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        DrawRectangleRec(rect, Fade(WHITE, 0.5f));
    }
}