#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "screen.h"

static struct screen* active_screen;
static struct window* focused_window;
static struct wl_display* display;
static struct wl_event_loop* loop;

static void new_screen(struct swc_screen* scr) {
	printf("screen here!\n");
}

static void screen_add_window(struct screen* screen, struct window* wnd) {
	wnd->screen = screen;
	wl_list_insert(&screen->windows, &wnd->link);
	screen->num_windows++;
	swc_window_show(wnd->handle);
}

static void screen_remove_window(struct screen* screen, struct window* wnd) {
	wnd->screen = NULL;
	wl_list_remove(&wnd->link);
	screen->num_windows--;
	swc_window_hide(wnd->handle);
}

static void screen_entered(void *data)
{
	struct screen *screen = data;
	active_screen = screen;
	/* duh */
}


static void screen_usable_geometry_changed(void *data)
{
	struct screen *screen = (struct screen*)data;
	/* TODO: resize here */
}

static const struct swc_screen_handler screen_handler = {
	.entered = &screen_entered,
};

static void
focus(struct window *window)
{
	if (focused_window) {
		swc_window_set_border(focused_window->handle,
		                     BORDER_COLOR_INACTIVE, BORDER_WIDTH_PX);
	}

	if (window) {
		swc_window_set_border(window->handle, BORDER_COLOR_ACTIVE, BORDER_WIDTH_PX );
		swc_window_focus(window->handle);
	} else
		swc_window_focus(NULL);

	focused_window = window;
}


static void
window_destroy(void *data)
{
	struct window *window = data, *next_focus;

	if (focused_window == window) {
		next_focus = wl_container_of(window->link.next, window, link);
		if (&next_focus->link == &window->screen->windows) {
			next_focus = wl_container_of(window->link.prev,
			                             window, link);

			if (&next_focus->link == &window->screen->windows)
				next_focus = NULL;
		}

		focus(next_focus);
	}

	screen_remove_window(window->screen, window);
	free(window);
}

static void new_window(struct swc_window* window) {
	printf("new window!\n");
	swc_window_set_stacked(window);
	swc_window_show(window);
	swc_window_focus(window);
}

static const struct swc_manager manager = {
	.new_screen = new_screen,
	.new_window = new_window
};

int main() {
	display = wl_display_create();
	if(!display) {
		fprintf(stderr, "[-] failed to create wayland display! what are you running? something exotic?\n");
		goto err_ret;
	}

	if(!swc_initialize(display, NULL, &manager)) {
		fprintf(stderr, "[-] failed to initialize swc library!\n");
		goto destroy_display;
	}

	printf("we are running\n");

	wl_display_run(display);


	swc_finalize();
	wl_display_destroy(display);
	return 0;

destroy_display:
	wl_display_destroy(display);
err_ret:
	return -1;
}
