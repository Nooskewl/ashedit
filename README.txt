Introduction
------------

AshEdit is a tile-based level editor used by many Nooskewl games. It was
originally written for an unreleased game called Ashes Fall, hence the name.

The levels it produces are in a very simple binary format that is easy to
load. See FORMAT.txt.

The file "area" in the binary distribution is an example area in Crystal
Picnic format.


License
-------

The license in LICENSE.txt does not cover the icons. Those are not to be
reused.


Using
-----

Tile sheets are named tiles0.png, tiles1.png, etc. TGA images are also
supported. For Monster RPG 2 maps, only one tile sheet can be used.

File type is determined by filename as so:

.map	Monster RPG 3 maps
area	This is the full filename of Crystal Picnic maps
.area	This extension is for Monster RPG 2 maps

Everything else is loaded in the new Wedge 2 format.

Refer to the online help for how to use the program.


Building
--------

AshEdit requires tgui2 to build. Allegro 5.2 is also needed.

Set CMake variables USER_INCLUDE_PATH and USER_LIBRARY_PATH to where you've
put Allegro and TGUI2 if necessary.

	mkdir build
	cd build
	cmake ..
	msbuild /p:Configuration=Release AshEdit.sln # or use make
	<run AshEdit.exe>

On Windows it loads arial.ttf from C:\Windows\Fonts.
