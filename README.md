# dogberry
a small, dumb wayland compositor (WIP)

![Logo](logo.jpg)

## building
dogberry depends on [wld](https://github.com/michaelforney/wld), [swc](https://github.com/michaelforney/swc), wayland, wayland-scanner and some dependencies of those libraries
```
# install and build wld, swc, they may not be in your package manager
# make sure these libraries can be detected with pkgconf
git clone https://github.com/psychoartistc/dogberry
cd dogberry
make
```
running: 
```
# from the source directory
swc-launch dogberry
```
