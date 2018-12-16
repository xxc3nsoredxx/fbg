#include <stdio.h>

#include "fbg/screen.h"

int main () {
    struct screen_s *scr;

    scr = get_screen();

    /* Test pixel drawing */
    clear(scr);
    getchar();

    draw_pixel(scr, COLOR(0xFF,0x00,0x00), point(0, 0));
    draw_pixel(scr, COLOR(0x00,0xFF,0x00), point(5, 0));
    draw_pixel(scr, COLOR(0x00,0x00,0xFF), point(5, 10));
    refresh(scr);
    getchar();

    /* Test line drawing */
    clear(scr);
    refresh(scr);

    /* Horizontal line */
    draw_line(scr, COLOR(0xFF,0x00,0x00), point(0,0), point(100,0));
    draw_line(scr, COLOR(0x00,0xFF,0x00), point(150,2), point(50,2));
    draw_line(scr, COLOR(0x00,0x00,0xFF), point(100,4), point(200,4));
    /* Vertical line */
    draw_line(scr, COLOR(0xFF,0x00,0x00), point(0,50), point(0,150));
    draw_line(scr, COLOR(0x00,0xFF,0x00), point(2,100), point(2,200));
    draw_line(scr, COLOR(0x00,0x00,0xFF), point(4,150), point(4,250));
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
