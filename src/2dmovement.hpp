/*#pragma once
#include <raylib.h>
struct Position{
  float x;
  float y;
};

struct Velocity {
  float x;
  float y;
};

constexpr float SPEED = 200;
extern "C" {
void key_movement(Velocity *vel);
void set_direction(Position *current_pos, Position targe_pos, Velocity *vel);
void mouse_movement(Position *pos, Velocity *vel);
void update_pos(Position *pos, Velocity *vel);
}*/

#pragma once
typedef void (*print_sig)(void);

extern "C" void print();

