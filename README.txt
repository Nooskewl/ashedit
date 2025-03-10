Introduction
------------

AshEdit is a tile-based level editor used by many Nooskewl games. It was
originally written for an unreleased game called Ashes Fall, hence the name.

The levels it produces are in a very simple binary format that is easy to
load. See FORMAT.txt.


Using
-----

Tile sheets are named tiles0.png, tiles1.png, etc. TGA images are also
supported. For Monster RPG 2 maps, only one tile sheet can be used.

File type is determined by filename if you run with
-use-filename-based-level-types as so:

.map	Monster RPG 3 maps
area	This is the full filename of Crystal Picnic maps
.area	This extension is for Monster RPG 2 maps

Everything else is loaded in the new Wedge 3 format.

Refer to the online help for how to use the program.

On Windows it loads arial.ttf from C:\Windows\Fonts. On Linux it
looks in the working directory for font.ttf.

Visit https://illnorth.ca for updates...
