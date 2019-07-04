#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include "base.h"

using namespace::std;

class Square : public BasicShape
{
	long int width, length;

public:
	Square();
	Square(long int aWidth, long int aLength);
	~Square();

	void setWidth(long int aWidth);
	void setLength(long int aLength);

	long int getWidth() const;
	long int getLength() const;

	double get_size() override;
};
#endif
