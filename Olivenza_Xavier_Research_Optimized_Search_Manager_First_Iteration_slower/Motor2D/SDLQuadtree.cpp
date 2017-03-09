#include "SDLQuadtree.h"

// This is used for debug purpose, to draw subdivisions done
std::vector<AABB> QuadtreeAABBs;

// Insert new point in the quadtree
bool Quadtree::insert(iPoint newpoint)
{
	// If new point is not in the quadtree AABB, return
	if (!boundary.contains(&newpoint))
		return false;

	// If in this node there is space for the point, pushback it
	if (objects.size() < 4)
	{
		objects.push_back(newpoint);
		return true;
	}

	// Otherwise, subdivide and add the point to one of the new nodes
	if (children[0] == nullptr)
		subdivide();

	for (uint i = 0; i < 4; i++)
		if (children[i]->insert(newpoint))
			return true;

	return false;
}

// Subdivade this quadtree node into 4 smaller children
void Quadtree::subdivide()
{
	// Get subdivision size
	iPoint qSize = { (int)(boundary.RangeFromCentre.x * 0.5), (int)(boundary.RangeFromCentre.y * 0.5) };
	iPoint qCentre;

	//Calculate new AABB centre for each child
	qCentre = { boundary.centre.x - qSize.x, boundary.centre.y + qSize.y};
	children[0] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize)); // Tis line is for debug draw purpose

	qCentre = { boundary.centre.x + qSize.x, boundary.centre.y + qSize.y};
	children[1] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize)); // Tis line is for debug draw purpose

	qCentre = { boundary.centre.x - qSize.x, boundary.centre.y - qSize.y};
	children[2] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize)); // Tis line is for debug draw purpose

	qCentre = { boundary.centre.x + qSize.x, boundary.centre.y - qSize.y};
	children[3] = new Quadtree(AABB(qCentre, qSize));
	QuadtreeAABBs.push_back(AABB(qCentre, qSize)); // Tis line is for debug draw purpose
}

// Search in the quadtree and return the points in the AABB sent
std::vector< iPoint > Quadtree::queryRange(AABB* range)
{
	// Vector that will be filled with all the points of the quadtree contained in the AABB range
	std::vector< iPoint > pInRange;

	// If range is not in the quadtree, return
	if (!boundary.intersects(range))
		return pInRange;

	//See if the point is in range and pushback it to the returning vector
	if (!objects.empty())
		for (int i = 0; i < 4; i++)
			if (range->contains(&objects[i]))
				pInRange.push_back(objects[i]);

	//If there is no children, end
	if (children[0] == nullptr)
		return pInRange;

	// Otherwise, add the points from the children
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

// Destroys the quadtree
void Quadtree::Clear()
{
	for (uint i = 0; i < 4; i++)
		if (children[i] != nullptr)
		{
			delete children[i];
			children[i] = nullptr;
		}
}