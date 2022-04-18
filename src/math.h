#include <cmath>

double g_x = 0;
double g_y = 0;


int cplxiter( double x, double y)
{
	int maxiters = 1000;
	int i = 0;
	double threshold = 4.0;
	double ix = g_x;
	double iy = g_y;
	double it;
	while( ix*ix + iy*iy < threshold*threshold && ++i<maxiters)
	{
		ix += x;
		iy += y;
		it = ix*ix - iy*iy;
		iy = 2*ix*iy;
		ix = it;
	}

	return i;
}

class Point //this is very inelegant
{
public:
	double x;
	double y;

	Point(){}

	Point(double X, double Y)
	{
		x = X;
		y = Y;
	}

	double length()
	{
		return sqrt(x*x + y*y);
	}

	void normalize()
	{
		if(length()!= 0.0)
		{
			x /= length();
			y /= length();
		}
	}
};


