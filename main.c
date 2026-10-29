#include <stdio.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include "config.h"
#include "screen.h"

static struct screen* active_screen;
static struct window* focused_window;
static struct wl_display* display;
static struct wl_event_loop* loop;

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

static void
window_entered(void *data)
{
	struct window *window = data;

	focus(window);
}

static const struct swc_window_handler window_handler = {
	.destroy = &window_destroy,
	.entered = &window_entered,
};

static void
new_screen(struct swc_screen *swc)
{
	struct screen *screen;

	screen = malloc(sizeof(*screen));

	if (!screen)
		return;

	screen->handle = swc;
	screen->num_windows = 0;
	wl_list_init(&screen->windows);
	swc_screen_set_handler(swc, &screen_handler, screen);
	active_screen = screen;
}

static void new_window(struct swc_window* swc) {
        struct window *window;

	window = malloc(sizeof(*window));

	if (!window) {
		fprintf(stderr, "[-] no memory\n");
		return;
	}

	window->handle = swc;
	window->screen = NULL;
	swc_window_set_handler(swc, &window_handler, window);
	swc_window_set_tiled(swc);
	screen_add_window(active_screen, window);
	focus(window);
}

static const struct swc_manager manager = {
	.new_screen = new_screen,
	.new_window = new_window
};


static void
spawn(void *data, uint32_t time, uint32_t value, uint32_t state)
{
	char *const *command = data;

	if (state != WL_KEYBOARD_KEY_STATE_PRESSED)
		return;

	if (fork() == 0) {
		execvp(command[0], command);
		exit(EXIT_FAILURE);
	}
}

static void
quit(void *data, uint32_t time, uint32_t value, uint32_t state)
{
	if (state != WL_KEYBOARD_KEY_STATE_PRESSED)
		return;

	wl_display_terminate(display);
}

int main() {
	const char *socket;

	display = wl_display_create();
	if(!display) {
		fprintf(stderr, "[-] failed to create wayland display! what are you running? something exotic?\n");
		goto err_ret;
	}

	socket = wl_display_add_socket_auto(display);
	if(!socket) {
		fprintf(stderr, "[-] failed to get wayland socket!\n");
		goto destroy_display;
	}

	setenv("WAYLAND_DISPLAY", socket, 1);

	if(!swc_initialize(display, NULL, &manager)) {
		fprintf(stderr, "[-] failed to initialize swc library!\n");
		goto destroy_display;
	}


	swc_add_binding(SWC_BINDING_KEY, SWC_MOD_LOGO, XKB_KEY_t,
	                &spawn, terminal_command);

	loop = wl_display_get_event_loop(display);

	wl_display_run(display);


	swc_finalize();
	wl_display_destroy(display);
	return 0;

destroy_display:
	wl_display_destroy(display);
err_ret:
	return -1;
}
