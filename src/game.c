#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "player.h"
#include "monster.h"
#include "world.h"

Uint8 _DRAWBOUNDS_ = 0;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    World* world;


    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,100,255,200);
    Entity* player;
    Entity* monster;
    GFC_Vector2D position = gfc_vector2d(100.0f, 100.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(500.0f, 100.0f);
    GFC_Rect rectangle = gfc_rect(100, 100, 500, 500);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new_entity(position);
    monster = monster_new_entity(monsterposition);
    world = world_test_new();

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        entity_system_think();
        entity_system_update();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            
            world_draw(world);

            gf2d_draw_rect(rectangle, GFC_COLOR_DARKYELLOW);
            gf2d_draw_rect(player->bounds, GFC_COLOR_RED);
            entity_system_draw();
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}

void parse_args(int argc, char* args[]);

void parse_args(int argc, char* args[])
{
    //if argcount < 2 return
    // for i = 1; i < argcount i++
    // gfc_stringlcmp = 0 in an if check some value (drawbounds)  argv sub i
    // set global to 1
}

/*eol@eof*/ 
