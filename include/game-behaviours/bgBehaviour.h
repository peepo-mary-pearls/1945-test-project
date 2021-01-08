#ifndef BG_BEHAVIOUR_H
#define BG_BEHAVIOUR_H

struct bgBehaviour{
    float speed;
};typedef struct bgBehaviour bgBehaviour;

void bg_behaviour_new(gameObject* go, float speed);
void bg_behaviour_update(component* comp);
void bg_behaviour_destroy(component* comp);

#endif