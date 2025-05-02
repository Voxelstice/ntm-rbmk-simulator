#include "raylib.h"

#include "submenu.h"
#include "../../utils.h"

Submenu::Submenu(Vector2 m_columnPos) {
    columnPos = m_columnPos;
}

void Submenu::open() {}
void Submenu::update() {}
void Submenu::draw() { }
void Submenu::close() {}