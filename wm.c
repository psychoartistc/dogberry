#include "wm.h"

#include <swc.h>
#include <wayland-server.h>
#include <wayland-util.h>
#include <xkbcommon/xkbcommon-keysyms.h>

extern void new_screen(struct swc_screen* scr);
extern void new_window(struct swc_window* win);
extern void new_device(struct libinput_device* dev);

struct swc_manager manager = {
	.new_screen = new_screen,
	.new_window = new_window,
	.new_device = new_device};

struct wm wm = {0};
