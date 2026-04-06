#ifndef FEYWOOD_ITEMS
#define FEYWOOD_ITEMS

struct item {
	int id;
	char name[12];
};

/*absolutely never put ERROR anywhere*/
enum itemNames {NOITEM = 0, DAGGER = 1, SWORD = 2, AXE = 3, ORB = 4, CHALICE = 5, RUBY = 6, error = 7};

extern const struct item ITEMS[]; 

int idOf(char*); //put the name of the item in to assign the appropriate id in an inventory array, eg you can say
		 // idOf("ruby"); to place 6 in the array of ints to be called back later.



#endif
