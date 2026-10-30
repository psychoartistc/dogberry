#include "types.h"
#include "wm.h"

#include <stdio.h>

#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>

void on_screen_destroy(void* data) {
	struct screen* s = (struct screen*)data;

	if(!s) {
		fprintf(stderr, "[-] on_screen_destroy bad arg\n");
		die();
	}

	if(wm.sel_screen == s) {
		if(wl_list_empty(&wm.screens))
			wm.sel_screen = NULL;
		else 
			wm.sel_screen = wl_container_of(wm.screens.next, wm.sel_screen, link);
	}

	free(s);
}

static void on_screen(struct swc_screen* handle) {
	struct screen* s = malloc(sizeof(struct screen));
	if(!s) {
		fprintf(stderr, "[-] no memory\n");
		die();
	}

	s->scr = handle;
	s->x = 0;
	s->y = 0;
	s->w = 0;
	s->h = 0;

	wl_list_insert(&wm.screens, &s->link);

	/* set the new screen as the selected one */
	if(!wm.sel_screen)
		wm.sel_screen = s;

	swc_screen_set_handler(handle, &screen_handler, s);
}

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

	wl_display_run(wm.display);
	swc_finalize();

	wl_display_destroy(wm.display);
	return -1;
}
