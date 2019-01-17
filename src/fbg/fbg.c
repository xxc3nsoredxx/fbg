#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "screen.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define ERR(MSG) do { \
                     write(STDERR,MSG,strlen(MSG)); \
                 } while (0)

/* Toggles a variable */
#define TOGGLE(X) X = ((X) ? 0 : 1)
/* Gets the minimum */
#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))
/* Gets the maximum */
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))

int fb_file;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
struct screen_s g_scr;
unsigned int *fb2;
unsigned int *fb_real;

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
    fb_real = mmap(0, finfo.smem_len, PROT_READ|PROT_WRITE,
                   MAP_SHARED, fb_file, 0);
    if ((long)fb_real == (long)MAP_FAILED) {
        ERR("Error mmapping framebuffer!\n");
    }
    /* Create the back buffer */
    fb2 = calloc(1, finfo.smem_len);
    if (!fb2) {
        ERR("Error creating back buffer!\n");
    }

    /* Setup global screen info */
    g_scr.width = vinfo.xres;
    g_scr.height = vinfo.yres;
    g_scr.ll = finfo.line_length;
    g_scr.bpp = vinfo.bits_per_pixel;
    g_scr.len = finfo.smem_len;
    g_scr.fb = fb2;

    ANSI(CURS_OFF);
}

void __attribute__ ((destructor)) exit_lib (void) {
    /* Unmap framebuffer if mapped */
    if (fb_real) {
        munmap(fb_real, g_scr.len);
    }
    if (fb2) {
        free(fb2);
    }
    /* Close framebuffer if open */
    if (fb_file > 0) {
        close(fb_file);
    }

    ANSI(CURS_ON);
}

struct screen_s* get_screen (void) {
    return &g_scr;
}

void refresh (struct screen_s *s) {
    if (!s) {
        s = &g_scr;
    }

    memcpy(fb_real, g_scr.fb, g_scr.len);
}

void clear (struct screen_s* s) {
    if (!s) {
        s = &g_scr;
    }
    memset(s->fb, 0, s->len);
    ANSI(CLEAR);
    ANSI(X_Y(0,0));
    refresh(s);
}

struct point_s point (unsigned int x, unsigned int y) {
    struct point_s ret;

    ret.x = x;
    ret.y = y;

    return ret;
}

void draw_point (struct screen_s *s, unsigned int color, struct point_s p) {
    size_t pos;
    unsigned int x = p.x;
    unsigned int y = p.y;

    if (!s) {
        s = &g_scr;
    }

    /* Test bounds */
    if (x >= s->width || y >= s->height) {
        return;
    }

    pos = (y * (s->ll / (s->bpp / 8))) + x;

    *(s->fb + pos) = color;
}

unsigned int get_point (struct screen_s *s, struct point_s point) {
    size_t pos;
    unsigned int x = point.x;
    unsigned int y = point.y;

    if (!s) {
        s = &g_scr;
    }

    if (x >= s->width || y >= s->height) {
        return 0;
    }

    pos = (y * (s->ll / (s->bpp / 8))) + x;

    return *(s->fb + pos);
}

void draw_line (struct screen_s *s, unsigned int color,
                struct point_s p1, struct point_s p2) {
    struct point_s cur;
    struct point_s target;
    int deltax;
    int deltay;
    int delta;
    int inc;

    if (!s) {
        s = &g_scr;
    }

    /* Draw start- and endpoints */
    draw_point(s, color, p1);
    draw_point(s, color, p2);

    /* Horizontal line */
    if (p1.y == p2.y) {
        if (p1.x < p2.x) {
            cur = p1;
            target = p2;
        } else {
            cur = p2;
            target = p1;
        }

        for (; cur.x < target.x; cur.x++) {
            draw_point(s, color, cur);
        }
    }

    /* Vertical line */
    else if (p1.x == p2.x) {
        if (p1.y < p2.y) {
            cur = p1;
            target = p2;
        } else {
            cur = p2;
            target = p1;
        }

        for (; cur.y < target.y; cur.y++) {
            draw_point(s, color, cur);
        }
    }

    /* Arbitrary line, using Bresenham's Algorithm */
    else {
        /* Low gradient */
        if (abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
            if (p1.x < p2.x) {
                cur = p1;
                target = p2;
            } else {
                cur = p2;
                target = p1;
            }

            deltax = target.x - cur.x;
            deltay = target.y - cur.y;
            inc = 1;
            if (deltay < 0) {
                inc = -1;
                deltay = -deltay;
            }
            delta = 2*deltay - deltax;

            for (; cur.x < target.x; cur.x++) {
                draw_point(s, color, cur);
                if (delta > 0) {
                    cur.y += inc;
                    delta -= 2*deltax;
                }
                delta += 2*deltay;
            }
        }

        /* High gradient */
        else {
            if (p1.y < p2.y) {
                cur = p1;
                target = p2;
            } else {
                cur = p2;
                target = p1;
            }

            deltax = target.x - cur.x;
            deltay = target.y - cur.y;
            inc = 1;
            if (deltax < 0) {
                inc = -1;
                deltax = -deltax;
            }
            delta = 2*deltax - deltay;

            for (; cur.y < target.y; cur.y++) {
                draw_point(s, color, cur);
                if (delta > 0) {
                    cur.x += inc;
                    delta -= 2*deltay;
                }
                delta += 2*deltax;
            }
        }
    }
}

void draw_rect (struct screen_s *s, unsigned int l_color, unsigned int f_color,
                struct point_s tl, struct point_s br, char fill) {
    /* The other two corners */
    struct point_s tr;
    struct point_s bl;

    if (!s) {
        s = &g_scr;
    }

    tr.x = br.x;
    tr.y = tl.y;
    bl.x = tl.x;
    bl.y = br.y;

    draw_line(s, l_color, tl, tr);
    draw_line(s, l_color, tr, br);
    draw_line(s, l_color, br, bl);
    draw_line(s, l_color, bl, tl);

    if (fill) {
        /* Move points inward by one */
        tl.x++;
        tl.y++;
        tr.x--;
        tr.y++;
        for (; tl.y < bl.y; tl.y++, tr.y++) {
            draw_line(s, f_color, tl, tr);
        }
    }
}

void draw_circle (struct screen_s *s, unsigned int l_color,
                    unsigned int f_color, struct point_s c,
                    unsigned int radius, char fill) {
    struct point_s cur;
    int deltax;
    int deltay;
    int err;

    if (!s) {
        s = &g_scr;
    }

    /* Midpoint circle algorithm */
    cur.x = radius;
    cur.y = 0;
    deltax = 1;
    deltay = 1;
    err = deltax - (radius << 1);

    while (cur.x >= cur.y) {
        /* Draw the first octant, fiddle with coordinates to get the others */
        draw_point(s, l_color, point(c.x + cur.x, c.y + cur.y));
        draw_point(s, l_color, point(c.x + cur.y, c.y + cur.x));
        draw_point(s, l_color, point(c.x - cur.x, c.y + cur.y));
        draw_point(s, l_color, point(c.x - cur.y, c.y + cur.x));
        draw_point(s, l_color, point(c.x + cur.x, c.y - cur.y));
        draw_point(s, l_color, point(c.x + cur.y, c.y - cur.x));
        draw_point(s, l_color, point(c.x - cur.x, c.y - cur.y));
        draw_point(s, l_color, point(c.x - cur.y, c.y - cur.x));

        /* Fill the inside */
        if (fill) {
            draw_line(s, f_color, point(c.x + cur.x - 1, c.y + cur.y),
                                  point(c.x + cur.x - 1, c.y - cur.y));
            draw_line(s, f_color, point(c.x + cur.y, c.y + cur.x - 1),
                                  point(c.x + cur.y, c.y - cur.x + 1));
            draw_line(s, f_color, point(c.x - cur.x + 1, c.y + cur.y),
                                  point(c.x - cur.x + 1, c.y - cur.y));
            draw_line(s, f_color, point(c.x - cur.y, c.y + cur.x - 1),
                                  point(c.x - cur.y, c.y - cur.x + 1));
        }

        if (err <= 0) {
            cur.y++;
            err += deltay;
            deltay += 2;
        }

        if (err > 0) {
            cur.x--;
            deltax += 2;
            err += deltax - (radius << 1);
        }
    }
}

void draw_poly (struct screen_s *s, unsigned int l_color, unsigned int f_color,
                struct point_s *points, size_t len, char fill) {
    size_t cx;
    struct screen_s canvas;
    struct point_s p;
    char filling = 0;
    struct point_s tl;
    struct point_s br;

    if (!s) {
        s = &g_scr;
    }

    /* Do nothing if points is NULL or length given as empty */
    if (!points || !len) {
        return;
    }

    /* Copy screen attributes */
    canvas.width = s->width;
    canvas.height = s->height;
    canvas.ll = s->ll;
    canvas.bpp = s->bpp;
    canvas.len = s->len;
    canvas.fb = calloc(canvas.len, 1);

    tl = points[0];
    br = tl;

    /* Draw polygon edges to the canvas */
    for (cx = 0; cx < len - 1; cx++) {
        draw_line(&canvas, l_color, points[cx], points[cx + 1]);

        /* Bounds */
        tl.x = MIN(tl.x, points[cx].x);
        tl.y = MIN(tl.y, points[cx].y);
        br.x = MAX(br.x, points[cx].x);
        br.y = MAX(br.y, points[cx].y);
    }
    draw_line(&canvas, l_color, points[len - 1], points[0]);
    tl.x = MIN(tl.x, points[len - 1].x);
    tl.y = MIN(tl.y, points[len - 1].y);
    br.x = MAX(br.x, points[len - 1].x);
    br.y = MAX(br.y, points[len - 1].y);

    /* Copy canvas to screen */
    p = tl;
    br.x++;
    br.y++;
    for (; p.y < br.y; p.y++) {
        unsigned int num = 0;
        filling = 0;

        /* First, count the number of polygon edge points in row */
        for (; fill && p.x < br.x; p.x++) {
            unsigned int c = get_point(&canvas, p);
            unsigned int prev = get_point(&canvas, point(p.x - 1, p.y));

            if (c == l_color && prev != l_color) {
                num++;
            }
        }

        for (p.x = tl.x; p.x < br.x; p.x++) {
            unsigned int c = get_point(&canvas, p);
            unsigned int prev = get_point(&canvas, point(p.x - 1, p.y));

            if (c) {
                draw_point(s, c, p);
                continue;
            } else if (prev == l_color && (num > 1)) {
                TOGGLE(filling);
            }

            /* Fill the polygons line by line */
            if (fill && filling) {
                draw_point(s, f_color, p);
            }
        }
    }

    free(canvas.fb);
}
