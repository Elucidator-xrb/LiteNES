#ifndef __FBDEV_H__
#define __FBDEV_H__

typedef unsigned int  u32;
typedef unsigned char u8;

#define FBDEV "/dev/fb0"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

// define in fbdev.c
extern u32 *pfb_back, *pfb_show;
extern u32 P_WIDTH, P_HEIGHT;  // pysical fb screen
extern u32 V_WIDTH, V_HEIGHT;  // virtual fb screen
extern u32 BBP;

extern u32 DISPLAY_X, DISPLAY_Y; // start point to display the nes window
extern u32 BG_COLOR;

extern unsigned long SCREENSIZE;

#define DRAW_PIXEL(pfb,x,y,c)       *((pfb) + (x) + (y) * V_WIDTH) = (c)
#define DRAW_PIXEL_BACK(x,y,c)  *(pfb_back + (x) + (y) * V_WIDTH) = (c);

int  fb_init();
u32  fb_map_rgb(u8 r, u8 g, u8 b);
void fb_init_background(u32 color);
void fb_flip_display();

#endif