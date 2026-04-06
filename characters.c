#include <stdlib.h>
#include "world.h" 
#include "characters.h"
#include "items.h"
#include "string.h"


static void sort(int list[INVENTORYSIZE]){
	for (int i = INVENTORYSIZE - 1; 0 < i; i--) {
		if (list[i - 1] == 0) {
			list[i - 1] = list[i];
			list[i] = 0;	
		}	
	}

}//, not a genuine sort, it just shuffles everything to the front of an array holding items, specifically either in a room or inventory.


int takeItem(struct character * actor, struct room * area) {
	int takeId = idOf(actor->target);
	int takeIndex = -1;
	int foundLocation = (INVENTORYSIZE - 1);
			
	do{
	if (takeId == (area)[actor->location].items[foundLocation]){
		takeIndex = foundLocation;
	}
		foundLocation--;
	}while( (-1 < foundLocation) && (takeIndex == -1) );

	if (actor->inventory[INVENTORYSIZE - 1] != NOITEM) {
		return FAIL2;	
	}

	else if (takeIndex != -1){
		int emptySlot = 0;
		while (actor->inventory[emptySlot] != NOITEM) {
			emptySlot++;
		}
		actor->inventory[emptySlot] = takeId;
		sort(actor->inventory);
		(area)[actor->location].items[takeIndex] = NOITEM;
		sort( (area)[actor->location].items );
		return SUCCESS;
	}

	else {
		return FAIL1;
	}

	
}

int dropItem(struct character * actor, struct room * area) {
	int dropId = idOf(actor->target);
	int dropIndex = -1; 
	int foundSlot = 0;
			
	do{
		if (dropId == actor->inventory[foundSlot]) {
			dropIndex = foundSlot;
		}
		foundSlot++;
	}while( (foundSlot < INVENTORYSIZE) && (dropIndex == -1) );

	if ( (area)[actor->location].items[INVENTORYSIZE - 1] != NOITEM){
		return FAIL2;
	}

	else if (dropIndex != -1) {
		int emptyLocation = 0;
		while ( (area)[actor->location].items[emptyLocation] != NOITEM) {
			emptyLocation++;
		}
		(area)[actor->location].items[emptyLocation] = dropId;
		sort( (area)[actor->location].items );
		actor->inventory[dropIndex] = 0;
		sort(actor->inventory);
		return SUCCESS;
	}
	else{
		return FAIL1;
	}
}

void characterMove(struct character * actor, int target) {
	actor->location = target;
}

int walkRoom(struct character * actor, struct room * area){
	
	if ( (strncmp("north", actor->target, 5) == 0) && ( (area)[actor->location].northCon != 0) ) {
		actor->location = (area)[actor->location].northCon;
		return SUCCESS;
	}
	else if ( (strncmp("south", actor->target, 5) == 0) && ( (area)[actor->location].southCon != 0) ){
		actor->location = (area)[actor->location].southCon;
		return SUCCESS;
	}
	else if ( (strncmp("east", actor->target, 4) == 0) && ( (area)[actor->location].eastCon != 0) ) {
		actor->location = (area)[actor->location].eastCon;
		return SUCCESS;		
	}
	else if ( (strncmp("west", actor->target, 4) == 0) && ( (area)[actor->location].westCon != 0) ) {
		actor->location = (area)[actor->location].westCon;
		return SUCCESS;
	}
	else {
		return FAIL1;
	}
}
