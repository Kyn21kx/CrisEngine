#include <cassert>
#include <cstdint>
#include <cstdio>
#include "asset_manager/asset_manager.h"
#include "platform_layer/engine_platform.hpp"
#include "2dmovement.hpp"
#include "raylib.h"
// #include "resource_manager.h"
#include "ecs/ecs.hpp"

//#include "resource_manager.h"

AssetManager g_assetManager;

ComponentId positionComp;
ComponentId velocityComp;
Entity playerEnt;
Image fullImage{};
Image partialImage{};
Texture2D partialTexture{};
void* libraryHandle;
using script_fn_t = void(*)(Position*, float dt);
script_fn_t scriptEntry;


constexpr float TEXTURE_R = 0;
constexpr float TEXTURE_S = 2;

void Intialize() {
    // Initialization
    const int32_t screenWidth = 800;
    const int32_t screenHeight = 600;
    //NOTE(cris): Introducir una virtaulizacion de path o una solucion cross platform empieza a ser necesario

    InitWindow(screenWidth, screenHeight, "Raylib Basic Loop");

    SetTargetFPS(60);
    
    ecs_init();

    libraryHandle = os::dlib_open("C:\\Users\\nefes\\Personal\\ChrisEngine\\scripts\\build\\Debug_Win64\\scripts\\scripts.dll");
    scriptEntry = (script_fn_t)os::dlib_load_symbol(libraryHandle, "print_fn");


    playerEnt = ecs_new_entity();

    positionComp = ecs_component_register(sizeof(Position), alignof(Position));
    Position posComp{};
    ecs_add(playerEnt, positionComp, &posComp);
    Position* pos = (Position*)ecs_get(playerEnt, positionComp);
    pos->x = 190;
    pos->y = 200;

    velocityComp = ecs_component_register(sizeof(Velocity), alignof(Velocity));
    Velocity velComp{};
    ecs_add(playerEnt, velocityComp, &velComp);
    Velocity* vel = (Velocity*)ecs_get(playerEnt, velocityComp);
    vel->x = 5;
    vel->y = 5;
}

void Update() {

    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    Position* pos = (Position*)ecs_get(playerEnt, positionComp);
    // Velocity* vel = (Velocity*)ecs_get(playerEnt, velocityComp);
    scriptEntry(pos, GetTime());

    
    //NOTE:Udate position after all modifications    
    // DrawTextureEx(partialTexture, {pos->x, pos->y},TEXTURE_R , TEXTURE_S, WHITE);
    DrawText("Hello there", pos->x, pos->y, 24, BLACK);
    EndDrawing();
}

int main() {
	Intialize();
    while (!WindowShouldClose())
    {
        Update();
    }

    CloseWindow();        // Close window and OpenGL context	
    os::dlib_close(libraryHandle);
	return 0;
}
