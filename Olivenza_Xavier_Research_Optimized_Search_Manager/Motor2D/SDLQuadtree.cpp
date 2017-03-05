#include "SDLQuadtree.h"

std::vector<AABB> QuadtreeAABBs;

bool Quadtree::insert(iPoint newpoint)
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

void Quadtree::subdivide()
{
	iPoint qSize = { (int)(boundary.RangeFromCentre.x * 0.5), (int)(boundary.RangeFromCentre.y * 0.5) };
	iPoint qCentre;

	qCentre = { boundary.centre.x - qSize.x, boundary.centre.y + qSize.y };
	children[0] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize));

	qCentre = { boundary.centre.x + qSize.x, boundary.centre.y + qSize.y };
	children[1] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize));

	qCentre = { boundary.centre.x - qSize.x, boundary.centre.y - qSize.y };
	children[2] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize));

	qCentre = { boundary.centre.x + qSize.x, boundary.centre.y - qSize.y };
	children[3] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize));
}

std::vector< iPoint > Quadtree::queryRange(AABB* range)
{
	std::vector< iPoint > pInRange;

	if (!boundary.intersects(range))
		return pInRange;

	if (!objects.empty())
		for (int i = 0; i < 4; i++)
			if (range->contains(&objects[i]))
				pInRange.push_back(objects[i]);

	if (children[0] == nullptr)
		return pInRange;

	std::vector< iPoint > temp;
	temp = children[0]->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = children[1]->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = children[2]->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = children[3]->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}

void Quadtree::Clear()
{
	for (uint i = 0; i < 4; i++)
		if (children[i] != nullptr)
		{
			delete children[i];
			children[i] = nullptr;
		}
}