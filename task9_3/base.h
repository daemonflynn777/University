#ifndef BASE_H
#define BASE_H

#include <iostream>

using namespace::std;

class BasicShape
{
	double area;

public:
	BasicShape();
	BasicShape(double aArea);
	~BasicShape();

	void set_area(double aArea);

	double get_area() const;

	virtual double get_size() = 0;
};
#endif
