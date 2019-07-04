#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "base.h"
#include "circle.h"
#include "square.h"
#include "squarecut.h"

using namespace::std;

int main()
{
	Circle c;
	Square sq;
	Squarecut sqr_cut;
    bool flag = true;

    std::cout << "> Type help to see available comands\n";
    std::cout << "> ";
    while (1 == 1)
    {
        string s;
        getline(std::cin, s);

        if (s == "help")
            std::cout << "\n  circle\n" << "  square\n" << "  print_circle\n" << "  print_square\n" << "  square_cut\n" << "  print_square_cut\n" << "  quit\n\n";

        else if (s == "circle")
        {
        	flag = false;
        	double radius;
            cout << "> Enter radius:\n>  ";
            cin >> radius;
            Circle td(0,0,radius);
            c = td;
        }

        else if (s == "square")
        {
            flag = false;
        	long int len;
        	long int wid;
            cout << "> Enter length:\n>  ";
            cin >> len;
            cout << "> Enter width:\n>  ";
            cin >> wid;
            Square td(len,wid);
            sq = td;
        }

				else if (s == "square_cut")
				{
						flag = false;
						long int len;
	        	long int wid;
						double radius;
						cout << "> Enter length:\n>  ";
            cin >> len;
            cout << "> Enter width:\n>  ";
            cin >> wid;
						cout << "> Enter radius:\n>  ";
            cin >> radius;
						Squarecut sqr_c(wid, len, radius);
						sqr_cut = sqr_c;
				}

        else if (s == "print_circle")
            cout << c.get_size() << "\n\n> ";

        else if (s == "print_square")
            cout << sq.get_size() << "\n\n> ";

				else if (s == "print_square_cut")
						cout << sqr_cut.get_squarecut_size() << "\n\n";

        else if (s == "quit")
            break;

        else if (flag)
            cout << "No such command.\n\n> ";

        else
        {
            flag = true;
            cout << "\n> ";
        }

        cout << "> ";
    }
	cout << "> Circle\n";
	cout << "> Coordinate of the x axis of the circle's center " << c.getCenterX() << "\n";
	cout << "\n> Coordinate of the y axis of the circle's center " << c.getCenterY() << "\n";
	cout << "\n> The area of a circle with a " << c.getRadius() << " radius is " << c.get_size();
	cout << "\n\n> Rectangle\n";
	cout << "> The area of a rectangle with a width of " << sq.getWidth() << " and a " << sq.getLength() << " of length is " << sq.get_size() << endl;

	return 0;

}
