#include "components.h"
#include "particles.h"

void enemy_spawner_new(gameObject* go, float spawnCD){
    enemy_spawner* e = (enemy_spawner*)malloc(sizeof(enemy_spawner));
    component* c = __component_new(go);
    e->spawnCD = spawnCD;
    e->__spawnTimer = 0;
    e->spawnNum = 2;
    e->__sprites = vector_new();
    vector_add(e->__sprites, "resources/assets/enemy/enemy1_strip3.png");
    vector_add(e->__sprites, "resources/assets/enemy/enemy2_strip3.png");
    vector_add(e->__sprites, "resources/assets/enemy/enemy3_strip3.png");
    vector_add(e->__sprites, "resources/assets/enemy/enemy4_strip3.png");

    e->enemies = createQueue();
    for (uint i = 0; i < 30; i++)
    {
        uint enemy_index = (uint)GetRandomIntBetween(e->__sprites->__count - 1, 0);
        enemy_data ed;
        switch (enemy_index)
        {
        case 0:  //green
            ed.maxHealth = 2;
            ed.shootCD = 0.7;
            ed.speed = 120;
            ed.bull_damage = 1;
            ed.bull_size = 60;
            ed.cannon_pivot = vec2_new(-5, 10);
            ed.cannon_switch = false;
            ed.bull_speed = 500;
            ed.type = GREEN_E;
            break;
        case 1:  //yellow
            ed.maxHealth = 3;
            ed.shootCD = 0.45;
            ed.speed = 95;
            ed.bull_damage = 1;
            ed.bull_size = 40;
            ed.cannon_pivot = vec2_new(-5, 10);
            ed.cannon_switch = false;
            ed.bull_speed = 450;
            ed.type = YELLOW_E;
            break;
        case 2:  //white
            ed.maxHealth = 2;
            ed.shootCD = 0.25;
            ed.speed = 120;
            ed.bull_damage = 1;
            ed.bull_size = 40;
            ed.cannon_pivot = vec2_new(10, 10);
            ed.cannon_switch = true;
            ed.bull_speed = 500;
            ed.type = WHITE_E;
            break;
        case 3: //blue
            ed.maxHealth = 1;
            ed.shootCD = 300;
            ed.speed = 400;
            ed.bull_damage = 1;
            ed.bull_size = 0;
            ed.cannon_pivot = vec2_new(0, 10);
            ed.cannon_switch = false;
            ed.bull_speed = 0;
            ed.type = BLUE_E;
            break;
        default:
            break;
        }
        vec2 pos = vec2_new(-100, -100);
        gameObject* enemy = gameObject_new_with_coord(go->__scene, "enemy", &pos);
        const char* path = (const char*)vector_at(e->__sprites, enemy_index);
        sprite_new_animated(enemy, path, 1, 3, 0.1f);
        enemy_new(enemy, &ed);
        circle_collider_new(enemy, 0, NULL, NULL, NULL);
        circle_collider_set_collision(enemy, ENEMY_MASK, PLAYER_MASK);
        gameObject_set_active(enemy, false);
        enqueue(e->enemies, enemy);
    }

    particles* particles = particles_new(go->__scene, 15);
    e->particles = particles->particles;
    
    c->data = e;
    c->init = enemy_spawner_init;
    c->update = enemy_spawner_update;
    c->destroy = enemy_spawner_destroy;
    c->type = ENEMY_SPAWNER_T;

    e->__component = c;
}

void enemy_spawner_init(component* c){

}

void enemy_spawner_update(component* c){
    enemy_spawner* es = (enemy_spawner*)c->data;
    double dt = delta_time_sec(c->owner->__scene);
    es->__spawnTimer += dt;
    if(es->__spawnTimer >= es->spawnCD){

        for (int i = 0; i <= es->spawnNum; i++)
        {
            gameObject* go = dequeue(es->enemies);
            enemy_behaviour* eb = (enemy_behaviour*)gameObject_get_component(go, ENEMY_T);
            vec2 p = vec2_new(GetRandomIntBetween(go->__scene->__game->width,0), -50);
            go->transform->pos = p;
            gameObject_set_active(go, true);
            enqueue(es->enemies, go);
        }
        es->__spawnTimer = 0;
    }   
}

void enemy_spawner_destroy(component* c){

}
