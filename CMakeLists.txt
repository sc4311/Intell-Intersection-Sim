cmake_minimum_required(VERSION 3.26)
project(code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(code
        car.c
        car.h
        driver.c
        equalsInt.c
        equalsInt.h
        event.c
        event.h
        graph.c
        graph.h
        hashTable.c
        hashTable.h
        makefile
        priorityQueue.c
        priorityQueue.h
        queue.c
        queue.h
        road.c
        road.h
        trafficSimulator.c
        trafficSimulator.h)
