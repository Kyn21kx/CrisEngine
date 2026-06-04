#include <cassert>
#include <cstdint>
#include "asset_manager/asset_manager.h"
#include "ecs/ecs.hpp"
#include "raylib.h"
#include "resource_manager.h"

AssetManager g_assetManager;
Texture2D g_tex;
ComponentId positionComp;
Entity playerEnt;

struct Position {
    float x;
    float y;
};

void Intialize() {
    // Initialization
    const int32_t screenWidth = 800;
    const int32_t screenHeight = 600;

    g_assetManager.Scan("C:\\Users\\nefes\\Personal\\ChrisEngine\\demo_project");

    const Asset& asset = g_assetManager.GetAssetAt(0);
    
    uint8_t* buffer = static_cast<uint8_t*>(malloc(asset.byteSize));

    assert(buffer != nullptr && "Failed to allocate!");
    
    AssetManager::EError loadResErr = g_assetManager.LoadResource(asset, buffer, asset.byteSize);

    if (loadResErr != AssetManager::EError::Ok) {
        printf("Failed to load resource!\n");
    }

    InitWindow(screenWidth, screenHeight, "Raylib Basic Loop");

    SetTargetFPS(60);

    // Image sampleImage{};
    // auto loadImageErr = ResourceManager::LoadImageIntoBuffer(buffer, asset.byteSize, &sampleImage);
    // assert(loadImageErr == ResourceManager::EError::Ok);

    // g_tex = LoadTextureFromImage(sampleImage);


    ecs_init();

    playerEnt = ecs_new_entity();
    positionComp = ecs_component_register(sizeof(Position), alignof(Position));
    Position posComp{};
    ecs_add(playerEnt, positionComp, &posComp);
    
    Position* pos = (Position*)ecs_get(playerEnt, positionComp);
    pos->x = 190;
    pos->y = 200;
}

void Update() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    Position* pos = (Position*)ecs_get(playerEnt, positionComp);
    pos->x += GetTime();
    pos->y += GetTime();

    DrawText("Hello, Raylib in C!", pos->x, pos->y, 20, DARKGRAY);
    EndDrawing();
}

int main() {
	Intialize();
    while (!WindowShouldClose())
    {
        Update();
    }

    CloseWindow();        // Close window and OpenGL context	
	return 0;
}
