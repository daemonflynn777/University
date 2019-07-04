#include <iostream>
#include "base.h"

using namespace::std;

BasicShape::BasicShape() { //default
	set_area(0.0);
}

BasicShape::BasicShape(double base_area) { //basic constructor
	set_area(base_area);
}

BasicShape::~BasicShape() { //destructor
	//cout << "D E S T R O Y E D";
}

void BasicShape::set_area(double base_area)
{
		if (base_area < 0)
		{
				cout << "\n>> The area can not be a negative number! >>";
				exit(1);

		}
		else
				area = base_area;
}

double BasicShape::get_area() const
{
		return area;
}
