#ifndef WM_H_
#define WM_H_

#include <swc.h>
#include <stdint.h>
#include "types.h"

extern void on_screen_destroy(void* data);

static const struct swc_screen_handler screen_handler = {
	.destroy = on_screen_destroy,
};

extern struct swc_manager manager;
extern struct wm wm;


#endif
