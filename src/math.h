#include <cmath>

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
