#ifndef SCREEN_H_20180821_172135
#define SCREEN_H_20180821_172135

/* ANSI control */
#define STDOUT 1
#define CSI "\x1B["
#define CURS_ON "?25h"
#define CURS_OFF "?25l"
#define CLEAR "2J"
#define X_Y(X,Y) "" #Y ";" #X "H"
#define ANSI(A) do { \
                    write(STDOUT, (CSI), strlen((CSI))); \
                    write(STDOUT, (A), strlen((A))); \
                } while (0)

/*
 * Creates an int representing the RGB color
 */
#define COLOR(R,G,B) (0 | ((R) << 16) | ((G) << 8) | (B))

struct screen_s {
    unsigned int width;
    unsigned int height;
    size_t ll;
    unsigned int bpp;
    size_t len;
    unsigned int *fb;
};

struct point_s {
    unsigned int x;
    unsigned int y;
};

/*
 * Creates a point from (x,y)
 */
struct point_s point (unsigned int x, unsigned int y);

/*
 * Return a pointer to the screen
 */
struct screen_s* get_screen (void);

/*
 * Draw the back buffer to the screen
 * If s is NULL, use global screen
 */
void refresh (struct screen_s *s);

/*
 * Clear the screen object
 * If s is NULL, clear global screen
 */
void clear (struct screen_s *s);

/*
 * Draw a pixel to the screen at location (x,y)
 * s is the pointer to the screen
 * if s is NULL use global screen
 * color is an integer with RGB
 */
void draw_pixel (struct screen_s *s, unsigned int color, struct point_s p);

/*
 * Draws a line between p1 amd p2
 */
void draw_line (struct screen_s *s, unsigned int color,
                struct point_s p1, struct point_s p2);

#endif
