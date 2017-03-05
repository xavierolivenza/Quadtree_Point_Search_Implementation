#include <ctime>

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"

static const uint Max_Points = 200;

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//-------------------------------------------------------------------------------------//
	//----------------------------------Quadtree Creation----------------------------------//
	//-------------------------------------------------------------------------------------//
	
	QuadtreeCreationTime.Start();

	Quadtree_area = new AABB({ 640,360 }, { 500,300 });
	Quadtree_area_search = new AABB({ 500,400 }, { 100,200 });
	Point_quadtree = new Quadtree(*Quadtree_area);

	//Generate random tree points
	srand(time(NULL));
	int x = 0;
	int y = 0;
	//Uncomment it for negative numbers
	//bool xsign = false;
	//bool ysign = false;

	uint accepted_points = 0;
	bool point_accepted = false;

	for (int i = 0; i < Max_Points; i++)
	{
		//Generates 200 random points between 0-150
		x = rand() % Quadtree_area->aabb.w;
		y = rand() % Quadtree_area->aabb.h;
		x += Quadtree_area->aabb.x;
		y += Quadtree_area->aabb.y;
		//Uncomment it for negative numbers
		//xsign = rand() % 2;
		//ysign = rand() % 2;
		//if (xsign) x *= -1;
		//if (ysign) y *= -1;

		//Insert point to tree
		point_accepted = Point_quadtree->insert({ x,y });

		//Debug
		if (point_accepted)
		{
			quadtree_points.push_back({ x,y });
			accepted_points++;
		}
	}
	LOG("Quadtree Creation Time = %f ms", QuadtreeCreationTime.ReadMs());
	LOG("Accepted Points = %i/%i", accepted_points, Max_Points);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update()
{
	//-------------------------------------------------------------------------------------//
	//-----------------------------------Quadtree Search-----------------------------------//
	//-------------------------------------------------------------------------------------//

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		//Normal search
		NormalSearchTime.Start();
		uint normalpointscount = 0;
		bool inrect = false;
		for (std::vector<iPoint>::iterator item = quadtree_points.begin(); item < quadtree_points.end(); item++)
		{
			SDL_Point point = { (*item).x,(*item).y };
			inrect = SDL_PointInRect(&point, &Quadtree_area_search->aabb);
			if (inrect)
				normalpointscount++;
		}
		LOG("Normal Search Time = %f ms", NormalSearchTime.ReadMs());
		LOG("Normal Points in Range = %i", normalpointscount);

		//Quadtree search
		QuadtreeSearchTime.Start();
		std::vector< iPoint > Points_in_range = Point_quadtree->queryRange(Quadtree_area_search);
		LOG("Quadtree Search Time = %f ms", QuadtreeSearchTime.ReadMs());
		LOG("Quadtree Points in Range = %i", Points_in_range.size());
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Point_quadtree->Clear();
		quadtree_points.clear();

		//Generate random tree points
		srand(time(NULL));
		int x = 0;
		int y = 0;
		//Uncomment it for negative numbers
		//bool xsign = false;
		//bool ysign = false;

		bool point_accepted = false;

		for (int i = 0; i < Max_Points; i++)
		{
			//Generates 150 random points between 0-150
			x = rand() % Quadtree_area->aabb.w;
			y = rand() % Quadtree_area->aabb.h;
			x += Quadtree_area->aabb.x;
			y += Quadtree_area->aabb.y;
			//Uncomment it for negative numbers
			//xsign = rand() % 2;
			//ysign = rand() % 2;
			//if (xsign) x *= -1;
			//if (ysign) y *= -1;

			//Insert point to tree
			point_accepted = Point_quadtree->insert({ x,y });

			//Debug
			if (point_accepted)
				quadtree_points.push_back({ x,y });
		}
		LOG("Quadtree Cleared");
	}

	//-------------------------------------------------------------------------------------//
	//---------------------------------Quadtree Debug Draw---------------------------------//
	//-------------------------------------------------------------------------------------//

	//Root
	App->render->DrawQuad(Quadtree_area->aabb, 255, 255, 255, 255, false, false);

	//Search Area
	App->render->DrawQuad(Quadtree_area_search->aabb, 255, 255, 255, 255, false, false);
	
	//Points
	for (std::vector<iPoint>::iterator item = quadtree_points.begin(); item < quadtree_points.end(); item++)
		App->render->DrawCircle((*item).x, (*item).y, 1, 255, 0, 0, 255, false);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	delete Quadtree_area;
	delete Quadtree_area_search;
	delete Point_quadtree;

	return true;
}