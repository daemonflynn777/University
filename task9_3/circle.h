#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include "base.h"

using namespace::std;

class Circle : public BasicShape
{
	long int centerX, centerY;
	double radius;

public:
	Circle();
	Circle(long int circle_x, long int circle_y, double circle_radius);
	~Circle();

	void setCenterX(long int aCenterX);
	void setCenterY(long int aCenterY);
	void setRadius(double aRadius);

	long int getCenterX() const;
	long int getCenterY() const;
	double getRadius() const;

	double get_size() override;
};
#endif
