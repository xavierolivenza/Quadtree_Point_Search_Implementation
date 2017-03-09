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

//Max points in the quadtree
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

	Quadtree_area = { 384,104,512,512 };
	//Quadtree_area = { 100,50,550,600 };
	Point_quadtree = new Quadtree(Quadtree_area);
	Quadtree_area_search = { 500,200,100,200 };

	GenerateRandomPointsAndPushToQuadtree();

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
		// Normal search
		NormalSearchTime.Start();
		normalpointscount = 0;
		bool inrect = false;
		std::vector< iPoint > Points_in_range_normal_search;
		for (std::vector<iPoint>::iterator item = quadtree_points.begin(); item < quadtree_points.end(); item++)
		{
			SDL_Point point = { (*item).x,(*item).y };
			inrect = SDL_PointInRect(&point, &Quadtree_area_search);
			if (inrect)
			{
				Points_in_range_normal_search.push_back({ (*item).x,(*item).y });
				normalpointscount++;
			}
		}
		normaltime = NormalSearchTime.ReadMs();
		LOG("Normal Search Time = %f ms", normaltime);
		LOG("Normal Points in Range = %i", normalpointscount);

		// Quadtree search
		Points_in_range_quadtree_search.clear();
		QuadtreeSearchTime.Start();
		Point_quadtree->CollectCandidates(Points_in_range_quadtree_search, Quadtree_area_search);
		quadtreetime = QuadtreeSearchTime.ReadMs();
		LOG("Quadtree Search Time = %f ms", quadtreetime);
		LOG("Quadtree Points in Range = %i", Points_in_range_quadtree_search.size());
	}

	// Generate a new quadtree
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Point_quadtree->Clear();
		quadtree_points.clear();
		QuadtreeAABBs.clear();

		Point_quadtree = new Quadtree(Quadtree_area);

		GenerateRandomPointsAndPushToQuadtree();

		LOG("Quadtree Cleared");
	}

	// Show all quadtree subdivions
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		seemesh = !seemesh;

	//Shows quadtree subdiviones step by step, use Q,A keys
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		seemeshstep = !seemeshstep;

	// Add 1 more step to the subdiviones step by step debug draw
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		seemeshstepnum += 1;
		if (seemeshstepnum > QuadtreeAABBs.size())
			seemeshstepnum = QuadtreeAABBs.size();
	}

	// Remove 1 step from the subdiviones step by step debug draw
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		seemeshstepnum -= 1;
		if(seemeshstepnum < 0)
			seemeshstepnum = 0;
	}
	
	//-------------------------------------------------------------------------------------//
	//---------------------------------Quadtree Debug Draw---------------------------------//
	//-------------------------------------------------------------------------------------//

	//Root
	App->render->DrawQuad(Quadtree_area, 255, 255, 255, 255, false, false);

	//Search Area
	App->render->DrawQuad(Quadtree_area_search, 255, 255, 255, 255, false, false);

	//Quadtreee AABBs
	//F1 mode all subdiviones
	if(seemesh)
		for (std::vector<SDL_Rect>::iterator item = QuadtreeAABBs.begin(); item < QuadtreeAABBs.end(); item++)
			App->render->DrawQuad((*item), 0, 255, 0, 255, false, false);

	//Quadtreee AABBs step
	//F2 mode subdiviones step by step
	uint i = 0;
	if (seemeshstep)
		for (std::vector<SDL_Rect>::iterator item = QuadtreeAABBs.begin(); item < QuadtreeAABBs.end(); item++, i++)
		{
			if (i >= seemeshstepnum)
				break;
			if (i < seemeshstepnum - 1)
				App->render->DrawQuad((*item), 0, 255, 0, 255, false, false);
			else
				App->render->DrawQuad((*item), 0, 0, 255, 255, false, false); //Print the lst one in blue
		}
	
	//Points
	for (std::vector<iPoint>::iterator item = quadtree_points.begin(); item < quadtree_points.end(); item++)
		App->render->DrawCircle((*item).x, (*item).y, 1, 255, 0, 0, 255, false);

	//Set title
	static char title[256];
	sprintf_s(title, 256, "Normal Search Time: %f ms, Points in Range = %i | Quadtree Search Time: %f ms, Points in Range = %i | Quadtree AABBs = %i | Accepted points %i/%i | Mesh step = %i",
		normaltime, normalpointscount, quadtreetime, Points_in_range_quadtree_search.size(), QuadtreeAABBs.size(), accepted_points, Max_Points, seemeshstepnum);
	App->win->SetTitle(title);

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

	delete Point_quadtree;

	return true;
}

void j1Scene::GenerateRandomPointsAndPushToQuadtree()
{
	//Generate random tree points
	srand(time(NULL));
	int x = 0;
	int y = 0;
	//Uncomment it for negative numbers
	//bool xsign = false;
	//bool ysign = false;

	accepted_points = 0;
	bool point_accepted = false;

	for (int i = 0; i < Max_Points; i++)
	{
		//Generates random points between
		x = rand() % Quadtree_area.w;
		y = rand() % Quadtree_area.h;
		x += Quadtree_area.x;
		y += Quadtree_area.y;
		//Uncomment it for negative numbers
		//xsign = rand() % 2;
		//ysign = rand() % 2;
		//if (xsign) x *= -1;
		//if (ysign) y *= -1;

		//Insert point to tree
		point_accepted = Point_quadtree->Insert(&iPoint( x,y ));

		//Debug
		if (point_accepted)
		{
			quadtree_points.push_back({ x,y });
			accepted_points++;
		}
	}
}