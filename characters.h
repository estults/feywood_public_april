#ifndef FEYWOOD_CHARACTERS_H
#define FEYWOOD_CHARACTERS_H

#include "config.h"

struct character {
	int location;
	int inventory[INVENTORYSIZE];
	char action[14];
	char target[14];
};


enum actionReturn {
	SUCCESS = 0,
	FAIL1 = 1,
	FAIL2 = 2};	

int takeItem(struct character * actor, struct room * area);//character takes an items from their location, returning SUCCESS, 
							   //FAIL1 if it is not in their inventory, 
							   //and FAIL2 if there is no room in their inventory.

int dropItem(struct character * actor, struct room * area);//character drops an item at their location. returns either 
							   //SUCCESS, FAIL1 if it is not in their inventory, 
							   //FAIL2 if there is no room at that location

void characterMove(struct character * actor, int target);//unconditionally moves from one room to 
									    //another. be careful not to warp out of bounds

int walkRoom(struct character * actor, struct room * area);//attempts to move from one room to an adjacent one by 
							   //walking through doorways, and results in SUCCESS if 
							   //there is a room that way and movement transpired or 
							   //FAIL1 in the case that there is no room in that direction 

#endif
