#include <stdio.h>

#include "fbg/screen.h"

int main () {
    struct screen_s *scr;
    struct point_s three [3];
    struct point_s four [4];
    struct point_s five [5];

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
    draw_circle(scr, COLOR_RED, COLOR_BLUE, point(200,350), 100, 1);
    draw_circle(scr, COLOR_GREEN, COLOR_RED, point(400,400), 100, 1);
    draw_circle(scr, COLOR_BLUE, COLOR_GREEN, point(600,450), 100, 1);
    refresh(scr);
    getchar();

    /* Test polygon drawing */
    clear(scr);
    refresh(scr);
    /* Unfilled 3-sided shape */
    three[0] = point(10,10);
    three[1] = point(10,100);
    three[2] = point(100,100);
    draw_poly(scr, COLOR_RED, COLOR_NONE, three,
                sizeof(three)/sizeof(*three), 0);
    /* Unfilled 4-sided shape */
    four[0] = point(150,30);
    four[1] = point(120,100);
    four[2] = point(150,50);
    four[3] = point(200,75);
    draw_poly(scr, COLOR_GREEN, COLOR_NONE, four,
                sizeof(four)/sizeof(*four), 0);
    /* Unfilled 5-sided shape */
    five[0] = point(250,100);
    five[1] = point(300,100);
    five[2] = point(300,50);
    five[3] = point(275,25);
    five[4] = point(250,50);
    draw_poly(scr, COLOR_BLUE, COLOR_NONE, five,
                sizeof(five)/sizeof(*five), 0);
    /* Filled 3-sided shape */
    three[0] = point(10,210);
    three[1] = point(10,300);
    three[2] = point(100,300);
    draw_poly(scr, COLOR_RED, COLOR_GREEN, three,
                sizeof(three)/sizeof(*three), 1);
    /* Filled 4-sided shape */
    four[0] = point(150,230);
    four[1] = point(120,300);
    four[2] = point(150,250);
    four[3] = point(200,275);
    draw_poly(scr, COLOR_GREEN, COLOR_BLUE, four,
                sizeof(four)/sizeof(*four), 1);
    /* Filled 5-sided shape */
    five[0] = point(250,300);
    five[1] = point(300,300);
    five[2] = point(300,250);
    five[3] = point(275,225);
    five[4] = point(250,250);
    draw_poly(scr, COLOR_BLUE, COLOR_RED, five,
                sizeof(five)/sizeof(*five), 1);
    refresh(scr);
    getchar();

    /* Print screen info at the end */
    clear(scr);
    refresh(scr);
    printf("Width: %u\n", scr->width);
    printf("Height: %u\n", scr->height);
    printf("Line lenght: %lu\n", scr->ll);
    printf("Bits per pixel: %u\n", scr->bpp);
    printf("Memory length: %lu\n", scr->len);

    return 0;
}
