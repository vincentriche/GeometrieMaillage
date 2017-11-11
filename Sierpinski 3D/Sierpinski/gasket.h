#ifndef GASKET_H
#define GASKET_H

struct point {
	float x, y, z;
};

class Gasket
{
public:
	Gasket();

	void draw();

private:
	point * points;
};

#endif // GASKET_H
