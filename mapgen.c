#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "mapgen.h"


/*the following functions are exposed through the header and can help to navigate the array returned by getMap*/


int northOf(int target, int * map){
	int x = *(map + 400);
	if ((target - x) < 0){
		return 0;}
	else{
		return *(map + ((target - x)));
	}
}


int southOf(int target, int * map){
	int x = *(map + 400);
	int y = *(map + 401);
	if ((target + x) >= (x * y)){
		return 0;}
	else{
		return *(map + ((target + x)));
	}
}


int eastOf(int target, int * map){
	int x = *(map + 400);
	if ((target + 1) % x == 0){
		return 0;}
	else{
		return *(map + ((target + 1)));
	}
}


int westOf(int target, int * map){
	int x = *(map + 400);
	if ((target % x) == 0){
		return 0;}
	else{
		return *(map + ((target - 1)));
	}
}


/*the following functions are used in concert in the generation of the map layoud in the getMap function*/

static int numAt(int xcord, int ycord, int * map) {
	int x = *(map + 400);
	return *( map + ( (ycord - 1) * x ) + xcord - 1);
}//pass in x and y coordinates to get the value at that poisition. there is now an enum in the header: 0s lack a room, 1s are quotidian rooms/tunnels, 2s are major rooms, 3 has main objective and 4 is entrypoint



static int locOf(int xcord, int ycord, int * map) {
	int x = *(map + 400);
	return ( (ycord - 1) * x) + xcord - 1;
}//pass in x and y coordinates to get the poisition in the array



static int xof (int loc, int * map){
	int x = *(map + 400);
	if ( (loc + 1) % x == 0) {
		return x;
	} 
	else {
		return ( (loc + 1) - (((loc + 1) / x) * x));
	}
}//gets the row of the coordinate



static int yof (int loc, int * map){
	int x = *(map + 400);
	return (loc / x) + 1;
}



static bool xcheck(int start, int * map) {
	int x = *(map + 400);
	int y = *(map + 401);
	bool result = false;
	int vert = xof(start, map);
	int i = (yof(start, map) + 1);
	while ( (i < y) && (result == false) ) {
		if (*(map + (x * i) + vert - 1) == EMPTY) {
			i++;
		}
		else {
			result = true;
		}
	}
	return result;
}//scans down the x axis for anything interesting starting at the specific point, returns 1 for positive



static bool ycheck(int start, int * map) {
	int x = *(map + 400);
	bool result = false;
	int row = yof(start, map);
	int i = 0;
	while ( (i < x) && (result == false) ) {
		if (*(map + (row - 1) + i) == EMPTY) {
			i++;
		} 
		else {
			result = true;
		}	
	}
	return result;
} //scans across the y axis for any sort of interesting room



static int entryFind (int * map){
	int x = *(map + 400);
	int y = *(map + 401);
	for (int i = 0; i < (x * y); i++){
		if ( * (map + i) == ENTRYPOINT){
			return xof(i, map);
		}
	}
}//returns the column of the entrypoint




static int xOrient(int coord, int vertical, int * map){
	if (xof(coord, map) <= vertical) {
		return 1;
	}
	else {
		return -1;
		}

}//a function for approaching the column specified by returning either 1 or -1 for left/west or right/east movement. insert your current coordinate first, followed by destination column. used for x-axis movement relative to entrypoint.




static void oad(int room, int * map){
	int x = *(map + 400);
	if (xcheck(room, map) == true){
		if(*(map + room + x) == EMPTY){
			*(map + room + x) = TUNNEL;
			oad(room + x, map);
		}
		else return;
	}
	else if ( * ( map + room + xOrient(room, entryFind(map), map) ) == EMPTY){
		*( map + room + xOrient(room, entryFind(map), map ) ) = TUNNEL;
		oad ( room + xOrient(room, entryFind(map), map ), map );
	}
	else return;
}//looks for a room, inserting 1s along the way, moving over and then down as it goes




static void dao(int room, int * map) {
	int x = *(map + 400);
	int y = *(map + 401);
	if(yof(room, map) == y){
		if( * ( map + room + xOrient(room, entryFind(map), map) ) == EMPTY){
			*( map + room + xOrient(room, entryFind(map), map) ) = TUNNEL;
			dao( room + xOrient(room, entryFind(map), map), map );
		}
		else return;	
	}
	else if(*(map + room + x) != EMPTY){
		return;
	}
	else {
		*(map + room + x) = 1;
		dao(room + x, map);
	} 
}



static void rotate(int * map){
	int oldx = *(map + 400);
	int oldy = *(map + 401);
	int newx = *(map + 401);
	int newy = *(map + 400);
	int array[newy * newx];
	for (int i1 = 0; i1 < oldy; i1++){
		for(int i2 = 0; i2 < oldx; i2++){
			array[((i2 + 1) * newx) - (1 + i1)] = *(map + (i1 * oldx) +i2);	
		}
	}
	*(map + 400) = newx;
	*(map + 401) = newy;
	memcpy(map, array, sizeof(array));
}//rotates 45 degrees









int * getMap(int xdim, int ydim, int orient, int density){

int *worldmap = calloc(402, sizeof(int));

	*(worldmap + 400) = xdim; //dimensions are stored for use elsewhere
	*(worldmap + 401) = ydim; //

	srand(time(NULL));

	int majorRoom = (xdim * ydim) / density;

	for (int i = 0; i < majorRoom; i++){
		*( worldmap + ( rand() % (xdim * ydim) ) ) = MAJORROOM;
	} // sets some rooms to be major rooms

	*(worldmap + locOf(rand()%xdim, ydim, worldmap) + 1) = ENTRYPOINT; //sets entrypoint

	if (1){
		int objectiveQty = 0;
		while (objectiveQty < 1){
			int target = ( rand()%(xdim * ydim) );
			if ( * (worldmap + target) != ENTRYPOINT ){
				*(worldmap + target) = OBJECTIVE; //randomizes a location of a room for primary obejctive, eg chalice, if one is set. for now, every map has exactly one objective room
				objectiveQty++;
			}
		}
	}

	for (int i = 0; i < (xdim * ydim); i ++) {
		if (*(worldmap + i) >= 3) {
			if ( (rand() % 100) < 75) {
				oad(i, worldmap);
			}
			else{
				dao(i, worldmap);
			}
		}
	}//connects major rooms with tunnels

	for (int i = 0; i < orient; i++){
		rotate(worldmap);
	}//time to rotate to fit orientation

	return worldmap;

}

