#ifndef PSV2CTRL_DISP_H
#define PSV2CTRL_DISP_H

#include <stdint.h>

void
InitializeDisplay(void);

void
WriteString(int x, int y, uint32_t fg, float sz, const char *str, ...);

void
SetClearColor(uint32_t color);

void
ClearScreen(void);

void
StartDrawing(void);

void
Flip(void);

#endif