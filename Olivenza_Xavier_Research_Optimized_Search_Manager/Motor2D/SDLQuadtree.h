#ifndef _SDL_QUADTREE_H_
#define _SDL_QUADTREE_H_

#include <vector>

#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

struct AABB // Axis-Aligned Bounding Box
{
	SDL_Rect aabb;

	AABB(iPoint centre = { 0,0 }, iPoint RangeFromCentre = { 0,0 })
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

	bool insert(iPoint newpoint)
	{
		if (!boundary.contains(&newpoint))
			return false;

		if (objects.size() < 4)
		{
			objects.push_back(newpoint);
			return true;
		}

		if (children[0] == nullptr)
			subdivide();

		for (uint i = 0; i < 4; i++)
			if (children[i]->insert(newpoint))
				return true;

		return false;
	}

	void subdivide()
	{
		iPoint qSize = { (int)(boundary.aabb.w * 0.5), (int)(boundary.aabb.h * 0.5) };
		iPoint qCentre;

		qCentre = { boundary.aabb.x - qSize.x, boundary.aabb.y - qSize.y };
		children[0] = new Quadtree(AABB(qCentre, qSize));

		qCentre = { boundary.aabb.x + qSize.x, boundary.aabb.y - qSize.y };
		children[1] = new Quadtree(AABB(qCentre, qSize));

		qCentre = { boundary.aabb.x - qSize.x, boundary.aabb.y + qSize.y };
		children[2] = new Quadtree(AABB(qCentre, qSize));

		qCentre = { boundary.aabb.x + qSize.x, boundary.aabb.y + qSize.y };
		children[3] = new Quadtree(AABB(qCentre, qSize));
	}

	std::vector< iPoint > queryRange(AABB* range)
	{
		std::vector< iPoint > pInRange;

		if (!boundary.intersects(range))
			return pInRange;

		for (int i = 0; i < 4; i++)
			if (range->contains(&objects.at(i)))
				pInRange.push_back(objects.at(i));

		if (children[0] == nullptr)
			return pInRange;

		std::vector< iPoint > temp = children[0]->queryRange(range);
		pInRange.insert(pInRange.end(), temp.begin(), temp.end());

		temp = children[1]->queryRange(range);
		pInRange.insert(pInRange.end(), temp.begin(), temp.end());

		temp = children[2]->queryRange(range);
		pInRange.insert(pInRange.end(), temp.begin(), temp.end());

		temp = children[3]->queryRange(range);
		pInRange.insert(pInRange.end(), temp.begin(), temp.end());

		return pInRange;
	}

	void Clear()
	{
		for (uint i = 0; i < 4; i++)
			if (children[i] != nullptr)
			{
				delete children[i];
				children[i] = nullptr;
			}
	}
};

#endif