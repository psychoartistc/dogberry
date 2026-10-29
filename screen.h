#ifndef SCREEN_H_
#define SCREEN_H_

#include <swc.h>
#include <xkbcommon/xkbcommon.h>
#include <wayland-server.h>

struct screen {
	struct swc_screen* handle;
	struct wl_list windows;
	unsigned int num_windows;
};

struct window {
	struct swc_window* swc;
	struct screen* screen;
	struct wl_list link;
};

#endif
