#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "utilities.h"

// Axis-Aligned Bounding Box
class AABB
{
private:
	Vector3 minAABB;
	Vector3 maxAABB;

public:
	AABB() { }

	Vector3& GetMinAABB() { return minAABB; }
	Vector3& GetMaxAABB() { return maxAABB; }
};

#endif // BOUNDINGBOX_H
