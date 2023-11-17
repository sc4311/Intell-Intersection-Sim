#include "road.h"

/* Suggested TODO - Write a function to create road.
 */
RoadData* createRoad(int length, int start, int end){
    RoadData* road = malloc(sizeof(RoadData));
    road->length = length;
    road->start = start;
    road->end = end;
    road->light = 0;
    road->cars = malloc(sizeof(Car*) * length);

    int k;
    for(k = 0; k < length; k++){
        road->cars[k] = NULL;
    }
    return road;
}

/* Suggested TODO - Write a function to free a road.
 */
void freeRoad(RoadData* road){
    if(!isEmpty(road->addCarQueue)){
        freeQueue(road->addCarQueue);
    }
    freeQueue(road->addCarQueue);
    int k = 0;
    for(k = 0; k <road->length; k++){
        if(road->cars[k] != NULL){
            freeCar(road->cars[k]);
        }
    }
    free(road->cars);
    free(road);
}
/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */
void moveCars(RoadData* road, int* hasMoved) {
    int end = road->length - 1;
    int k;
    for (k = end; k >= 0; k--) {
        if (k == 0) {
            break;
        } else if (road->cars[k] == NULL) {
            continue;
        }
        else if(road->cars[k-1] == NULL){
            if(road->cars[k]->moved == 1){
                road->cars[k]->moved = 0;
            }
            else{
                road->cars[k-1] = road->cars[k];
                road->cars[k] = NULL;
                *hasMoved = 1;
            }
        }
    }
}
/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road
 * to the last position of a another given road.
 */
int moveCarToFirst(RoadData* road1, RoadData* road2){
    if(road1->cars[0] == NULL){
        return 0;
    }
    else if(road2->cars[road2->length - 1] == NULL){
        road2->cars[road2->length-1] = road1->cars[0];
        road1->cars[0] = NULL;
        return 1;
    }
    return 0;
}
/* Suggested TODO - Write a function to attempt to move a car from the add car queue onto the end of this road.
 */
int addCar(RoadData* road){
    if(road->cars[road->length - 1] == NULL){
        Car* car = dequeue(road->addCarQueue);
        if (car == NULL){
            return 0;
        }
        else{
            road->cars[road->length - 1] = car;
            return 1;
        }
    }
    return 0;
}
/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */

/* Suggested TODO - Write a function to print the destinations of the cars on the road
 */
//printf("Cars on the road from %d to %d: \n", ????, ????);
void printCars(RoadData* road){
    int k;
    for(k = 0; k <road->length; k++){
        if(road->cars[k] == NULL){
            printf("_ ");
        }
        else{
            printf("%d ", road->cars[k]->destination);
        }
    }
    printf("\n");
}

void printRoads(int time, RoadData** roadArray, int numRoads){
    printf("Cars on the road from %d to %d: \n", roadArray[0]->start, roadArray[0]->end);
    int k;
    for(k = 0; k < numRoads; k++){
        printf("Cars on the road from %d to %d: \n", roadArray[k]->start, roadArray[k]->end);

        int l;
        for(l = 0; l <roadArray[k]->length; l++){
            if(roadArray[k]->cars[l] == NULL){
                printf("_ ");
            }
            else{
                printf("%d ", roadArray[k]->cars[l]->destination);
            }
        }
        printf("\n");
    }
    printf("\n");
}

