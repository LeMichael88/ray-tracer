/* //-------------------------------------------------------------------------
//
// polyroots.hpp/polyroots.cpp
//
// Utility functions to solve low-order polynomial equations efficiently
// and robustly.  Very useful when writing ray-object intersection tests.
//
//------------------------------------------------------------------------- */

#include <cstddef>

size_t quadraticRoots(double A, double B, double C, double roots[2]);
size_t cubicRoots(double A, double B, double C, double roots[3]);
size_t quarticRoots(double A, double B, double C, double D, double roots[4]);
