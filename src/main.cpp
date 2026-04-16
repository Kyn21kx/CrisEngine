#include <cassert>
#include <cstdint>
#include "asset_manager/asset_manager.h"
#include "raylib.h"
#include "resource_manager.h"

AssetManager g_assetManager;
Texture2D g_tex;

// This is a change for visualizing git
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

    Image sampleImage{};
    auto loadImageErr = ResourceManager::LoadImageIntoBuffer(buffer, asset.byteSize, &sampleImage);
    assert(loadImageErr == ResourceManager::EError::Ok);

    g_tex = LoadTextureFromImage(sampleImage);
}

void Update() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello, Raylib in C!", 190, 200, 20, DARKGRAY);
    DrawTexture(g_tex, 190, 200, WHITE);

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
