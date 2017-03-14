#include "SDLQuadtree.h"

// This is used for debug purpose, to draw subdivisions done
std::vector<SDL_Rect> QuadtreeAABBs;

// Insert new point in the quadtree
bool AABB::Insert(iPoint* newpoint)
{
	// TODO 1: The new point is inside the quadtree AABB?

	// TODO 2: Add the point to the node if there is space for it (remember Max_Elements_in_Same_Node)

	// TODO 5: If there is not space, add the point to a subdivision

	return false;
}

// Subdivade this quadtree node into 4 smaller children
void AABB::subdivide()
{
	// TODO 3: Calculate the size and position of each of the 4 new nodes

	// TODO 4: Now we have all the positions and size of each number we 
	// can create each child with its AABB
	// Don't forget to set child root
}

int AABB::CollectCandidates(std::vector< iPoint* >& nodes, const SDL_Rect& r)
{
	uint ret = 0;

	// TODO 6: Check if range is in the quadtree AABB

	// TODO 7: See if the points of this node are in range, get them
	//(remember Max_Elements_in_Same_Node and ret to count how many points we check)

	// TODO 8: If the node don't hace children, we can end

	// TODO 9: If the noide has children, get them points

	return ret;
}