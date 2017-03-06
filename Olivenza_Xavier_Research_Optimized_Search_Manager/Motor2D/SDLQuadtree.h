#ifndef _SDL_QUADTREE_H_
#define _SDL_QUADTREE_H_

#include <vector>

#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

// Axis-Aligned Bounding Box
// This is the rectangles used by the quadtree to subdivide the space
struct AABB
{
	SDL_Rect aabb;
	iPoint centre;
	iPoint RangeFromCentre;

	// Creates the rectangle with its centre, half-wide and half-height
	// RangeFromCentre(half-wide, half-height)
	AABB(iPoint centre = { 0,0 }, iPoint RangeFromCentre = { 0,0 }): centre(centre), RangeFromCentre(RangeFromCentre)
	{
		aabb = { centre.x - RangeFromCentre.x, centre.y - RangeFromCentre.y, RangeFromCentre.x * 2, RangeFromCentre.y * 2 };
	}

	// See if the point is in the rectangle, including its boundaries. The code in 
	// SDL_PointInRect can be modified to adapt witch points are considered "in"
	bool contains(iPoint* a) const
	{
		SDL_Point point = { a->x,a->y };
		return SDL_PointInRect(&point, &aabb);
	}

	// See if this rectangle intersects with other
	bool intersects(const AABB* other) const
	{
		return SDL_HasIntersection(&aabb, &other->aabb);
	}
};

class Quadtree
{
private:
	// 4 children, the "magic number" 4 you will see is for that
	Quadtree* children[4];
	// 0 = nw child
	// 1 = ne child
	// 2 = sw child
	// 3 = se child

	// Axis-Aligned Bounding Box of the quadtree node
	AABB boundary;

	// Points in this quad tree node
	std::vector<iPoint> objects;

public:
	Quadtree(AABB& boundary)
	{
		this->boundary = boundary;
	}

	~Quadtree()
	{
		for (uint i = 0; i < 4; i++)
			delete children[i];
	}

	// Insert new point in the quadtree
	bool insert(iPoint newpoint);

	// Subdivade this quadtree node into 4 smaller children
	void subdivide();

	// Search in the quadtree and return the points in the AABB sent
	std::vector< iPoint > queryRange(AABB* range);

	// Destroys the quadtree
	void Clear();
};

// This is used for debug purpose, to draw subdivisions done
extern std::vector<AABB> QuadtreeAABBs;

#endif