#include"components.h"

void enemy_new(gameObject* go, enemy_data* e_data){
    enemy_behaviour* e = (enemy_behaviour*)malloc(sizeof(enemy_behaviour));
    component* c = __component_new(go);
    e->dir = vec2_new(0, 1);
    e->__speed = e_data->speed;
    e->__shootCD = e_data->shootCD;
    e->__shootTimer = e_data->shootCD * .5f;
    e->__maxHealth = e_data->maxHealth;
    e->__currHealth = e_data->maxHealth;
    e->__bullets = createQueue();
    e->sfx = audio_emitter_new(go, "resources/assets/audio/snd_explosion2.wav", 0, WAV, 60, false);

    for (uint i = 0; i < 20; i++)
    {
        vec2 pos = vec2_new(-300, -300);
        gameObject* bullGo = gameObject_new_with_coord(go->__scene, "", &pos);
        int switch_x = (i % 2 == 0 ? -e_data->cannon_pivot.x * 2 : e_data->cannon_pivot.x);
        vec2 cannon_pivot = vec2_new(e_data->cannon_switch ? switch_x : e_data->cannon_pivot.x, e_data->cannon_pivot.y); 
        //vec2 cannon_pivot = vec2_new(i % 2 == 0 ? -20 : 10, 10);
        bullet* b = bullet_new(bullGo, go, e_data->bull_speed, e_data->bull_damage, e->dir, cannon_pivot, NULL);
        sprite_new(bullGo, "resources/assets/enemy/enemybullet1.png", 0, e_data->bull_size, e_data->bull_size);
        circle_collider_new(bullGo, 0, bullet_on_enter, NULL, NULL);
        circle_collider_set_collision(bullGo, ENEMY_MASK, PLAYER_MASK);
        gameObject_set_active(bullGo, false);
        enqueue(e->__bullets, bullGo);
    }

    c->data = e;
    c->init = enemy_init;
    c->update = enemy_update;
    c->destroy = enemy_destroy;
    c->on_enable = enemy_on_enable;
    c->on_disable = enemy_on_disable;
    c->type = ENEMY_T;
    e->__component = c;
}

void enemy_init(component* comp){
    enemy_behaviour* eb = (enemy_behaviour*)comp->data;
    gameObject* pGo = scene_get_obj(comp->owner->__scene, "spawner");
    enemy_spawner* es = (enemy_spawner*)gameObject_get_component(pGo, ENEMY_SPAWNER_T);
    eb->particles = es->particles;
}

void enemy_update(component* c){
    //movement
    transform* t = (transform*)gameObject_get_component(c->owner, TRANSFORM_T);
    enemy_behaviour* eb = (enemy_behaviour*)gameObject_get_component(c->owner, ENEMY_T);
    sprite* s = (sprite*)gameObject_get_component(c->owner, SPRITE_T);
    double dt = delta_time_sec(c->owner->__scene);
    t->pos.x += eb->dir.x * eb->__speed * dt;
    t->pos.y += eb->dir.y * eb->__speed * dt;

    //shoot
    eb->__shootTimer += delta_time_sec(c->owner->__scene);
    if(eb->__shootTimer >= eb->__shootCD){
        gameObject* bullGo = (gameObject*)dequeue(eb->__bullets);
        gameObject_set_active(bullGo, true);
        enqueue(eb->__bullets, bullGo);
        eb->__shootTimer = 0;
    }
}

void enemy_destroy(component* comp){
    enemy_behaviour* eb = (enemy_behaviour*)comp->data;
    destroyQueue(eb->__bullets);
    free(eb);
}

void enemy_on_enable(component* comp){
    enemy_behaviour* p = (enemy_behaviour*)comp->data;
}

void enemy_on_disable(component* comp){
    enemy_behaviour* p = (enemy_behaviour*)comp->data;
}

boolean enemy_on_damage(enemy_behaviour* eb, int damage){
    if(eb == NULL) return false;
    eb->__currHealth -= damage;
    if(eb->__currHealth <= 0){
        gameObject_set_active(eb->__component->owner, false);

        gameObject* particle = dequeue(eb->particles);
        particle->transform->pos = eb->__component->owner->transform->pos;
        gameObject_set_active(particle, true);
        enqueue(eb->particles, particle);

        audio_play(eb->sfx);

        return true;
    }
    return false;
}