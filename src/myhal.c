/*
This file presents all abstractions needed to port LiteNES.
  (The current working implementation uses allegro library.)

To port this project, replace the following functions by your own:
1) nes_hal_init()
    Do essential initialization work, including starting a FPS HZ timer.

2) nes_set_bg_color(c)
    Set the back ground color to be the NES internal color code c.

3) nes_flush_buf(*buf)
    Flush the entire pixel buf's data to frame buffer.

4) nes_flip_display()
    Fill the screen with previously set background color, and
    display all contents in the frame buffer.

5) wait_for_frame()
    Implement it to make the following code is executed FPS times a second:
        while (1) {
            wait_for_frame();
            do_something();
        }

6) int nes_key_state(int b) 
    Query button b's state (1 to be pressed, otherwise 0).
    The correspondence of b and the buttons:
      0 - Power
      1 - A
      2 - B
      3 - SELECT
      4 - START
      5 - UP
      6 - DOWN
      7 - LEFT
      8 - RIGHT
*/
#include "hal.h"

#if (SELF_IMPL == 1)

#include "fce.h"
#include "common.h"
#include "fbdev.h"
//#include <allegro5/allegro.h>
//#include <allegro5/allegro_primitives.h>

// static u32 vtx[100000]; // let us guess it is used for 256*240 screen
u32 color_map[64];
// int vtx_sz = 0;         // seemed useless now

// 在这里更新键位状态
uint8_t key_state[9];
void update_key_state()
{
    return;
}
/* Wait until next allegro timer event is fired. */
void wait_for_frame()
{
    int i = 10000000;
    while (i--);
    // while (1)
    // {
    //     ALLEGRO_EVENT event;
    //     al_wait_for_event(fce_event_queue, &event);
    //     if (event.type == ALLEGRO_EVENT_TIMER) break;
    // }
}

/* Set background color. RGB value of c is defined in fce.h */
void nes_set_bg_color(int c)
{
    printf("start nes_set_bg_color...  ");
    int x, y;
    BG_COLOR = color_map[c];
    for (y = 0; y < SCREEN_HEIGHT; ++ y) { 
        for (x = 0; x < SCREEN_WIDTH; ++x) {
            // DRAW_BUF(vtx, x, y, BG_COLOR);
            DRAW_PIXEL_BACK(x, y, BG_COLOR);
        }
    }
    // fill vtx buf with BG_COLOR
    printf("finish nes_set_bg_color\n");
}

/* Flush the pixel buffer */
void nes_flush_buf(PixelBuf *buf) 
{
    printf("start nes_flush_buf...  ");
    int i;
    u32 x, y, c;
    Pixel *p;
    for (i = 0; i < buf->size; ++ i) {
        p = &(buf->buf[i]);
        x = (p->x) & 0xff; 
        y = p->y;
        c = color_map[p->c];

        DRAW_PIXEL_BACK(x  , y  , c);
        // DRAW_PIXEL_BACK(x+1, y  , c);
        // DRAW_PIXEL_BACK(x  , y+1, c);
        // DRAW_PIXEL_BACK(x+1, y+1, c);
        // DRAW_BUF(vtx, x  , y, c);
        // DRAW_BUF(vtx, x+1, y, c);
        // DRAW_BUF(vtx, x  , y+1, c);
        // DRAW_BUF(vtx, x+1, y+1, c);
    }
    printf("finsh nes_flush_buf\n");
}

/* Initialization:
   (1) start a 1/FPS Hz timer. 
   (2) register fce_timer handle on each timer event */
void nes_hal_init()
{
    int i;

    fb_init();

    for (i = 0; i < 64; i ++) {
        pal color = palette[i];
        color_map[i] = fb_map_rgb(color.r, color.g, color.b);
    }

    printf("finsh nes_hal_init\n");
    // fce_timer = al_create_timer(1.0 / FPS);
    // fce_event_queue = al_create_event_queue();
    // al_register_event_source(fce_event_queue, al_get_timer_event_source(fce_timer));
    // al_start_timer(fce_timer);
}

/* Update screen at FPS rate by allegro's drawing function. 
   Timer ensures this function is called FPS times a second. */
void nes_flip_display()
{
    printf("start nes_flip_display...  ");
    int x, y;
    // fb_draw_display(vtx);
    fb_flip_display();
    for (y = 0; y < SCREEN_HEIGHT; ++ y) { 
        for (x = 0; x < SCREEN_WIDTH; ++x) {
            // DRAW_BUF(vtx, x, y, BG_COLOR);
            DRAW_PIXEL_BACK(x, y, BG_COLOR);
        }
    }
    printf("finish nes_flush_buf\n");
}

/* Query a button's state.
   Returns 1 if button #b is pressed. */
int nes_key_state(int b)
{
    // 仅仅只有9个键，计划使用两个8 1byte数拼接而成。
    // 为了与ascii控制码兼容，使用 224 - 255 之间的数字进行编码，便于位运算。
    // 选取
    switch (b)
    {
        case 0: // On / Off
            return 1;
        case 1: // A
            return 0; // al_key_down(&state, ALLEGRO_KEY_K);
        case 2: // B
            return 0; // al_key_down(&state, ALLEGRO_KEY_J);
        case 3: // SELECT
            return 0; // al_key_down(&state, ALLEGRO_KEY_U);
        case 4: // START
            return 0; // al_key_down(&state, ALLEGRO_KEY_I);
        case 5: // UP
            return 0; // al_key_down(&state, ALLEGRO_KEY_W);
        case 6: // DOWN
            return 0; // al_key_down(&state, ALLEGRO_KEY_S);
        case 7: // LEFT
            return 0; // al_key_down(&state, ALLEGRO_KEY_A);
        case 8: // RIGHT
            return 0; // al_key_down(&state, ALLEGRO_KEY_D);
        default:
            return 1;
    }
}

#endif