#include <iostream>
#include "base.h"
#include "square.h"


using namespace::std;

Square::Square() { //default
	setWidth(0);
	setLength(0);
}

Square::Square(long int square_width, long int square_length) : BasicShape() { //basic constructor
	setWidth(square_width);
	setLength(square_length);
}

Square::~Square() { //destructor
	//cout << "D E S T R O Y E D";
}

void Square::setWidth(long int square_width)
{
		if (square_width < 0)
		{
				cerr << "The width can not be a negative number.";
				exit(1);
		}
		else
				width = square_width;
}

void Square::setLength(long int square_length)
{
		if (square_length < 0)
		{
				cerr << "The length can not be a negative number.";
				exit(1);
		}
		else
				length = square_length;
}

long int Square::getWidth() const
{
		return width;
}

long int Square::getLength() const
{
		return length;
}

double Square::get_size()
{
		double area;
		area = length * width;
		return area;
}
