#include "utilities.h"

Vector3 Vector3::CrossProduct(const Vector3& u, const Vector3& v)
{
	return Vector3(
		(u.y * v.z) - (u.z * v.y),
		(u.z * v.x) - (u.x * v.z),
		(u.x * v.y) - (u.y * v.x));
}

float Vector3::DotProduct(const Vector3& u, const Vector3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float Vector3::Magnitude(const Vector3& u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

bool Vector3::operator==(const Vector3& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}
