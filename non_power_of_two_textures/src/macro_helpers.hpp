#ifndef MACRO_HELPERS_HPP
#define MACRO_HELPERS_HPP

/*
Since we work with std::array's instead of separate values and we want to get
all the best from both world (memory contigous chunks of memory instead of
separate vars) and still maintain readability, thus we have these macroses
*/

// X coordinate
#define Xc(point) (point[0])

// X coordinate
#define Yc(point) (point[1])

// Width value
#define Wv(dims) (dims[0])

// Height value
#define Hv(dims) (dims[1])

// Left value
#define Lv(rect) (rect[0])

// Top value
#define Tv(rect) (rect[1])

// Right value
#define Rv(rect) (rect[2])

// Bottom value
#define Bv(rect) (rect[3])

#endif // MACRO_HELPERS_HPP

// takes 37 lines of assembly code in CE