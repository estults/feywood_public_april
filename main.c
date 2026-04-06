#include <stdlib.h>
#include <stdio.h>
#include "world.h"
#include "characters.h"
#include "interface.h"

int main() {
	struct character *mainCharacter = calloc(1, sizeof(struct character)); 

	struct room *world = createWorld();
	
	printf("Welcome to the Feywood: A Realm of Magical Adventure!\nType anything to begin your adventure, and type 'quit' at any\
time to close the application.\nYou traverse the game by typing your inputs,and your main actions in the game\
are to 'go,' 'take,' and 'drop.' The 'help' command can give you more instruction.\nThis is a work in progress,\
and currently there isn't really a way to win. I have thus far been mostly interested in the world generation,\
and have not yet added other characters with whom to interact.\n\n");

	characterMove(mainCharacter, 1);

	lookAround(mainCharacter, world);
	
	for (;;) {

		int state = playerDoes(mainCharacter, world);
	
		if (state == QUIT) {
			break;
		}
	}

	free(mainCharacter);
	free(world);

	return 0;
}

