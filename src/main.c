#include <stdio.h>

#include "fbg/screen.h"

int main () {
    struct screen_s *scr;

    scr = get_screen();

    /* Test pixel drawing */
    clear(scr);
    getchar();
    draw_point(scr, COLOR_RED, point(0, 0));
    draw_point(scr, COLOR_GREEN, point(5, 0));
    draw_point(scr, COLOR_BLUE, point(5, 10));
    refresh(scr);
    getchar();

    /* Test line drawing */
    clear(scr);
    refresh(scr);
    /* Horizontal line */
    draw_line(scr, COLOR_RED, point(0,0), point(100,0));
    draw_line(scr, COLOR_GREEN, point(150,2), point(50,2));
    draw_line(scr, COLOR_BLUE, point(100,4), point(200,4));
    /* Vertical line */
    draw_line(scr, COLOR_RED, point(0,50), point(0,150));
    draw_line(scr, COLOR_GREEN, point(2,100), point(2,200));
    draw_line(scr, COLOR_BLUE, point(4,150), point(4,250));
    /* Arbitrary line */
    draw_line(scr, COLOR_RED, point(50,50), point(150,150));
    draw_line(scr, COLOR_GREEN, point(75,50), point(80,150));
    draw_line(scr, COLOR_BLUE, point(100,50), point(110,40));
    refresh(scr);
    getchar();

    /* Test rectangle drawing */
    clear(scr);
    refresh(scr);
    /* Unfilled rectangles */
    draw_rect(scr, COLOR_RED, COLOR_NONE, point(0,0), point(50,50), 0);
    draw_rect(scr, COLOR_GREEN, COLOR_NONE, point(10,10), point(60,60), 0);
    draw_rect(scr, COLOR_BLUE, COLOR_NONE, point(20,20), point(70,70), 0);
    /* Filled rectangles */
    draw_rect(scr, COLOR_RED, COLOR_BLUE, point(0,100), point(50,200), 1);
    draw_rect(scr, COLOR_GREEN, COLOR_RED, point(10,110), point(60,210), 1);
    draw_rect(scr, COLOR_BLUE, COLOR_GREEN, point(20,120), point(70,220), 1);
    refresh(scr);
    getchar();

    /* Test circle drawing */
    clear(scr);
    refresh(scr);
    /* Unfilled circles */
    draw_circle(scr, COLOR_RED, COLOR_NONE, point(100,100), 100, 0);
    draw_circle(scr, COLOR_GREEN, COLOR_NONE, point(125,125), 100, 0);
    draw_circle(scr, COLOR_BLUE, COLOR_NONE, point(150,150), 100, 0);
    /* Filled circles */
    draw_circle(scr, COLOR_RED, COLOR_BLUE, point(200,300), 100, 1);
    draw_circle(scr, COLOR_GREEN, COLOR_RED, point(225,325), 100, 1);
    draw_circle(scr, COLOR_BLUE, COLOR_GREEN, point(250,350), 100, 1);
    refresh(scr);
    getchar();

    clear(scr);
    refresh(scr);

    printf("Width: %u\n", scr->width);
    printf("Height: %u\n", scr->height);
    printf("Line lenght: %lu\n", scr->ll);
    printf("Bits per pixel: %u\n", scr->bpp);
    printf("Memory length: %lu\n", scr->len);

    return 0;
}
