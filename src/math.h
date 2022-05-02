#include <cmath>

double g_x = 0;
double g_y = 0;


int cplxiter( double ix, double iy)
{
	int maxiters = 1000;
	int i = 0;
	double threshold = 8.0;
	double x = g_x;
	double y = g_y;
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


