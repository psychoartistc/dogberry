#include "utils.h"
#include "wm.h"

noreturn void die() {
	fprintf(stderr, "died!\n");
	wl_display_destroy(wm.display);
	exit(-1);
}
