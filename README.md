# Optimized search algorithm research

[This project GitHub Pages](https://xavierolivenza.github.io/Quadtree_Point_Search_Implementation/)

This project is a research about how to optimize search algorithms. 
After searching for a lot of information, I decided to try the quadtree, in the repository I have generated two codes, the one named "Olivenza_Xavier_Research_Optimized_Search_Manager_first_Iteration_slower" is my first attempt, which is slow, but then I generate the one named "Olivenza_Xavier_Research_Optimized_Search_Manager_second_Iteration_faster" which really works and is faster (if you are looking for some quadtree to implement take the one from this second project). There are also two PDFs with theoretical information of my search for information (one in English and the other in Spanish), with some help links, but to understand the code, the better you enter the project and look at the multiple code comments of the SDLQuadtree.h / .cpp files (they are named like this because I use SDL_Rect and some SDL functions) and j1Scene.h / .cpp. In the first files are the implementation of quadtree, and in the seconds are where I use it.

## Usage
###How to use the Quadtree

Look in the Olivenza_Xavier_Research_Optimized_Search_Manager_ENG or Olivenza_Xavier_Research_Optimized_Search_Manager_ESP file. In the section on the last but one page, How does my quadtree implementation work?/Cómo funciona mi implementación de quadtree, explains how to use the SDLQuadtree.h / .cpp files in another project, without worrying about what is inside these files.
For more information about the SDLQuadtree.h / .cpp files, enter them, they are full of comments.

###Debug
R  ➣ Regenerates a new quadtree with new points.

S  ➣ Search, in this project, brute force and quadtree search will be performed.

F1 ➣ See the whole generated quadtree.

F2 ➣ Activate the "see quadtree steps mode". To start seeing something, press Q.

Q  ➣ In "see quadtree steps mode", add one step.

A  ➣ In "see quadtree steps mode", remove one step.

CITM Terrassa 2016-2017