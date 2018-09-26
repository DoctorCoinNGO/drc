
Debian
====================
This directory contains files used to package drcd/drc-qt
for Debian-based Linux systems. If you compile drcd/drc-qt yourself, there are some useful files here.

## drc: URI support ##


drc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install drc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your drc-qt binary to `/usr/bin`
and the `../../share/pixmaps/drc128.png` to `/usr/share/pixmaps`

drc-qt.protocol (KDE)

