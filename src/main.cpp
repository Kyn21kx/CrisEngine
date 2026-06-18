/*#include <cassert>
#include <cstdint>
#include <cstdio>
#include "asset_manager/asset_manager.h"
#include "resource_manager.h"
#include "ecs/ecs.hpp"
#include "gameplay.hpp"
#include <dlfcn.h>

//#include "resource_manager.h"

AssetManager g_assetManager;

ComponentId positionComp;
ComponentId velocityComp;
Entity playerEnt;
Image fullImage{};
Image partialImage{};
Texture2D partialTexture{};


constexpr float TEXTURE_R = 0;
constexpr float TEXTURE_S = 2;

void Intialize() {
    // Initialization
    const int32_t screenWidth = 800;
    const int32_t screenHeight = 600;
//NOTE(cris): Introducir una virtaulizacion de path o una solucion cross platform empieza a ser necesario
    g_assetManager.Scan("/home/cristhian/CrisEngine/media/");

   const Asset& asset = g_assetManager.GetAssetAt(0);
    
    uint8_t* buffer = static_cast<uint8_t*>(malloc(asset.byteSize));

    assert(buffer != nullptr && "Failed to allocate!");
    
    AssetManager::EError loadResErr = g_assetManager.LoadResource(asset, buffer, asset.byteSize);

    if (loadResErr != AssetManager::EError::Ok) {
        printf("Failed to load resource!\n");
    }

    InitWindow(screenWidth, screenHeight, "Raylib Basic Loop");

    SetTargetFPS(60);
    
     auto loadImageErr = ResourceManager::LoadImageIntoBuffer(buffer, asset.byteSize, &fullImage);
     assert(loadImageErr == ResourceManager::EError::Ok);

    partialImage = ImageFromImage(fullImage,{ 0, 0 , 30 , 40 });
    partialTexture = LoadTextureFromImage(partialImage);
    ecs_init();



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
    Velocity* vel = (Velocity*)ecs_get(playerEnt, velocityComp);
    
    key_movement(vel);
    mouse_movement(pos, vel);
    //NOTE:Udate position after all modifications    
    update_pos(pos, vel);
    DrawTextureEx(partialTexture, {pos->x, pos->y},TEXTURE_R , TEXTURE_S, WHITE);
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
*/

#include <cassert>
#include <cstdlib>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>

#include "2dmovement.hpp"


void dlib_loader(const char* library_path, const char* function_name){    
    print_sig print_fnc;

void* libhandle = dlopen(library_path, RTLD_NOW);

     //assert(libhandle == NULL && "Library not found!");  
    
(void) dlerror();

*(void **)(&print_fnc) = dlsym(libhandle,function_name);

//const char* error = dlerror();
 //assert(error!=NULL && "Function not found!");
 
 print_fnc();

dlclose(libhandle);

}



int main(){

    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
         dlib_loader("/home/cristhian/CrisEngine/libgameplay.so", "print");
            }    
    return 0;
}
