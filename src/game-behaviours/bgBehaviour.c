#include "components.h"

void bg_behaviour_new(gameObject* go, float speed){
    bgBehaviour* bb = (bgBehaviour*)malloc(sizeof(bgBehaviour));
    component* c = __component_new(go);
    bb->speed = speed;
    c->data = bb;
    c->update = bg_behaviour_update;
    c->destroy = bg_behaviour_destroy;
    c->on_enable = NULL;
    c->init = NULL;
    c->on_disable = NULL;
}

void bg_behaviour_update(component* comp){
    bgBehaviour* bb = (bgBehaviour*)comp->data;
    comp->owner->transform->pos.y += bb->speed * delta_time_sec(comp->owner->__scene);

    int h = comp->owner->__scene->__game->height;

    if (comp->owner->transform->pos.y >= h)
        comp->owner->transform->pos.y = -h;
}

void bg_behaviour_destroy(component* comp){
    bgBehaviour* bb = (bgBehaviour*)comp->data;
    free(bb);
}
