#include <iostream>
#include "circle.h"
#include "base.h"

#define pi_num 3.141592653

using namespace::std;

Circle::Circle() { //default
	setCenterX(0);
	setCenterY(0);
	setRadius(0.0);
}

Circle::Circle(long int circle_x, long int circle_y, double circle_radius) : BasicShape() { //basic constructor
	setCenterX(circle_x);
	setCenterY(circle_y);
	setRadius(circle_radius);
}

Circle::~Circle() { //destructor
	//cout << "D E S T R O Y E D";
}

void Circle::setCenterX(long int circle_x) //X-axis
{
		if (circle_x < 0)
		{
				cerr << "\n>> The center can not be a negative number! >>\n";
				exit(1);
		}
		else
				centerX = circle_x;
}

void Circle::setCenterY(long int circle_y) //Y-axis
{
		if (circle_y < 0)
		{
				cerr << "\n>> The center can not be a negative number! >>";
				exit(1);
		}
		else
				centerY = circle_y;
}

void Circle::setRadius(double circle_radius) //radius
{
		if (circle_radius < 0)
		{
				cerr << "The radius can not be a negative number.";
				exit(1);
		}
		else
				radius = circle_radius;
}

long int Circle::getCenterX() const
{
		return centerX;
}

long int Circle::getCenterY() const
{
		return centerY;
}

double Circle::getRadius() const
{
		return radius;

}

double Circle::get_size()
{
		double area;
		area = pi_num * radius * radius;
		return area;
}
