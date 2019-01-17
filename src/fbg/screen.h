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
/* Basic colors */
#define COLOR_NONE  0
#define COLOR_RED   COLOR(0xFF,0x00,0x00)
#define COLOR_GREEN COLOR(0x00,0xFF,0x00)
#define COLOR_BLUE  COLOR(0x00,0x00,0xFF)

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
 * Draw a point to the screen at location (x,y)
 * s is the pointer to the screen
 * if s is NULL use global screen
 * color is an integer with RGB
 */
void draw_point (struct screen_s *s, unsigned int color, struct point_s p);

/*
 * Gets the color at the specified point
 */
unsigned int get_point (struct screen_s *s, struct point_s point);

/*
 * Draws a line between p1 amd p2
 */
void draw_line (struct screen_s *s, unsigned int color,
                struct point_s p1, struct point_s p2);

/*
 * Draws a rectangle given the line color, fill color, and
 * top left/bottom right corners
 * Only fill rectangle if fill is set
 */
void draw_rect (struct screen_s *s, unsigned int l_color, unsigned int f_color,
                struct point_s tl, struct point_s br, char fill);

/*
 * Draws a circle given the center, radius, line color, and fill color
 * Only fill circle if fill is set
 */
void draw_circle (struct screen_s *s, unsigned int l_color,
                    unsigned int f_color, struct point_s c,
                    unsigned int radius, char fill);

/*
 * Draw arbitrary polygons by supplying an array of points (drawn in order)
 * The last point automatically gets connected to the first
 */
void draw_poly (struct screen_s *s, unsigned int l_color, unsigned int f_color,
                struct point_s *points, size_t len, char fill);

#endif
