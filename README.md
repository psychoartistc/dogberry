# dogberry
a small, dumb wayland compositor (WIP)

![Logo](logo.jpg)

## building
dogberry depends on [neuwld](https://git.sr.ht/~shrub900/neuwld), [neuswc](https://git.sr.ht/~shrub900/neuswc), wayland, wayland-scanner, libinput (for linux, wscons for bsd) and some dependencies of neuwld and neuswc libraries
```
# install and build neuwld and neuswc, they may not be in your package manager
# make sure these libraries can be detected with pkgconf
git clone https://github.com/psychoartistc/dogberry
cd dogberry
make
```
running: 
```
# from the source directory in a tty
swc-launch dogberry
```

## compatibility
the compositor itself doesnt have any platform specific code other than basic posix stuff (unistd.h for example, this header needs to be and usually is on your system)  
neuswc and neuwld libraries have been tested on linux and bsd systems, plan9 with wl9 have not been tested
