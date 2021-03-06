#include "components.h"

bullet* bullet_new(gameObject* go, gameObject* owner, float speed, int damage, vec2 dir, vec2 cannon_pivot, const char* sfx_path){
    bullet* b = (bullet*)malloc(sizeof(bullet));
    b->transform = (transform*)gameObject_get_component(go, TRANSFORM_T);
    b->owner = owner;
    b->speed = speed;
    b->damage = damage;
    b->dir = dir;
    b->__cannon_pivot = cannon_pivot;
    b->sfx = sfx_path == NULL ? NULL : audio_emitter_new(go, sfx_path, 0, WAV, 100, false);

    component* c = __component_new(go);
    c->data = b;
    c->init = bullet_init;
    c->update = bullet_update;
    c->on_enable = bullet_on_enable;
    c->on_disable = bullet_on_disable;
    c->destroy = bullet_destroy;
    c->type = BULLET_T;

    return b;
}

void bullet_init(component* comp){

}

void bullet_update(component* comp){
    bullet* b = (bullet*)comp->data;
    vec2 dirSpeed = vec2_mul_scal(&b->dir, b->speed);
    vec2 endSpeed = vec2_mul_scal(&dirSpeed, delta_time_sec(comp->owner->__scene));
    b->transform->pos = vec2_add(&b->transform->pos, &endSpeed);
    sprite* s = (sprite*)gameObject_get_component(comp->owner, SPRITE_T);
    
    if(b->transform->pos.x > comp->owner->__scene->__game->width || b->transform->pos.x < 0) 
        gameObject_set_active(comp->owner, false);
    else if(b->transform->pos.y > comp->owner->__scene->__game->height || b->transform->pos.y <  -s->dst_rect.h)
        gameObject_set_active(comp->owner, false);
}

void bullet_destroy(component* comp){
    bullet* b = (bullet*)comp->data;
    free(b);
}

void bullet_on_enable(component* comp){
    bullet* b = (bullet*)comp->data;
    transform* ownerT = (transform*)gameObject_get_component(b->owner, TRANSFORM_T);
    //vec2 endOffset = vec2_mul_scal(&offset, );
    if(b->sfx != NULL) audio_play(b->sfx);
    b->transform->pos = vec2_add(&ownerT->pos, &b->__cannon_pivot);
}

void bullet_on_disable(component* comp){
}

void bullet_on_enter(component* me, component* other){
    circle_collider* meCC = (circle_collider*)gameObject_get_component(me->owner, CIRCLE_COLLIDER_T);
    bullet* b = (bullet*)gameObject_get_component(me->owner, BULLET_T);
    gameObject_set_active(me->owner, false);
    if(meCC->collider_mask == PLAYER_MASK){
        enemy_behaviour* eb = (enemy_behaviour*)gameObject_get_component(other->owner, ENEMY_T);
        enemy_on_damage(eb, b->damage);
    }
    else if(meCC->collider_mask == ENEMY_MASK){
        playerBehaviour* pb = (playerBehaviour*)gameObject_get_component(other->owner, PLAYER_T);
        player_on_damage(pb, b->damage);
    }
}
