#include "head.h"

// would love to know how to ship these as library

// inline EdgeDirection flip(EdgeDirection dir) {
//   if (dir == EdgeDirection::TopToRight)
//     return EdgeDirection::TopToLeft;
//   if (dir == EdgeDirection::RightToDown)
//     return EdgeDirection::RightToTop;
//   if (dir == EdgeDirection::BottomToLeft)
//     return EdgeDirection::BottomToRight;
//   if (dir == EdgeDirection::LeftToTop)
//     return EdgeDirection::LeftToBottom;
//   if (dir == EdgeDirection::TopToLeft)
//     return EdgeDirection::TopToRight;
//   if (dir == EdgeDirection::RightToTop)
//     return EdgeDirection::RightToDown;
//   if (dir == EdgeDirection::BottomToRight)
//     return EdgeDirection::BottomToLeft;
//   if (dir == EdgeDirection::LeftToBottom)
//     return EdgeDirection::LeftToTop;
//
//   std::cerr << "PANIC\n";
//   return EdgeDirection::LeftToTop;
// }
//
// int smaller(int a, int b) {
//   if (a < b)
//     return a;
//   return b;
// }
