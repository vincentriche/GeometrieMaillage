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
	AABB(){ minAABB = 20; maxAABB = 20; }

	void SetMinAABB(Vector3 m){ minAABB = m; }
	void SetMaxAABB(Vector3 m){ maxAABB = m; }
	Vector3 GetMinAABB(){ return minAABB; }
	Vector3 GetMaxAABB(){ return maxAABB; }
};

#endif // BOUNDINGBOX_H
