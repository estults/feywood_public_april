#ifndef FEYWOOD_WORLD_H
#define FEYWOOD_WORLD_H

#include "config.h"

struct room{

char descr[800];
int items[INVENTORYSIZE];
int intel; //either 0 or 1//
int northCon;
int southCon;
int eastCon;
int westCon;

};

struct room * createWorld();

#endif

