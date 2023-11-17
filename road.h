#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h"

typedef struct RoadData
{
    /*Length of the road*/
    int length;

    /*information to use to record and update the lights*/
    int light;
    int greenOnLength;
    int redOnLength;
    int currCycle;
    int reset;

    /* intersections this road from and moves to*/
    int start;
    int end;

    Car** cars;

    Queue* addCarQueue;

}  RoadData;

#endif
void printCar();
RoadData* createRoad(int length, int start, int end);
void moveCars(RoadData* road, int* hasMoved);
int addCar(RoadData* road);
void freeRoad(RoadData* road);
int moveCarToFirst(RoadData* road1, RoadData* road2);
void printCars(RoadData* road);
void printRoads(int time, RoadData** roadArray, int numRoads);

