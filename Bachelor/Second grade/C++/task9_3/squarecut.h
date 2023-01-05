#ifndef SQUARECUT_H
#define SQUARECUT_H

#include <iostream>
#include "square.h"
#include "circle.h"

using namespace::std;

class Squarecut: public Square, public Circle {
  double squarecut_size;

public:
  Squarecut();
  Squarecut(long int cut_width, long int cut_length, double cut_radius);
  ~Squarecut();

  double get_squarecut_size();
};
#endif
