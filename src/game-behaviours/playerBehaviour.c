#include "components.h"

void player_new(gameObject* go, float speed, float shootCooldown, int maxHealth){
    playerBehaviour* p = (playerBehaviour*)malloc(sizeof(playerBehaviour));
    component* c = __component_new(go);
    p->__speed = speed;
    p->__shootCD = shootCooldown;
    p->__shootTimer = shootCooldown;
    p->__maxHealth = maxHealth;
    p->__currHealth = maxHealth;
    p->comp = c;

    circle_collider_new(go, 0, player_on_collision_enter, NULL, NULL);
    circle_collider_set_collision(go, PLAYER_MASK, ENEMY_MASK);

    //"Simple" bullet creation
    p->__bullets = createQueue();
    for (uint i = 0; i < 40; i++)
    {
        vec2 pos = vec2_new(300, 300);
        gameObject* bullGo = gameObject_new_with_coord(go->__scene, "", &pos);
        vec2 cannon_pivot = vec2_new(i % 2 == 0 ? 0 : 30, -10);
        bullet* b = bullet_new(bullGo, go, 800, 1, vec2_new(0, -1), cannon_pivot, "resources/assets/audio/snd_explosion1.wav");
        sprite_new(bullGo, "resources/assets/player/bullet.png", 0, 0, 0);
        circle_collider_new(bullGo, 0, bullet_on_enter, NULL, NULL);
        circle_collider_set_collision(bullGo, PLAYER_MASK, ENEMY_MASK);
        gameObject_set_active(bullGo, false);
        enqueue(p->__bullets, bullGo);
    }

    p->lives = vector_new();
    for (int i = 0; i < p->__maxHealth; i++)
    {
        vec2 pos = vec2_new(15 + (30 *  i), 410);
        gameObject* life = gameObject_new_with_coord(go->__scene, "life", &pos);
        sprite_new(life, "resources/assets/ui/life.png", 0, 0, 0);
        vector_add(p->lives, life);
    }
    
    c->data = p;
    c->init = player_init;
    c->update = player_update;
    c->destroy = player_destroy;
    c->on_enable = player_on_enable;
    c->on_disable = player_on_disable;
    c->type = PLAYER_T;
}

void __shoot(queue* bullets){
    gameObject* bullGo = (gameObject*)dequeue(bullets);
    gameObject_set_active(bullGo, true);
    enqueue(bullets, bullGo);
}

void player_init(component* comp){
}   

void player_update(component* c){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    transform* t = (transform*)gameObject_get_component(c->owner, TRANSFORM_T);
    playerBehaviour* p = (playerBehaviour*)gameObject_get_component(c->owner, PLAYER_T);
    sprite* s = (sprite*)gameObject_get_component(c->owner, SPRITE_T);
    double dt = delta_time_sec(c->owner->__scene);
    p->__shootTimer += dt;
    if(state[SDL_SCANCODE_LEFT]){
        if((int)t->pos.x > 0) //avoid exit from screen x < 0
            t->pos.x -= 1.f * p->__speed * dt;
    }
    if(state[SDL_SCANCODE_RIGHT]){
        if((int)t->pos.x < c->owner->__scene->__game->width - s->dst_rect.w) //avoid exit from screen x > window.w
            t->pos.x += 1.f * p->__speed * dt;
    }
    if(state[SDL_SCANCODE_UP]){
        if((int)t->pos.y > 0)
            t->pos.y -= 1.f * p->__speed * dt;
    }
    if(state[SDL_SCANCODE_DOWN]){
        if((int)t->pos.y + s->dst_rect.h < 405) //hard coded atm
            t->pos.y += 1.f * p->__speed * dt;
    }
    if(state[SDL_SCANCODE_SPACE]){
        if(p->__shootTimer >= p->__shootCD){
            __shoot(p->__bullets);
            p->__shootTimer = 0;
        }
    }
}

void player_destroy(component* comp){
    playerBehaviour* p = (playerBehaviour*)comp->data;
    free(p); 
}

void player_on_enable(component* comp){

}

void player_on_disable(component* comp){

}

boolean player_on_damage(playerBehaviour* pb, int damage){
    if(pb == NULL) return false;
    pb->__currHealth -= damage;
    if(pb->__currHealth < 0){
        game_end(pb->comp->owner->__scene->__game);
        return true;
    } 
    gameObject_set_active(vector_at(pb->lives, pb->__currHealth), false);
    return false;
}

void player_on_collision_enter(component* me, component* other){
    if(strcmp(other->owner->name, "enemy") != 0) return;
    playerBehaviour* pb = (playerBehaviour*)gameObject_get_component(me->owner, PLAYER_T);
    player_on_damage(pb, 1);
    enemy_behaviour* eb = (enemy_behaviour*)gameObject_get_component(other->owner, ENEMY_T);
    enemy_on_damage(eb, 99);
}