#include "display.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vita2d.h>

static vita2d_pgf *font;

void
InitializeDisplay(void) {
    vita2d_init();
    vita2d_set_clear_color(0);
    font = vita2d_load_default_pgf();
}

void
StartDrawing(void){
    vita2d_start_drawing();
}

void
Flip(void){
    vita2d_end_drawing();
    vita2d_wait_rendering_done();
    vita2d_swap_buffers();
}

void
SetClearColor(uint32_t color){
    vita2d_set_clear_color(color);
}

void
ClearScreen(){
    vita2d_clear_screen();
}

void
WriteString(int x, int y, uint32_t fg, float sz, const char *str, ...) {
    va_list l;
    va_start(l, str);

    char * a = malloc(vsnprintf(NULL, 0, str, l) + 1);
    vsprintf(a, str, l);
    vita2d_pgf_draw_text(font, x, y, fg, sz, a);

    va_end(l);
}