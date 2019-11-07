
Debian
====================
This directory contains files used to package bitcredsd/bitcreds-qt
for Debian-based Linux systems. If you compile bitcredsd/bitcreds-qt yourself, there are some useful files here.

## bitcreds: URI support ##


bitcreds-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bitcreds-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bitcreds-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcreds128.png` to `/usr/share/pixmaps`

bitcreds-qt.protocol (KDE)

