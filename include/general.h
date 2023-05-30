#ifndef GENERAL_H
#define GENERAL_H

#include <allegro5/allegro.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifdef ALLEGRO_WINDOWS
#define mkdir(a, b) _mkdir(a)
#include <allegro5/allegro_windows.h>
#include <winreg.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#define sprintf_s snprintf
#define strcpy_s(a, b, c) strncpy(a, c, b)
#define MAX_PATH 1024
#endif

class General {
public:
	static int tileSize;
	static int areaSize;
	static int startLayers;
	static bool can_add_and_delete_layers;
	static int scale;
};

// Formerly in TGUI
extern ALLEGRO_EVENT_SOURCE evtsrc;
const int TGUI_EVENT_OBJECT = 0;
void pushEvent(int type, void *data);

enum LEVEL_TYPE {
	NONE,
	WM2,
	MO3,
	MO2,
	CP
};

#endif // GENERAL_H
