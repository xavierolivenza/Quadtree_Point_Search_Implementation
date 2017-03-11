#include "SDLQuadtree.h"

// This is used for debug purpose, to draw subdivisions done
std::vector<SDL_Rect> QuadtreeAABBs;

// Insert new point in the quadtree
bool AABB::Insert(iPoint* newpoint)
{
	// TODO 1: If new point is not in the quadtree AABB, return

	// TODO 2: If in this node there is space for the point, pushback it (remember Max_Elements_in_Same_Node)

	// TODO 5: Otherwise, subdivide and add the point to one of the new nodes

	return false;
}

// Subdivade this quadtree node into 4 smaller children
void AABB::subdivide()
{
	// TODO 3: Calculate the size and position of each of the 4 new nodes

	// TODO 4: Now we have all the positions and size of each number we 
	// can create each child with its AABB
	// ! Don't forget to set child root!
}

int AABB::CollectCandidates(std::vector< iPoint* >& nodes, const SDL_Rect& r)
{
	uint ret = 1;

	// TODO 6: If range is not in the quadtree, return

	// TODO 7: See if the points of this node are in range and pushback them to the vector (remember Max_Elements_in_Same_Node)

	// TODO 8: If there is no children, end

	// TODO 9: Otherwise, add the points from the children (Recursive)

	return ret;
}