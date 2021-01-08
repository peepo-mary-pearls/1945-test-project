#ifndef ENEMY_BEHAVIOUR_H
#define ENEMY_BEHAVIOUR_H

enum move_axis {
    HORIZONTAL,
    VERTICAL
}; typedef enum move_axis move_axis;

enum enemy_type{
    GREEN_E, //single line shoot
    YELLOW_E, //single line shoot
    BLUE_E, //kamikaze
    WHITE_E //shoot like player
}; typedef enum enemy_type enemy_type; 

struct enemy_behaviour{
    vec2 dir;
    float __speed;
    int __maxHealth;
    int __currHealth;
    float __shootCD;
    float __shootTimer;
    queue* __bullets;
    component* __component;
    queue* particles;
    struct audio_emitter* sfx;
};typedef struct enemy_behaviour enemy_behaviour;

void enemy_new(gameObject* go, struct enemy_data* e_data);
void enemy_init(component* comp);
void enemy_update(component* c);
void enemy_destroy(component* comp);
void enemy_on_enable(component* comp);
void enemy_on_disable(component* comp);
boolean enemy_on_damage(enemy_behaviour* eb, int damage);

#endif