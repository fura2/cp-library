#pragma once

#include "algebra/collection/add_group.hpp"
#include "mint/mint.hpp"

using MintAddGroup = AddGroup<mint, []() { return mint(0); }>;
