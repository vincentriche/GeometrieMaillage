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
	AABB() { minAABB = Vector3(-20.0, -20.0, 0.0f); maxAABB = Vector3(20.0, 20.0, 0.0f); }

	void SetMinAABB(Vector3 m) { minAABB = m; }
	void SetMaxAABB(Vector3 m) { maxAABB = m; }
	Vector3 GetMinAABB() { return minAABB; }
	Vector3 GetMaxAABB() { return maxAABB; }
};

#endif // BOUNDINGBOX_H
