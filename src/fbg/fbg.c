#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include "screen.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define ERR(MSG) (write(STDERR,MSG,strlen(MSG)))

int fb_file;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
struct screen_s g_scr;
unsigned int *fb;

void __attribute__ ((constructor)) init_lib (void) {
    /* Attempt to open framebuffer */
    fb_file = open("/dev/fb0", O_RDWR);
    if (fb_file == -1) {
        ERR("Error opening framebuffer!\n");
    }

    /* Attempt to get screen info */
    if (ioctl(fb_file, FBIOGET_VSCREENINFO, &vinfo)) {
        ERR("Error getting var screeninfo!\n");
    }
    if (ioctl(fb_file, FBIOGET_FSCREENINFO, &finfo)) {
        ERR("Error getting fix screeninfo!\n");
    }

    /* Attempt to map framebuffer to memory */
    fb = mmap(0, finfo.smem_len, PROT_READ|PROT_WRITE,
                 MAP_SHARED, fb_file, 0);
    if ((long)fb == (long)MAP_FAILED) {
        ERR("Error mmapping framebuffer!\n");
    }

    g_scr.width = vinfo.xres;
    g_scr.height = vinfo.yres;
    g_scr.fb = fb;
}

void __attribute__ ((destructor)) exit_lib (void) {
    /* Unmap framebuffer if mapped */
    if (fb) {
        munmap(fb, finfo.smem_len);
    }
    /* Close framebuffer if open */
    if (fb_file > 0) {
        close(fb_file);
    }
}

struct screen_s* get_screen (void) {
    return &g_scr;
}

void clear (struct screen_s* s) {
    if (!s) {
        s = &g_scr;
    }
    memset(s->fb, 0x0, finfo.smem_len);
}
