#include "utilities.h"
#include <atlbase.h>

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

void Color::InitColors()
{
	colors.push_back(ConvertHexColor("00FFFF"));
	colors.push_back(ConvertHexColor("7FFFD4"));
	colors.push_back(ConvertHexColor("0000FF"));
	colors.push_back(ConvertHexColor("8A2BE2"));
	colors.push_back(ConvertHexColor("A52A2A"));
	colors.push_back(ConvertHexColor("DEB887"));
	colors.push_back(ConvertHexColor("5F9EA0"));
	colors.push_back(ConvertHexColor("7FFF00"));
	colors.push_back(ConvertHexColor("D2691E"));
	colors.push_back(ConvertHexColor("FF7F50"));
	colors.push_back(ConvertHexColor("6495ED"));
	colors.push_back(ConvertHexColor("DC143C"));
	colors.push_back(ConvertHexColor("00FFFF"));
	colors.push_back(ConvertHexColor("00008B"));
	colors.push_back(ConvertHexColor("008B8B"));
	colors.push_back(ConvertHexColor("B8860B"));
	colors.push_back(ConvertHexColor("A9A9A9"));
	colors.push_back(ConvertHexColor("006400"));
	colors.push_back(ConvertHexColor("BDB76B"));
	colors.push_back(ConvertHexColor("556B2F"));
	colors.push_back(ConvertHexColor("FF8C00"));
	colors.push_back(ConvertHexColor("9932CC"));
	colors.push_back(ConvertHexColor("8B0000"));
	colors.push_back(ConvertHexColor("E9967A"));
	colors.push_back(ConvertHexColor("8DBC8F"));
	colors.push_back(ConvertHexColor("483D8B"));
}

Vector3 Color::ConvertHexColor(const char* hex)
{
	int r, g, b;
	sscanf(hex, "%02x%02x%02x", &r, &g, &b);
	return Vector3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}