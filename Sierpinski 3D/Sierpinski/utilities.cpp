#include "utilities.h"

Vector3 Vector3::CrossProduct(const Vector3& u)
{
	return Vector3(
		(y * u.z) - (z * u.y),
		(z * u.x) - (x * u.z),
		(x * u.y) - (y * u.x));
}

float Vector3::DotProduct(const Vector3& u)
{
	return x * u.x + y * u.y + z * u.z;
}

float Vector3::Magnitude()
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::operator-(const Vector3& a)
{
	return Vector3(x - a.x, y - a.y, z - a.z);
}

Vector3 Vector3::operator+(const Vector3& a)
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}
