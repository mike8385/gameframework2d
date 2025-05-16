#include <SDL.h>
#include "simple_logger.h"
#include "gfc_config_def.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "SDL_audio.h"

#include "commands.h"

#include "font.h"

#include "camera.h"
#include "entity.h"
#include "player.h"
#include "player2.h"
#include "monster.h"
#include "fire_wizard.h"
#include "fast_wizard.h"
#include "ice_wizard.h"
#include "melee_wizard.h"
#include "basic_wizard.h"
#include "end_boss.h"
#include "particles.h"
#include "gfc_vector.h"
#include "items.h"
#include "windows.h"
#include "button.h"
#include "pets.h"



#include "world.h"
#include "status.h"

#include "UI.h"
#include "level.h"


Uint8 _DRAWBOUNDS_ = 0;
int process = 0;
int done = 0;
int currentLevel = 0;
const Uint8* keys;
int multiplayer = 0;



//extern World* world;

int main(int argc, char* argv[])
{

    multiplayer = 0;

    /* Initialize SDL Audio */
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        slog("SDL audio initialization failed: %s", SDL_GetError());
        return -1;
    }

    /* Load WAV file */
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;

    if (SDL_LoadWAV("music/main_menu.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
        slog("Failed to load WAV file: %s", SDL_GetError());
        return -1;
    }

    /* Open audio device */
    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (audioDevice == 0) {
        slog("Failed to open audio device: %s", SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        return -1;
    }

    /* Play the sound */
    SDL_QueueAudio(audioDevice, wavBuffer, wavLength);
    SDL_PauseAudioDevice(audioDevice, 0);  // Start playing

    /*variable declarations*/
    const Uint8* keys;
    //Sprite* sprite;
    //World* world;
    //Window* window;

    int mx, my;
    float mf = 0;
    Sprite* mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255, 100, 255, 200);
    Entity* player;
    Entity* monster;
    Entity* fire_wizard;
    Entity* ice_wizard;
    Entity* fast_wizard;
    Entity* melee_wizard;
    Entity* boss_wizard;
    GFC_Vector2D position = gfc_vector2d(100.0f, 500.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(500.0f, 500.0f);
    //GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0, 0, 0, 255),
        0);
    gfc_input_init("config/input.cfg");
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    font_init(1024);
    particle_system_init(1000);
    entity_system_init(1024);
    effect_system_init(1024);
    window_system_init(1024);
    //items_initalize("def/item.def");
    SDL_ShowCursor(SDL_DISABLE);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    camera_set_size(gfc_vector2d(1200, 720));
    SDL_audio_h_
    gfc_config_def_init();
    items_initalize("def/item.def");
    pets_initalize("def/pets.def");

    //gfc_config_def_load("def/spray_particle.def");



    slog("press [escape] to quit");
    /*main game loop*/
    while (!done)
    {
        //slog("%s", world_get_next_world());
        cmds();
        gfc_input_update();
     //   SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //font_cleanup();
        /*update things here*/

        //SDL_GetMouseState(&mx, &my);  // <-- add this
        //if (SDL_GetMouseState(&mx, &my))
        //{
        //    //particles_from_file("def/spray_particle.def", 100, gfc_vector2d(mx, my), gfc_vector2d(1, -1), gfc_vector2d(0, 0.1));


        //}

        //Mix_PlayChannel(1, blaster, 0)
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        //entity_system_think();
        //slog("Here");

        //entity_system_update();
        //entity_system_move();
        switch (process)
        {
        case 1:
            SDL_ClearQueuedAudio(audioDevice);  // Stop current sound
              level_process();
              break;
        case 2:
            level_editor();
            break;  
        default:
              level_main_menu();
              break;
        }

            SDL_GetMouseState(&mx, &my);
            //if (SDL_GetMouseState(&mx, &my))
            //{
            //    particles_from_file("def/spray_particle.def", 100, gfc_vector2d(mx, my), gfc_vector2d(1, -1), gfc_vector2d(0, 0.1));

            //}
        



        /*
        int scene = 0;        
        switch(scene):
        1) level.process();
        */

        game_draw();
        
        //slog("Runnning");
        //gf2d_graphics_clear_screen();// clears drawing buffers
        //// all drawing should happen betweem clear_screen and next_frame
        //    //backgrounds drawn first
        //    //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
        //    
        //    //world_draw(world);
        //   // window_draw(window);
        //    button_system_draw();



        //    //gf2d_draw_rect(rectangle, GFC_COLOR_DARKYELLOW);
        //    //gf2d_draw_rect(player->bounds, GFC_COLOR_RED);
        //    entity_system_draw();
            //UI elements last
            //gf2d_sprite_draw(
            //    mouse,
            //    gfc_vector2d(mx,my),
            //    NULL,
            //    NULL,
            //    NULL,
            //    NULL,
            //    &mouseGFC_Color,
            //    (int)mf);

            //UI_health_bar(gfc_rect(15, 15, 300, 75));
            //slog("%d, %d, %d, %d", get_world_bounds().x, get_world_bounds().y, get_world_bounds().h, get_world_bounds().w);

       // gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            switch (process) {
            case 1:
                level_free();
                setup = 0;
                break;
            case 2:
                level_editor_free();
                break;
            default:
                slog("Nothing assigned, this is default");
            }
            process = 0; // exit condition
            multiplayer = 0;

        }

        if (get_level_transition_flag())
        {
            level_transition();
        }



        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free();
    SDL_CloseAudioDevice(audioDevice);
    SDL_FreeWAV(wavBuffer);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    //entity_free(player);
    //world_free(world);
    //Mix_HaldMusic
    //Mix_FreeMusic
    //Mix
    slog("---==== END ====---");
    return 0;
}

//void parse_args(int argc, char* args[]);

//void parse_args(int argc, char* args[])
//{
//    //if argcount < 2 return
//    // for i = 1; i < argcount i++
//    // gfc_stringlcmp = 0 in an if check some value (drawbounds)  argv sub i
//    // set global to 1
//}

/*eol@eof*/ 


/*
Channel means different things:
Output channels
Mixing channels: (Like background audio)
Input and ouput channels
Mix chunks
Full background music
LoadWAV - Load a WAV file and convert into a mixed chunk handle (Must free) [WAVE, AIFF, RIFF, OGG, VOC)
-1 means idc pick a new channel
*/

/*

Vector2D(x, y* .5% + z-1 *.5)

*/


//// Load menu music
//if (SDL_LoadWAV("music/main_menu.wav", &menuSpec, &menuWavBuffer, &menuWavLength) == NULL) {
//    slog("Failed to load menu WAV: %s", SDL_GetError());
//    return -1;
//}
//
//// Load game music
//if (SDL_LoadWAV("music/game_theme.wav", &gameSpec, &gameWavBuffer, &gameWavLength) == NULL) {
//    slog("Failed to load game WAV: %s", SDL_GetError());
//    SDL_FreeWAV(menuWavBuffer);
//    return -1;
//}
//
//// Open audio device (use menuSpec as default)
//audioDevice = SDL_OpenAudioDevice(NULL, 0, &menuSpec, NULL, 0);
//if (audioDevice == 0) {
//    slog("Failed to open audio device: %s", SDL_GetError());
//    SDL_FreeWAV(menuWavBuffer);
//    SDL_FreeWAV(gameWavBuffer);
//    return -1;
//}
//
//
//
//void play_sound(Uint8* buffer, Uint32 length) {
//    SDL_ClearQueuedAudio(audioDevice);  // Stop current sound
//    SDL_QueueAudio(audioDevice, buffer, length);  // Queue new sound
//    SDL_PauseAudioDevice(audioDevice, 0);  // Play
//}