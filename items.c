#include <stdlib.h>
#include <string.h>
#include "items.h"


const struct item ITEMS[] = {{DAGGER, "dagger"}, {SWORD, "sword"}, {AXE, "axe"}, {ORB, "orb"}, {CHALICE, "chalice"}, 
{RUBY, "ruby"}, {error, "error"}};

const int ITEMLENGTH = sizeof(ITEMS);

int idOf(char *itemName){
	for (int i = 0; i<ITEMLENGTH; i++) { 
		int length = strlen(ITEMS[i].name);
		if (strncmp(ITEMS[i].name, itemName, length) == 0) {
			return ITEMS[i].id;
		}
	}
}

