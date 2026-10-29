#include <stdio.h>
#include <wayland-server.h>
#include <swc.h>

static void new_screen(struct swc_screen* scr) {
	printf("screen here!\n");
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
	struct wl_display* display;
	
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
