#include "types.h"
#include "wm.h"

#include <stdio.h>

#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>

static const struct swc_manager manager = {};

int main(char** argv, int argc) {
	wm.display = wl_display_create();
	if(!wm.display) {
		fprintf(stderr, "[-] wl_display_create failed! are you running something exotic?\n");
		return -1;
	}

	wl_list_init(&wm.screens);
	wl_list_init(&wm.clients);
	
	wm.sel_client = NULL;
	wm.sel_screen = NULL;
	wm.grab.active = false;
	wm.grab.resize = false;
	wm.grab.c = NULL;
	wm.ws = 1;
	
	wm.ev_loop = wl_display_get_event_loop(wm.display);
	if(!swc_initialize(wm.display, wm.ev_loop, &manager)) {
		fprintf(stderr, "[-] swc_initialize failed!");
		wl_display_destroy(wm.display);
		return -1;
	}

	const char* sock = wl_display_add_socket_auto(wm.display);
	if(!sock) { 
		fprintf(stderr, "[-] wl_display_add_socket_auto failed!");
		wl_display_destroy(wm.display);
		return -1;
	}

	setenv("WAYLAND_DISPLAY", sock, 1);

	printf("[+] initialized");
}
