#pragma once
struct CMeColor
{
	CMeColor()
		: red(0)
		, green(0)
		, blue(0)
		, alpha(1.0)
	{
	}
	CMeColor(double r, double g, double b, double a = 1.0)
		: red(r)
		, green(g)
		, blue(b)
		, alpha(a)
	{
	}
	void setColor(double r, double g, double b, double a = 1.0)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
	double getRed(){ return min(max(red, 0.), 1.);}
	double getGreen(){ return min(max(green, 0.), 1.);}
	double getBlue(){ return min(max(blue, 0.), 1.);}
	double getAlpha(){ return min(max(alpha, 0.), 1.);}
private:
	double red;
	double green;
	double blue;
	double alpha;
};
struct CMeDPoint
{
	CMeDPoint()
		: x(0.0)
		, y(0.0)
	{}
	CMeDPoint(double xx, double yy)
		: x(xx)
		, y(yy)
	{}
	CMeDPoint(const CPoint &pt)
		: x(static_cast<double>(pt.x))
		, y(static_cast<double>(pt.y))
	{}

	CPoint Point() const
	{
		CPoint pt;
		pt.x = static_cast<int>(x);
		pt.y = static_cast<int>(y);

		return pt;
	}

	bool operator == (const CMeDPoint &other) const
	{
		return (x == other.x && y == other.y);
	}

	CMeDPoint operator - (const CMeDPoint &other) const
	{
		CMeDPoint pt(x-other.x, y-other.y);
		return pt;
	}
	CMeDPoint operator -= (const CMeDPoint &other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	CMeDPoint operator + (const CMeDPoint &other) const
	{
		CMeDPoint pt(x+other.x, y+other.y);
		return pt;
	}
	CMeDPoint operator += (const CMeDPoint &other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	CMeDPoint operator * (const double lfPar) const
	{
		CMeDPoint pt(x*lfPar, y*lfPar);
		return pt;
	}
	CMeDPoint operator / (const double lfPar) const
	{
		CMeDPoint pt(x/lfPar, y/lfPar);
		return pt;
	}
	CMeDPoint operator *= (const double lfPar)
	{
		x *= lfPar;
		y *= lfPar;
		return *this;
	}
	CMeDPoint operator /= (const double lfPar)
	{
		x /= lfPar;
		y /= lfPar;
		return *this;
	}
	operator CPoint() { return CPoint(int(x), int(y));}
	double x;
	double y;
};