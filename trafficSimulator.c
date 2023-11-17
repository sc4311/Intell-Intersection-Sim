#include "trafficSimulator.h"

typedef struct addCarEvent addCarEvent;

void printRoadData(int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength );
int max( int a, int b );

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("<student name #1>\n");
    printf("<student name #2 (if no partner write \"N/A\")>\n\n");
}

/* createTrafficData
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData* createTrafficData( char* filename ) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    TrafficData *trafficData = malloc(sizeof(TrafficData));
    trafficData->gridlock = 0;
    trafficData->numOfCars = 0;
    int numOfVertex;
    int numOfRoads;
    fscanf(fp, "%d %d", &numOfVertex, &numOfRoads);
    trafficData->roadArray = (RoadData **) malloc(sizeof(RoadData *) * numOfRoads);

    int curr = 0;
    trafficData->roadNetwork = createGraph(numOfVertex);
    trafficData->eventQueue = createPQ();
    trafficData->longestCycle = 0;
    trafficData->numOfRoads = 0;
    int i;
    fscanf(fp, "\n\n");
    for (i = 0; i < numOfVertex; i++) {
        addVertex(trafficData->roadNetwork, i);
    }
    for( i = 0; i <numOfVertex; i++){
        int size;
        fscanf(fp, "%d\n", &size);
        int j;
        for(j = 0; j < size; j++){
            int fromVertex;
            int length;
            int greenOn;
            int greenOff;
            int cycleResets;
            fscanf(fp, "%d %d   %d %d %d\n", &fromVertex, &length, &greenOn, &greenOff, &cycleResets);
            setEdge(trafficData->roadNetwork, fromVertex, i, length);
            trafficData->longestCycle = cycleResets> trafficData->longestCycle ? cycleResets : trafficData->longestCycle;
            edgeType data = createRoad(length, fromVertex, i);
            data-> greenOnLength = greenOn;
            data-> redOnLength = greenOff;
            data-> currCycle = 0;
            data-> reset = cycleResets;
            trafficData->numOfRoads++;
            trafficData->roadArray[curr++] = data;

            setEdgeData(trafficData->roadNetwork, fromVertex, i, data);
        }
    }
    fscanf(fp, "\n\n");
    int numOfCarCommands;
    fscanf(fp, "%d\n", &numOfCarCommands);
    fscanf(fp, "\n\n");
    for(i = 0; i < numOfCarCommands; i++){
        int fromVertex;
        int toVertex;
        int time;
        fscanf(fp, "%d %d %d\n", &fromVertex, &toVertex, &time);
        int numOfCars;
        fscanf(fp, "%d\n", &numOfCars);
        int* destinationsByCar = (int*) malloc(sizeof(int) * numOfCars);
        PQNode* node = (PQNode*) malloc(sizeof(PQNode));
        Event* qt = (Event*) malloc(sizeof(Event));
        node->qt = qt;
        node->priority = time;
        node->pNext = NULL;
        node->qt->eventCode = ADD_CAR_EVENT;
        int j;
        for(j = 0; j< numOfCars; j++) {
            fscanf(fp, "%d\n", &destinationsByCar[j]);
            Event *newCarCommand = createAddCarEvent(time, trafficData->roadArray[fromVertex * numOfVertex + toVertex]);
            newCarCommand->destination = destinationsByCar[j];
            newCarCommand->fromVertex = fromVertex;
            newCarCommand->toVertex = toVertex;
            node->qt->carEventPointer = newCarCommand;
        }
        enqueueByPriority(trafficData->eventQueue, node->qt,time);
        free(destinationsByCar);
        free(node);
    }
    fscanf(fp, "\n\n");
    int numOfPrintCommands;
    fscanf(fp, "%d\n", &numOfPrintCommands);
    int* commandTimeStep;
    commandTimeStep = (int*) malloc(sizeof(int) * numOfPrintCommands);
    fscanf(fp, "\n\n");
    for(i = 0; i < numOfPrintCommands; i++){
        fscanf(fp, "%d\n", &commandTimeStep[i]);
        PQNode* node = (PQNode*) malloc(sizeof(PQNode));
        Event* qt = (Event*) malloc(sizeof(Event));
        node->qt = qt;
        node->priority = commandTimeStep[i];
        node->pNext = NULL;
        node->qt->eventCode = PRINT_ROADS_EVENT;
        enqueueByPriority(trafficData->eventQueue, node->qt,commandTimeStep[i]);
        free(node);
    }
    free(commandTimeStep);
    fclose(fp);
    return trafficData;
}

/* Print color coding for printRoadData (may not work outside of Linux environment) */
#define PRINT_COLOR false

/* provided code - DO NOT CHANGE
 * printRoadData
 * input: int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength
 * output: prints the given road
 *
 * Call this function to print out the road data you read in 
 */
void printRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength )
{
	int i;
	char state='R';
	
	printf("Created road from %d to %d with length %d (green=%d; red=%d; reset=%d).\n", from, to, length, greenStartTime, greenEndTime, lightCycleLength );
	printf("Cycle number: ");
	for( i=0; i<25; i++ ){
		printf("%2d ", i );
	}
	printf("...\nLight state : ");	
	if(PRINT_COLOR) 
		printf("\033[0;31m");//Color = RED
	for( i=0; i<25; i++ ){	/* print example light cycle: */
		if( i%lightCycleLength==greenStartTime ){
			if(PRINT_COLOR) 
				printf("\033[0;32m");//Color = GREEN
			state = 'G';
		}
		if( i%lightCycleLength==greenEndTime ){
			if(PRINT_COLOR) 
				printf("\033[0;31m");//Color = RED
			state = 'R';
		}
		printf("%2c ", state );
	}
	if(PRINT_COLOR) 
		printf("\033[0m");//Reset color
	printf("...\n\n");
}

/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Simulate the road network in the given TrafficData variable
 */
void trafficSimulator( TrafficData* pTrafficData )
{
    TrafficData* trafficData = createTrafficData((char *) pTrafficData);
    int time  = 0;
    int timeSinceLastMove = 0;
    do {
        int hasMoved = 0;

        if(!isEmptyPQ(trafficData->eventQueue)){
            while(getFrontPriority(trafficData->eventQueue) == time) {
                Event *currEvent = (Event *) dequeue((Queue *) trafficData->eventQueue);
                if (currEvent->eventCode == ADD_CAR_EVENT) {
                    int fromVertex = currEvent->carEventPointer->fromVertex;
                    int toVertex = currEvent->carEventPointer->toVertex;
                    int destination = currEvent->carEventPointer->destination;

                    int k;
                    for (k = 0; k < trafficData->numOfRoads; k++) {
                        if (trafficData->roadArray[k]->end == fromVertex &&
                            trafficData->roadArray[k]->start == toVertex) {
                            Car *newCar = malloc(sizeof(Car));
                            newCar->destination = destination;
                            newCar->stepAdded = time;
                            printf("CYCLE %d - ADD_CAR_EVENT- Cars enqeued on road from %d to %d\n", time, fromVertex,
                                   toVertex);
                            enqueue(trafficData->roadArray[k]->addCarQueue, newCar);
                            break;
                        }
                    }
                    trafficData->numOfCars++;
                    free(currEvent->pCarQueue);
                } else if (currEvent->eventCode == PRINT_ROADS_EVENT) {
                    printRoads(time, trafficData->roadArray, trafficData->numOfRoads);
                }
                if (currEvent != NULL) {
                    free(currEvent);
                }
                if (isEmptyPQ(trafficData->eventQueue)) {
                    break;
                }
            }
        }
        int k;
        for(k = 0; k<trafficData->numOfRoads; k++){
            trafficData->roadArray[k]->currCycle += 1;
            if(trafficData->roadArray[k]->currCycle == trafficData->roadArray[k]->greenOnLength){
                trafficData->roadArray[k]->light = 0;
            }
            else if(trafficData->roadArray[k]->currCycle == trafficData->roadArray[k]->greenOnLength + trafficData->roadArray[k]->redOnLength){
                trafficData->roadArray[k]->light = 1;
            }
            if(trafficData->roadArray[k]->currCycle == trafficData->roadArray[k]->reset){
                trafficData->roadArray[k]->currCycle = 0;
            }
        }
        for(k = 0; k < trafficData->numOfRoads; k++){
            RoadData* road = trafficData->roadArray[k];
            if(road->light ==1){
                if(trafficData->roadArray[k]->cars[0]->destination){
                    printf("CYCLE %d - Car has sucsessfully traveled from %d to %d in %d time steps\n", time, road->start, road->end, time - road->cars[0]->stepAdded + 1);
                    free(road->cars[0]);
                    hasMoved = 1;
                }
                else{
                    int nextVertex =- 1;
                    bool success = getNextOnShortestPath(trafficData->roadNetwork, road->end, road->cars[0]->destination, &nextVertex);
                    if(success){
                        int j;
                        for(j = 0; j < trafficData->numOfRoads; j++){
                            if(trafficData->roadArray[j]->end == nextVertex){
                                if(trafficData->roadArray[j]->start == road->end){
                                    if(trafficData->roadArray[j]->cars[trafficData->roadArray[j]->length - 1] == NULL){
                                        trafficData->roadArray[k]->cars[trafficData->roadArray[k]->length - 1] = trafficData->roadArray[j]->cars[0];
                                        trafficData->roadArray[j]->cars[0]->moved = 1;
                                        trafficData->roadArray[j]->cars[0] = NULL;
                                        hasMoved = 1;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        int j;
        for(j = 0; j < trafficData->roadNetwork->numVertices; j++){
            int i;
            for( i = 0; i < trafficData->numOfRoads; i++){
                if(trafficData->roadArray[i]->cars[trafficData->roadArray[i]->length-1] == NULL){
                    if(!isEmpty(trafficData->roadArray[i]->addCarQueue)){
                        Car* car = dequeue(trafficData->roadArray[i]->addCarQueue);
                        car->moved = 1;
                        hasMoved = 1;
                        trafficData->roadArray[i]->cars[trafficData->roadArray[i]->length-1] = car;
                    }
                }
            }
        }
        int i;
        for ( i = 0; i < trafficData->numOfRoads; i++) {
            moveCars(trafficData->roadArray[i], &hasMoved);
        }
        printf("%d\n", timeSinceLastMove);
        timeSinceLastMove++;
        if(hasMoved == 0){
            timeSinceLastMove++;
            if(timeSinceLastMove == trafficData->longestCycle + 2){
                printf("CYCLE %d - Gridlock detected\n", time);
                trafficData->gridlock = 1;
                break;
            }
        }
        else{
            timeSinceLastMove = 0;
        }

    }while(trafficData->numOfCars != 0 && trafficData->gridlock == 0);
    freeTrafficData(trafficData);
}

/* freeTrafficData
 * input: TrafficData* pTrafficData
 * output: N/A
 *
 * Free the data in the TrafficData variable
 */
void freeTrafficData( TrafficData* pTrafficData )
{
    if(!isEmptyPQ(pTrafficData->eventQueue)){
        Event* curr = (Event *) dequeue((Queue *) pTrafficData->eventQueue);
        while(curr != NULL){
            if(curr->eventCode == ADD_CAR_EVENT){
                free(curr->pCarQueue);
            }
            free(curr);
            curr = (Event *) dequeue((Queue *) pTrafficData->eventQueue);
        }
    }
    freePQ(pTrafficData->eventQueue);
    freeGraph(pTrafficData->roadNetwork);
    int p;
    for(p = 0; p < pTrafficData->numOfCars; p++){
        if(!isEmpty(pTrafficData->roadArray[p]->addCarQueue)){
            Car* curr = dequeue(pTrafficData->roadArray[p]->addCarQueue);
            while(curr != NULL){
                free(curr);
                curr = dequeue(pTrafficData->roadArray[p]->addCarQueue);
            }
        }
        freeQueue(pTrafficData->roadArray[p]->addCarQueue);
    }
    for(p = 0; p< pTrafficData->numOfRoads; p++){
        freeRoad(pTrafficData->roadArray[p]);
    }
    free(pTrafficData->roadArray);
    free(pTrafficData);
}

int max( int a, int b )
{
    if(a>b)
        return a;
    return b;
}

