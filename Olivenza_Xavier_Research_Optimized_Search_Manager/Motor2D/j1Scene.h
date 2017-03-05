#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include <vector>

#include "j1Module.h"
#include "SDLQuadtree.h"
#include "j1PerfTimer.h"

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	//Quadtree
	AABB* Quadtree_area = nullptr;
	AABB* Quadtree_area_search = nullptr;
	Quadtree* Point_quadtree = nullptr;

	//Quadtree debug
	std::vector<iPoint> quadtree_points;

	//Timers
	j1PerfTimer QuadtreeCreationTime;
	j1PerfTimer QuadtreeSearchTime;
	j1PerfTimer NormalSearchTime;
};

#endif // __j1SCENE_H__