#include "components.h"

void island_spawner_new(gameObject* go, float spawnCD, int island_speed){
    island_spawner* is = (island_spawner*)malloc(sizeof(island_spawner));
    component* c = __component_new(go);
    is->spawnCD = spawnCD;
    is->__spawnTimer = spawnCD;
    is->island_speed = island_speed;
    is->__sprites = vector_new();
    vector_add(is->__sprites, "resources/assets/map/island1.png");
    vector_add(is->__sprites, "resources/assets/map/island2.png");
    vector_add(is->__sprites, "resources/assets/map/island3.png");

    is->islands = createQueue();
    for (uint i = 0; i < 30; i++)
    {
        vec2 pos = vec2_new(-100, -100);
        vec2 dir = vec2_new(0,1);
        gameObject* island = gameObject_new_with_coord(go->__scene, "island", &pos);
        const char* path = (const char*)vector_at(is->__sprites, (uint)GetRandomIntBetween(is->__sprites->__count - 1, 0));
        sprite_new(island, path, 0, 0, 0);
        bg_behaviour_new(island, is->island_speed);
        gameObject_set_active(island, false);
        enqueue(is->islands, island);
    }
    
    c->data = is;
    c->init = island_spawner_init;
    c->update = island_spawner_update;
    c->on_enable = NULL;
    c->on_disable = NULL;
    c->destroy = island_spawner_destroy;
    c->type = ISLAND_SPAWNER_T;

    is->__component = c;
}

void island_spawner_init(component* c){

}

void island_spawner_update(component* c){
    island_spawner* is = (island_spawner*)c->data;
    double dt = delta_time_sec(c->owner->__scene);
    is->__spawnTimer += dt;
    if(is->__spawnTimer >= is->spawnCD){

        gameObject* go = dequeue(is->islands);
        vec2 p = vec2_new(GetRandomIntBetween(go->__scene->__game->width,0), -50);
        go->transform->pos = p;
        gameObject_set_active(go, true);

        enqueue(is->islands, go);
        is->__spawnTimer = GetRandomFloatBetween(0.6, -0.6);
    }   
}

void island_spawner_destroy(component* c){
    island_spawner* is = (island_spawner*)c->data;
    destroyQueue(is->islands);
    free(is);
}
