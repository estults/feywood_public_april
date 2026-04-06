#ifndef FEYWOOD_MAPGEN_H
#define FEYWOOD_MAPGEN_H

int * getMap(int xdim, int ydim, int orient, int density); //inset x dimension, followed by y dimension, followed by 
							   //rotation for the purposes of entrypoint, followed by 
							   //density. 0 is rotate clockwise no times, so entry point 
							   //is from the South, 1 is West, 2 is North, 3 is East. 
							   //lower value is higher density, and vice versa. 
							   //DO NO EXCEED X OR Y DIMENSIONS OF 20!!! 20 x 20 is BIGGEST MAP!

int northOf(int target, int * map);

int southOf(int target, int * map);

int eastOf(int target, int * map);

int westOf(int target, int * map);//the above four functions recieve a pointer to specifically an array of chars 
				   //returned by getMap as well as a point on the array within the bounds of the 
				   //matrix, and it gives you back the adjacent value.

enum roomtype {
EMPTY = 0,
TUNNEL = 1,
ENTRYPOINT = 2,
OBJECTIVE = 3,
MAJORROOM = 4,
};


#endif

