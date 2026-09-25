#pragma once

#include "algebra/collection/add_group.hpp"
#include "algebra/pair_group.hpp"

using LintAddIntAddPairGroup = PairGroup<LintAddGroup, IntAddGroup>;
using LintAddLintAddPairGroup = PairGroup<LintAddGroup, LintAddGroup>;
