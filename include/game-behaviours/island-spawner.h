#ifndef ISLAND_SPAWNER_H
#define ISLAND_SPAWNER_H
#include "queue.h"

struct island_spawner{
    vector* __sprites;
    queue* islands;
    component* __component;
    float spawnCD;
    float __spawnTimer;
    int spawnNum;
    int island_speed;
}; typedef struct island_spawner island_spawner;

void island_spawner_new(gameObject* go, float spawnCD, int island_speed);
void island_spawner_init(component* c);
void island_spawner_update(component* c);
void island_spawner_destroy(component* c);

#endif