#ifndef UTILITIES_H
#define UTILITIES_H

#include "qvector.h"
#include "Windows.h"

class Vector3
{
private:
	double x, y, z;

public:
	Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3(double n) : x(n), y(n), z(n) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	bool operator==(const Vector3& u) const;
	Vector3 operator-(const Vector3& u) const;
	Vector3 operator+(const Vector3& u) const;

	static float DotProduct(const Vector3& u, const Vector3& v);
	static Vector3 CrossProduct(const Vector3& u, const Vector3& v);
	static float Magnitude(const Vector3& u);

	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }


	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setZ(double z) { this->z = z; }
};

class Color
{
private:
	QVector<Vector3> colors;

public:
	void InitColors();
	Vector3 ConvertHexColor(const char* hex);
	Vector3 GetColor(int i) { return colors[i % colors.size()]; }
};

#endif // UTILITIES_H
