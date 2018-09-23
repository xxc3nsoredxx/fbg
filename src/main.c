#include <stdio.h>

#include "fbg/screen.h"

int main () {
    struct screen_s *scr;

    scr = get_screen();

    printf("Width: %u\n", scr->width);
    printf("Height: %u\n", scr->height);

    clear(scr);

    getchar();

    return 0;
}
