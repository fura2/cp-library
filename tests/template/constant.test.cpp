#include "template/constant.hpp"

#include <limits>

static_assert(INF > 0 && INF <= std::numeric_limits<int>::max() / 2);
static_assert(LINF > 0 && LINF <= std::numeric_limits<long long>::max() / 2);
static_assert(DINF > 0 && DINF < std::numeric_limits<double>::max());

int main() {}
