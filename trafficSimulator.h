#ifndef _trafficSimulator_h
#define _trafficSimulator_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


#include "graph.h"
#include "queue.h"
#include "priorityQueue.h"
#include "event.h"
#include "road.h"

typedef struct TrafficData
{
    //Optional TODO - Fill this struct in with the data needed to simulate traffic.  Suggested data:
    /* graph representing the road network */
    Graph  *roadNetwork;
    /* array of the roads of the graph in the sequence they were added to the graph */
    int numOfRoads;
    RoadData **roadArray;
    /* priority queue of events where the priority represents the time the event will occur */
    PriorityQueue *eventQueue;
    /* track the number of cars still in the simulator */
    int numOfCars;
    /* track the longest number of time steps that any light takes to cycle around.  This is useful for detecting gridlock. */
    int longestCycle;
    int gridlock;
}  TrafficData;

void printNames( );

TrafficData* createTrafficData( char* name );
void trafficSimulator( TrafficData* pTrafficData );
void freeTrafficData( TrafficData* pTrafficData );

#endif
