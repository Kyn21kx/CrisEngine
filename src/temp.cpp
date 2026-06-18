/*#include "raylib.h"
#include "gameplay.hpp"
#include <cmath>


Position last_click_pos;
//NOTE(cris): Think about dt
void key_movement(Velocity *vel){
    
    if (IsKeyDown(KEY_A) ) { vel->x -= 2; }
    if (IsKeyDown(KEY_D) ) { vel->x += 2; }
    if (IsKeyDown(KEY_W) ) { vel->y -= 2; }
    if (IsKeyDown(KEY_S) ) { vel->y += 2; }
}
extern "C" {

Vector2 get_direction(const Position *current_pos, const Position *target_pos){
    
    float dy = target_pos->y - current_pos->y;
    float dx = target_pos->x - current_pos->x;
    float length = sqrtf(dx*dx + dy*dy);

    if (length  <= 0.1) {
        //NOTE(cris): si
        return {0,0};
    }
    
    return {dx/length, dy/length};
}

void mouse_movement(Position *pos, Velocity *vel ){
    
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        last_click_pos = {(float)GetMouseX(), (float)GetMouseY()};
        Position target_pos = {last_click_pos.x, last_click_pos.y};
        Vector2 dir = get_direction(pos, &target_pos);

        vel->x = SPEED * dir.x;
        vel->y = SPEED * dir.y;
    }
}

void update_pos(Position *pos,Velocity *vel){

        float dt = GetFrameTime();

    pos->x += vel->x * dt;
    pos->y += vel->y * dt;

    // Stop when close enough
    float dx = last_click_pos.x - pos->x;
    float dy = last_click_pos.y - pos->y;

    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 5.0f) {
        pos->x = last_click_pos.x;
        pos->y = last_click_pos.y;
        vel->x = 0;
        vel->y = 0;
    }
}
*/
#include <cstdio>

extern "C" void print(){
    printf("Hello from gameplay\n");
}
