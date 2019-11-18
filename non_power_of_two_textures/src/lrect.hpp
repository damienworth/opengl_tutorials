#ifndef LRECT_HPP
#define LRECT_HPP

#include "lopengl.hpp"

#include <array>

template <typename Numeric>
using lrect = std::array<Numeric, 4>;

using lfrect = lrect<GLfloat>;

#endif // LRECT_HPP
