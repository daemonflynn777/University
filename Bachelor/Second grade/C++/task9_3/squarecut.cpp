#include <iostream>
#include "square.h"
#include "circle.h"
#include "squarecut.h"

using namespace::std;

Squarecut::Squarecut() { //default
    squarecut_size = 0;
}

Squarecut::Squarecut(long int cut_width, long int cut_length, double cut_radius): Square(cut_width, cut_length), Circle(0, 0, cut_radius) {
    if ((Square::getWidth() >= 2*Circle::getRadius()) && (Square::getLength() >= 2*Circle::getRadius()))
        squarecut_size = Square::get_size() - Circle::get_size();
    else
        cerr << "> Can't make a cut! Radius too big.\n";
}

Squarecut::~Squarecut() { //destructor
	//cout << "D E S T R O Y E D";
}

double Squarecut::get_squarecut_size()
{
    return squarecut_size;
}
