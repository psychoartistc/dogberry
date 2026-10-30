CC := c99
LD := c99

PKG_CONFIG := pkgconf

PKGS := swc wayland-server xkbcommon

PKG_CFLAGS  := $(shell $(PKG_CONFIG) --cflags $(PKGS))
PKG_LDFLAGS := $(shell $(PKG_CONFIG) --libs $(PKGS))

CFLAGS := -O2 -Wextra $(PKG_CFLAGS)
LDFLAGS := $(PKG_LDFLAGS)

TARGET := dogberry
SRCS := main.c wm.c
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

