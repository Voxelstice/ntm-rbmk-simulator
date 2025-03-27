#pragma once

#include <string>

#include "rbmkFuelRod.h"

void RegisterFuel(std::string name, RBMKFuelRod* fuel);
void RegisterFuels();

RBMKFuelRod *PrepareFuel(std::string name);