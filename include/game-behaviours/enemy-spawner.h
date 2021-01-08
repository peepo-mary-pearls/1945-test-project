#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H
#include "queue.h"

struct enemy_data{
    enemy_type type;
    float speed;
    float shootCD;
    int maxHealth;
    int  bull_damage;
    int bull_size;
    float bull_speed;
    vec2 cannon_pivot;
    boolean cannon_switch;
}; typedef struct enemy_data enemy_data;

struct enemy_spawner{
    vector* __sprites;
    queue* enemies;
    queue* particles;
    component* __component;
    float spawnCD;
    float __spawnTimer;
    int spawnNum;
}; typedef struct enemy_spawner enemy_spawner;

void enemy_spawner_new(gameObject* go, float spawnCD);
void enemy_spawner_init(component* c);
void enemy_spawner_update(component* c);
void enemy_spawner_destroy(component* c);

#endif