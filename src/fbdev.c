#include "fbdev.h"
#include "hal.h"

u32 *pfb = NULL;
u32 P_WIDTH, P_HEIGHT;  // pysical fb screen
u32 V_WIDTH, V_HEIGHT;  // virtual fb screen
u32 BBP;

u32 DISPLAY_X, DISPLAY_Y; // start point to display the window
u32 BG_COLOR;

unsigned long SCREENSIZE;

int fb_init() {
    int fd, r;

    struct fb_fix_screeninfo finfo = {0};
	struct fb_var_screeninfo vinfo = {0};

    // open /dev/fb
    if ((fd = open(FBDEV, O_RDWR)) < 0) {
        perror("failed to open /dev/fb0");
        return -1;
    }
	printf("open %s success.\n", FBDEV);

    // read fix_screeninfo
    if ((r = ioctl(fd, FBIOGET_FSCREENINFO, &finfo)) < 0) {
		perror("failed to get fix_screeninfo");
		return r;
	}
	printf("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);

    // read var_screeninfo
    if ((r = ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)) < 0) {
		perror("failed to get var_screeninfo");
		return r;
	}
    V_WIDTH = vinfo.xres_virtual;  V_HEIGHT = vinfo.yres_virtual;
    P_WIDTH = vinfo.xres;          P_HEIGHT = vinfo.yres;
    BBP = vinfo.bits_per_pixel;
    SCREENSIZE = V_WIDTH * V_HEIGHT * BBP / 8;

	printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	printf("xoffset = %u, yoffset = %u.\n", vinfo.xoffset, vinfo.yoffset);
	printf("bpp = %u.\n", vinfo.bits_per_pixel);
	printf("screensize = %ld\n", SCREENSIZE);

    // do mem map
    if ((pfb = mmap(NULL, SCREENSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL) {
		perror("failed to mmap");
		return -1;
	}
	printf("pfb = %p.\n", pfb);
    pfb = pfb + vinfo.xoffset + vinfo.yoffset * V_WIDTH;

    // display the fb
    vinfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
    if(ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo) < 0) {
      perror("failed to refresh\n");
      return -1;
    }

    DISPLAY_X = (P_WIDTH  >> 1) - (SCREEN_WIDTH  >> 1);
    DISPLAY_Y = (P_HEIGHT >> 1) - (SCREEN_HEIGHT >> 1);
    printf("display_x: %u, display_y: %u\n", DISPLAY_X, DISPLAY_Y);

    draw_back(0xffffffff);

    return 0;
}

// in fact, r g b should each limit to 8bit
u32 fb_map_rgb(u8 r, u8 g, u8 b) {
    return (0xff << 24) | (r << 16) | (g << 8) | b;
}

void fb_draw_display(u32 *buf) {
    int x, y;
    printf("%08x\n", COLOR_OF_BUF(buf, SCREEN_HEIGHT-1, SCREEN_WIDTH-1));
    printf("max_x:%d  max_y:%d\n", SCREEN_WIDTH+DISPLAY_X, SCREEN_HEIGHT+DISPLAY_Y);
    for (y = 0; y < SCREEN_HEIGHT; ++ y) {
        for (x = 0; x < SCREEN_WIDTH; ++ x) {
            //printf("[x y c]= %u %u %08x\n", x, y, COLOR_OF_BUF(buf,x,y));
            DRAW_PIXEL(DISPLAY_X+x, DISPLAY_Y+y, COLOR_OF_BUF(buf,x,y));
            //DRAW_PIXEL(DISPLAY_X+x, DISPLAY_Y+y, 0xfff0f0f0);
        }
    }
}

void draw_back(u32 color) {
	u32 x, y;
	
	for (y = 0; y < P_HEIGHT; ++ y) {
		for (x = 0; x < P_WIDTH; ++ x) {
			DRAW_PIXEL(x, y, color);
		}
	}
}