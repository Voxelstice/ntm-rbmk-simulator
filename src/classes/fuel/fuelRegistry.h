#pragma once

#include <string>
#include <functional>

#include "../inventory/itemSlot.h"
#include "rbmkFuelRod.h"

void RegisterFuel(std::string name, RBMKFuelRod* fuel);
void RegisterFuels();

std::function<std::vector<PickerItem>()> GetFuelItemsLambda();

RBMKFuelRod *PrepareFuel(std::string name);