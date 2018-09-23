#ifndef SCREEN_H_20180821_172135
#define SCREEN_H_20180821_172135

#include <linux/fb.h>

struct screen_s {
    unsigned int width;
    unsigned int height;
    unsigned int *fb;
};

/* Return a pointer to the screen */
struct screen_s* get_screen (void);

/*
 * Clear the screen object
 * If s is NULL, clear global screen
 */
void clear (struct screen_s* s);

#endif
