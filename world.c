#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "world.h"
#include "items.h"
#include "mapgen.h"


static void int buildFixedLevel(struct room * level, int offset){

	strcpy((level + offset + 0)->descr, "You are in the entrance to the Hero's Keep, and to the North of you is a gate \
through which you can see a courtyard. To your South is a pathway into dense, magical woodlands!");
	(level + offset + 0)->northCon = offset + 1;

	strcpy((level + offset + 1)->descr, "This is the central courtyard of the Hero's Keep. To your South is the gateway\
out of the Keep, and to your East and West are grand doors into flanking halls.");
	(level + offset + 1)->southCon = offset + 0;
	(level + offset + 1)->eastCon = offset + 2;
	(level + offset + 1)->westCon = offset + 3;

	strcpy((level + offset + 2)->descr, "You are in the hall of arms, and to your West is a door to the courtyard, while\
to your North is a stairway down into the dungeon.");
	(level + offset + 2)->westCon = offset + 1;
	(level + offset + 2)->northCon = offset + 4;
	(level + offset + 2)->items[0] = idOf("sword");
	(level + offset + 2)->items[1] = idOf("axe");

	strcpy((level + offset + 4)->descr, "You are in the library, and to your East is a door to the courtyard, while to\
your North is a stairway down into the dungeon.");
	(level + offset + 3)->eastCon = offset + 1;
	(level + offset + 3)->northCon = offset + 4;
	(level + offset + 3)->items[0] = idOf("orb");

	strcpy((level + offset + 4)->descr, "You are in the dungeon! Scary! to your East and West are stairways back up to\
the ground floor.");
	(level + offset + 4)->eastCon = offset + 2;
	(level + offset + 4)->westCon = offset + 3;

	return offset + 5;
}

/* I am currently in the process of trying to rebvild this function into a system that pulls from a new "rooms.c" that 
 * has templates for zones and can simply be called with a pointer, an offset, an an entrypoint, and it generates the
 *  rooms in based on the templates. alas, it is not yet finished. when it is, the dungeon below the hero's keep will
 *  be vast and spooky! it will also be different every time! */


static void int buildDynamicLevel(struct room * level, int entry, int orient, int xdim, int ydim, int den, int offset){

	int *feywood = getMap(xdim, ydim, orient, den);// generates a map layout from mapgen that is organized as a matrix of ints
						       // stored on an array with the last two ints storing the x and y dimensions of 
						       // array orientation n/s/e/w orientation must be brought into alignment through 
						       // calling this function, and the rotation so the entrypoint is the right location
						       // is achieved internal to the getMap function.
	int x = *(feywood + 400);
	int y = *(feywood + 401);

	srand(time(NULL));

	for(int i = 0; i < (x * y); i++){
		if(*(feywood + i) == TUNNEL){
			int tunnelChoice = (rand()%7);
			switch(tunnelChoice){
			case 0:
				strcpy((level + i + offset)->descr, "You are on a wooded path amongst blue flowers.");
				break;
			case 1: 
				strcpy((level + i + offset)->descr, "You are on a wooded path amongst purple flowers.");
				break;
			case 2:
				strcpy((level + i + offset)->descr, "You are on a wooded path amongst red flowers.");
				break;
				
			case 3:
				strcpy((level + i + offset)->descr, "You are on a path that passes under dense brush.");
				break;
			case 4:
				strcpy((level + i + offset)->descr, "You are on a path under towering pine trees.");
				break;
			case 5:
				strcpy((level + i + offset)->descr, "You are on a wooded path surrounded by brambles and rocks.");
				break;
			case 6:
				strcpy((level + i + offset)->descr, "You are on a path cut through the ground amongst roots.");
				break;
			}
		}

		if(*(feywood + i) == ENTRYPOINT){
			(level + i + offset)->northCon = entry;
			(level + entry)->southCon = (i + offset);
			strcpy((level + i + offset)->descr, "You stand at the entrance of the Feywood. To your North lies a castle.");
		}

		if(*(feywood + i) == OBJECTIVE){
			strcpy((level + i + offset)->descr, "Before you is a tranquil clearing basked in golden radiance. In the middle is\
the altar of the Holy Grail.");
			(level + i + offset)->items[0] = idOf("chalice");
		}

		if(*(feywood + i) == MAJORROOM){
			int roomChoice = (rand()%6);
			switch(roomChoice){		
			case 0:
				strcpy((level + i + offset)->descr, "You have found a mysterious pool amongst the trees.");
				break;
			case 1:
				strcpy((level + i + offset)->descr, "In the middle of the woods you find a confluence of vines\
and roots, and at its center the decayed corpse of a knight.");
				(level + i + offset)->items[0] = idOf("sword");
				break;
			case 2:
				strcpy((level + i + offset)->descr, "You come across a stump in the middle of the forest alongside\
a partially processed tree trunk. Who would be felling trees here, and why did they stop?");
				(level + i + offset)->items[0] = idOf("axe");
				break;
			case 3:
				strcpy((level + i + offset)->descr, "You find yourself in a glade that sends a chill down your\
spine--perhaps it is haunted by resentful spirits.");
				break;
			case 4:
				strcpy((level + i + offset)->descr, "You have found an old campsite. The campers have vanished\
without a trace, leaving all their supplies and possessions here.");
				(level + i + offset)->items[0] = idOf("ruby");
				(level + i + offset)->items[1] = idOf("dagger");
				break;
			case 5:
				strcpy((level + i + offset)->descr, "You find a section of forest filled with stone cairns built\
here for no discernable use.");
				break;
			}
		}

	}//assigns contents to all rooms

	for(int i = 0; i < (x * y); i++){
		char doorDesc[40] = " ";
		int routes = 0;
		if(westOf(i, feywood) != EMPTY){
			(level + i + offset)->westCon = (i + offset -1);
			strcat(doorDesc, "West");
			routes++;
		}
		if(eastOf(i, feywood) != EMPTY){
			(level + i + offset)->eastCon = (i + offset + 1); 
			if(routes == 1){
				char temp[20] = " East, and";
				strcat(temp, doorDesc);
				strcpy(doorDesc, temp);
			}
			if(routes == 0){
				strcat(doorDesc, "East");
			}
				routes++;
		}
		if(southOf(i, feywood) != EMPTY){
			(level + i + offset)->southCon = (i + offset + x);
			if(routes == 1){
				char temp[40] = " South, and";
				strcat(temp, doorDesc);
				strcpy(doorDesc, temp);
			}
			if(routes > 1){
				char temp[40] = " South,";
				strcat(temp, doorDesc);
				strcpy(doorDesc, temp);
			}
			if(routes == 0){
				strcat(doorDesc, "South");
			}
			routes++;
		}
		if(northOf(i, feywood) != EMPTY){
			(level + i + offset)->northCon = (i + offset - x);
			if(routes == 1){
				char temp[60] = " North, and";
				strcat(temp, doorDesc);
				strcpy(doorDesc, temp);
			}
			if(routes > 1){
				char temp[60] = " North,";
				strcat(temp, doorDesc);
				strcpy(doorDesc, temp);
			}
			if(routes == 0){
				strcat(doorDesc, "North");
			}
			routes++;
		}
		if(*(feywood +  i) != EMPTY){
			if (routes > 1){
				strcat((level + i + offset)->descr, " There are routes through the undergrowth to your");
			}
			else{
				strcat((level + i + offset)->descr, " There is a route through the undergrowth to your");
			}
			strcat(doorDesc, ".");
			strcat((level + i + offset)->descr, doorDesc);
		}
	}//this connects all the rooms with whatever is North, South, East and West

	free(feywood);
	
	return offset + (xdim * ydim);
}

struct room * createWorld(){
	
	struct room (*world)[34] = calloc(34, sizeof(struct room));

	int roomCount = 1;//room 0 is forbidden

	roomCount = buildFixedLevel(world[0], roomCount);

	roomCount = buildDynamicLevel(world[0], 1, 2, 7, 4, 4, roomCount);

	return world[0];
}

