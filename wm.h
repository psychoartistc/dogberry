#ifndef WM_H_
#define WM_H_

#include <stdint.h>
#include "types.h"

void focus_next(void* data, uint32_t time, uint32_t val, uint32_t state);
void focus_prev(void* data, uint32_t time, uint32_t val, uint32_t state);
void kill_selected(void* data, uint32_t time, uint32_t val, uint32_t state);

void mouse_move(void* data, uint32_t time, uint32_t val, uint32_t state);
void mouse_resize(void* data, uint32_t time, uint32_t val, uint32_t state);
void fullscreen(void* data, uint32_t time, uint32_t val, uint32_t state);

void new_screen(struct swc_screen* screen);
void new_window(struct swc_window* wn);
void new_device(struct libinput_device* dev);

void quit(void* data, uint32_t time, uint32_t value, uint32_t state);
void spawn(void* data, uint32_t time, uint32_t value, uint32_t state);
void workspace_goto(void* data, uint32_t time, uint32_t value, uint32_t state);
void workspace_moveto(void* data, uint32_t time, uint32_t value, uint32_t state);

extern struct wm wm;

#endif
