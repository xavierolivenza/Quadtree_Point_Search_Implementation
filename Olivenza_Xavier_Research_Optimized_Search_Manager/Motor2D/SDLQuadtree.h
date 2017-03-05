#ifndef _SDL_QUADTREE_H_
#define _SDL_QUADTREE_H_

#include <vector>

#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

struct AABB // Axis-Aligned Bounding Box
{
	SDL_Rect aabb;
	iPoint centre;
	iPoint RangeFromCentre;

	AABB(iPoint centre = { 0,0 }, iPoint RangeFromCentre = { 0,0 }): centre(centre), RangeFromCentre(RangeFromCentre)
	{
		aabb = { centre.x - RangeFromCentre.x, centre.y - RangeFromCentre.y, RangeFromCentre.x * 2, RangeFromCentre.y * 2 };
	}

	bool contains(iPoint* a) const
	{
		SDL_Point point = { a->x,a->y };
		return SDL_PointInRect(&point, &aabb);
	}

	bool intersects(const AABB* other) const
	{
		return SDL_HasIntersection(&aabb, &other->aabb);
	}
};

class Quadtree
{
private:
	//4 children, the "magic number" 4 you will see is for that
	Quadtree* children[4];
	//0 = nw child
	//1 = ne child
	//2 = sw child
	//3 = se child

	AABB boundary;

	std::vector<iPoint> objects;

public:
	Quadtree()
	{

	}

	Quadtree(AABB& boundary)
	{
		this->boundary = boundary;
	}

	~Quadtree()
	{
		for (uint i = 0; i < 4; i++)
			delete children[i];
	}

	bool insert(iPoint newpoint);

	void subdivide();

	std::vector< iPoint > queryRange(AABB* range);

	void Clear();
};

extern std::vector<AABB> QuadtreeAABBs;

#endif