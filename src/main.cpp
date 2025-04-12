#define ALLEGRO_STATICLINK

#include <fstream>

#include "general.h"
#include "widgets.h"

#include <sstream>
#include <cstdio>
#include <map>

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_memfile.h>

#include "images/icon.h"
#include "images/mouse_cursor.h"
#include "images/moved_tile.h"
#include "images/solid.h"

#ifdef ALLEGRO_UNIX
#define ASSERT ALLEGRO_ASSERT
//#define X11_ATOM(x)  XInternAtom(display, #x, False);
#include "allegro5/allegro.h"
//#include "allegro5/allegro_x.h"
//#include "allegro5/internal/aintern_x.h"
//#include "allegro5/internal/aintern_xdisplay.h"
//#include "allegro5/internal/aintern_xsystem.h"
//#include "allegro5/internal/aintern_xwindow.h"

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xatom.h>
#endif

#ifdef ALLEGRO_MACOSX
#undef ALLEGRO_KEY_LCTRL
#undef ALLEGRO_KEY_RCTRL
#define ALLEGRO_KEY_LCTRL ALLEGRO_KEY_COMMAND
#define ALLEGRO_KEY_RCTRL ALLEGRO_KEY_COMMAND

#include <AppKit/AppKit.h>
#endif

const int NUM_RECENT = 10;
const uint16_t RECENT_ID_START = 0xfff;

enum {
	FILE_ID = 1,
	FILE_NEW_ID,
	FILE_OPEN_ID,
	FILE_OPEN_RECENT_ID,
	FILE_SAVE_ID,
	FILE_SAVE_AS_ID,
	FILE_RELOAD_TILES_ID,
	FILE_LOAD_TILES_ID,
	FILE_SET_TILE_SIZE_ID,
	FILE_EXIT_ID,
	EDIT_ID,
	EDIT_UNDO_ID,
	EDIT_REDO_ID,
	EDIT_SELECT_ALL_ID,
	EDIT_SELECT_NONE_ID,
	EDIT_INVERT_ID,
	EDIT_COPY_ID,
	EDIT_COPY_ALL_ID,
	EDIT_CUT_ID,
	EDIT_CUT_ALL_ID,
	EDIT_DELETE_ID,
	EDIT_DELETE_ALL_ID,
	EDIT_PASTE_ID,
	SCALE_ID,
	SCALE_1_ID,
	SCALE_2_ID,
	SCALE_3_ID,
	SCALE_4_ID,
	SCALE_5_ID,
	SCALE_6_ID,
	SCALE_7_ID,
	SCALE_8_ID,
	SCALE_9_ID,
	SCALE_10_ID,
#ifdef SUPPORT_GROUPS
	GROUP_TYPE_ID,
	GROUP_1_ID,
	GROUP_2_ID,
	GROUP_3_ID,
	GROUP_4_ID,
	GROUP_5_ID,
	GROUP_6_ID,
	GROUP_7_ID,
	GROUP_8_ID,
	GROUP_9_ID,
	GROUP_10_ID,
	GROUP_11_ID,
	GROUP_12_ID,
	GROUP_13_ID,
	GROUP_14_ID,
	GROUP_15_ID,
	GROUP_16_ID,
	GROUP_17_ID,
	GROUP_18_ID,
	GROUP_19_ID,
	GROUP_20_ID,
	GROUP_21_ID,
	GROUP_22_ID,
	GROUP_23_ID,
	GROUP_24_ID,
	GROUP_25_ID,
	GROUP_26_ID,
	GROUP_27_ID,
	GROUP_28_ID,
	GROUP_29_ID,
	GROUP_30_ID,
	GROUP_31_ID,
	GROUP_32_ID,
#endif
	LAYERS_ID,
	LAYER_BEFORE_ID,
	LAYER_AFTER_ID,
	LAYER_DELETE_ID,
	HELP_ID,
	HELP_QUICK_REFERENCE_ID
};

#ifdef ALLEGRO_MACOSX
#define ONE_TAB ""
#define TWO_TABS ""
#define THREE_TABS ""
#define CTRL_N ""
#define CTRL_O ""
#define CTRL_S ""
#define CTRL_SHIFT_S ""
#define CTRL_Z ""
#define CTRL_Y ""
#define CTRL_C ""
#define CTRL_SHIFT_C ""
#define CTRL_X ""
#define CTRL_SHIFT_X ""
#define ASH_DELETE ""
#define SHIFT_DELETE ""
#define CTRL_V ""
#define CTRL_A ""
#define CTRL_D ""
#define CTRL_SHIFT_I ""
#define ALT_UP ""
#define ALT_DOWN ""
#define ALT_L ""
#define MAC_CTRL "&"
#define MAC_CTRL_SHIFT "&|"
#define MAC_DELETE "~\x7F"
#define MAC_SHIFT_DELETE "|\x7F"
#define CTRL_H ""
#define MAC_CTRL_H "~H"
#define HELP_CTRL "Command"
#define HELP_ALT "Option"
#define MAC_CTRL_Z "&Z"
#define MAC_CTRL_Y "&Y" 
#define MAC_CTRL_X "&X"
#define MAC_CTRL_SHIFT_X "&|X"
#define MAC_CTRL_V "&V"
#define MAC_CTRL_D "&D"
#else
#ifdef __linux__
#define ONE_TAB "\t"
#define TWO_TABS "\t\t"
#define THREE_TABS "\t\t\t"
#else
#define ONE_TAB "\t"
#define TWO_TABS "\t"
#define THREE_TABS "\t"
#endif
#define CTRL_N "(Ctrl+N)"
#define CTRL_O "(Ctrl+O)"
#define CTRL_S "(Ctrl+S)"
#define CTRL_SHIFT_S "(Ctrl+Shift+S)"
#define CTRL_Z "(Ctrl+Z)"
#define CTRL_Y "(Ctrl+Y)"
#define CTRL_C "(Ctrl+C)"
#define CTRL_SHIFT_C "(Ctrl+Shift+C)"
#define CTRL_X "(Ctrl+X)"
#define CTRL_SHIFT_X "(Ctrl+Shift+X)"
#define ASH_DELETE "(Delete)"
#define SHIFT_DELETE "(Shift+Delete)"
#define CTRL_V "(Ctrl+V)"
#define CTRL_A "(Ctrl+A)"
#define CTRL_D "(Ctrl+D)"
#define CTRL_SHIFT_I "(Ctrl+Shift+I)"
#define ALT_UP "(Alt+Up)"
#define ALT_DOWN "(Alt+Down)"
#define ALT_L "(Alt+L)"
#define MAC_CTRL ""
#define MAC_CTRL_SHIFT ""
#define MAC_SHIFT_DELETE ""
#define CTRL_H "(Ctrl+H)"
#define MAC_CTRL_H ""
#define HELP_CTRL "Ctrl"
#define HELP_ALT "Alt"
#define MAC_CTRL_Z ""
#define MAC_CTRL_Y ""
#define MAC_CTRL_X ""
#define MAC_CTRL_SHIFT_X ""
#define MAC_DELETE ""
#define MAC_SHIFT_DELETE ""
#define MAC_CTRL_V ""
#define MAC_CTRL_D ""
#endif

#ifdef __linux__
#define FOUR_TABS "\t\t\t\t"
#define FIVE_TABS "\t\t\t\t\t"
#define SIX_TABS "\t\t\t\t\t\t"
#define FILE_NEW_TABS FOUR_TABS
#define FILE_OPEN_TABS FOUR_TABS
#define FILE_SAVE_TABS FOUR_TABS
#define FILE_SAVE_AS_TABS THREE_TABS
#define EDIT_UNDO_TABS FIVE_TABS
#define EDIT_REDO_TABS FIVE_TABS
#define EDIT_COPY_TABS FIVE_TABS
#define EDIT_COPY_ALL_TABS THREE_TABS
#define EDIT_CUT_TABS SIX_TABS
#define EDIT_CUT_ALL_TABS THREE_TABS
#define EDIT_DELETE_TABS FIVE_TABS
#define EDIT_DELETE_ALL_TABS TWO_TABS
#define EDIT_PASTE_TABS FIVE_TABS
#define EDIT_SELECT_ALL_TABS FOUR_TABS
#define EDIT_SELECT_NONE_TABS FOUR_TABS
#define EDIT_INVERT_TABS THREE_TABS
#define LAYER_BEFORE_TABS THREE_TABS
#define LAYER_AFTER_TABS THREE_TABS
#define LAYER_DELETE_TABS FOUR_TABS
#define HELP_QUICK_REFERENCE_TABS TWO_TABS
#else
#define FILE_NEW_TABS THREE_TABS
#define FILE_OPEN_TABS THREE_TABS
#define FILE_SAVE_TABS THREE_TABS
#define FILE_SAVE_AS_TABS THREE_TABS
#define EDIT_UNDO_TABS TWO_TABS
#define EDIT_REDO_TABS TWO_TABS
#define EDIT_COPY_TABS TWO_TABS
#define EDIT_COPY_ALL_TABS TWO_TABS
#define EDIT_CUT_TABS TWO_TABS
#define EDIT_CUT_ALL_TABS TWO_TABS
#define EDIT_DELETE_TABS TWO_TABS
#define EDIT_DELETE_ALL_TABS TWO_TABS
#define EDIT_PASTE_TABS TWO_TABS
#define EDIT_SELECT_ALL_TABS THREE_TABS
#define EDIT_SELECT_NONE_TABS THREE_TABS
#define EDIT_INVERT_TABS TWO_TABS
#define LAYER_BEFORE_TABS TWO_TABS
#define LAYER_AFTER_TABS TWO_TABS
#define LAYER_DELETE_TABS TWO_TABS
#define HELP_QUICK_REFERENCE_TABS TWO_TABS
#endif

const char *FILE_NEW_ID_STR = MAC_CTRL "New..." FILE_NEW_TABS CTRL_N;
const char *FILE_OPEN_ID_STR = MAC_CTRL "Open..." FILE_OPEN_TABS CTRL_O;
const char *FILE_SAVE_ID_STR = MAC_CTRL "Save" FILE_SAVE_TABS CTRL_S;
const char *FILE_SAVE_AS_ID_STR = MAC_CTRL_SHIFT "Save As..." FILE_SAVE_AS_TABS CTRL_SHIFT_S;
const char *FILE_RELOAD_TILES_ID_STR = "Reload Tiles";
const char *FILE_LOAD_TILES_ID_STR = "Load Tiles...";
const char *FILE_SET_TILE_SIZE_ID_STR = "Set Tile Size...";
const char *FILE_EXIT_ID_STR = "Exit";
const char *EDIT_UNDO_ID_STR = "Undo" EDIT_UNDO_TABS CTRL_Z MAC_CTRL_Z;
const char *EDIT_REDO_ID_STR = "Redo" EDIT_REDO_TABS CTRL_Y MAC_CTRL_Y;
const char *EDIT_COPY_ID_STR = MAC_CTRL "Copy" EDIT_COPY_TABS CTRL_C;
const char *EDIT_COPY_ALL_ID_STR = MAC_CTRL_SHIFT "Copy (all layers)" EDIT_COPY_ALL_TABS CTRL_SHIFT_C;
const char *EDIT_CUT_ID_STR = "Cut" EDIT_CUT_TABS CTRL_X MAC_CTRL_X;
const char *EDIT_CUT_ALL_ID_STR = "Cut (all layers)" EDIT_CUT_ALL_TABS CTRL_SHIFT_X MAC_CTRL_SHIFT_X;
const char *EDIT_DELETE_ID_STR = "Delete" EDIT_DELETE_TABS ASH_DELETE MAC_DELETE;
const char *EDIT_DELETE_ALL_ID_STR = "Delete (all layers)" EDIT_DELETE_ALL_TABS SHIFT_DELETE MAC_SHIFT_DELETE;
const char *EDIT_PASTE_ID_STR = "Paste" EDIT_PASTE_TABS CTRL_V MAC_CTRL_V;
const char *EDIT_SELECT_ALL_ID_STR = "Select " MAC_CTRL "All" EDIT_SELECT_ALL_TABS CTRL_A;
const char *EDIT_SELECT_NONE_ID_STR = "Select None" EDIT_SELECT_NONE_TABS CTRL_D MAC_CTRL_D;
const char *EDIT_INVERT_ID_STR = MAC_CTRL_SHIFT "Invert Selection" EDIT_INVERT_TABS CTRL_SHIFT_I;
const char *LAYER_BEFORE_ID_STR = "Insert Layer Before" LAYER_BEFORE_TABS ALT_UP;
const char *LAYER_AFTER_ID_STR = "Insert Layer After" LAYER_AFTER_TABS ALT_DOWN;
const char *LAYER_DELETE_ID_STR = "Delete Layer" LAYER_DELETE_TABS ALT_L;
const char *SCALE_1_ID_STR = "1x";
const char *SCALE_2_ID_STR = "2x";
const char *SCALE_3_ID_STR = "3x";
const char *SCALE_4_ID_STR = "4x";
const char *SCALE_5_ID_STR = "5x";
const char *SCALE_6_ID_STR = "6x";
const char *SCALE_7_ID_STR = "7x"; 
const char *SCALE_8_ID_STR = "8x";
const char *SCALE_9_ID_STR = "9x";
const char *SCALE_10_ID_STR = "10x";
const char *GROUP_1_ID_STR = "1";
const char *GROUP_2_ID_STR = "2";
const char *GROUP_3_ID_STR = "3";
const char *GROUP_4_ID_STR = "4";
const char *GROUP_5_ID_STR = "5";
const char *GROUP_6_ID_STR = "6";
const char *GROUP_7_ID_STR = "7";
const char *GROUP_8_ID_STR = "8";
const char *GROUP_9_ID_STR = "9";
const char *GROUP_10_ID_STR = "10";
const char *GROUP_11_ID_STR = "11";
const char *GROUP_12_ID_STR = "12";
const char *GROUP_13_ID_STR = "13";
const char *GROUP_14_ID_STR = "14";
const char *GROUP_15_ID_STR = "15";
const char *GROUP_16_ID_STR = "16";
const char *GROUP_17_ID_STR = "17";
const char *GROUP_18_ID_STR = "18";
const char *GROUP_19_ID_STR = "19";
const char *GROUP_20_ID_STR = "20";
const char *GROUP_21_ID_STR = "21";
const char *GROUP_22_ID_STR = "22";
const char *GROUP_23_ID_STR = "23";
const char *GROUP_24_ID_STR = "24";
const char *GROUP_25_ID_STR = "25";
const char *GROUP_26_ID_STR = "26";
const char *GROUP_27_ID_STR = "27";
const char *GROUP_28_ID_STR = "28";
const char *GROUP_29_ID_STR = "29";
const char *GROUP_30_ID_STR = "30";
const char *GROUP_31_ID_STR = "31";
const char *GROUP_32_ID_STR = "32";
const char *HELP_QUICK_REFERENCE_ID_STR = "Quick Reference" HELP_QUICK_REFERENCE_TABS CTRL_H MAC_CTRL_H;

ALLEGRO_MENU_INFO main_menu_info[] = {
	ALLEGRO_START_OF_MENU("File", FILE_ID),
		{ (const char *)FILE_NEW_ID_STR, FILE_NEW_ID, 0, NULL },
		{ (const char *)FILE_OPEN_ID_STR, FILE_OPEN_ID, 0, NULL },
		ALLEGRO_START_OF_MENU("Open Recent...", FILE_OPEN_RECENT_ID),
		ALLEGRO_END_OF_MENU,
		{ (const char *)FILE_SAVE_ID_STR, FILE_SAVE_ID, 0, NULL },
		{ (const char *)FILE_SAVE_AS_ID_STR, FILE_SAVE_AS_ID, 0, NULL },
		ALLEGRO_MENU_SEPARATOR,
		{ (const char *)FILE_RELOAD_TILES_ID_STR, FILE_RELOAD_TILES_ID, 0, NULL },
		{ (const char *)FILE_LOAD_TILES_ID_STR, FILE_LOAD_TILES_ID, 0, NULL },
		{ (const char *)FILE_SET_TILE_SIZE_ID_STR, FILE_SET_TILE_SIZE_ID, 0, NULL },
#ifndef ALLEGRO_MACOSX
		ALLEGRO_MENU_SEPARATOR,
		{ (const char *)FILE_EXIT_ID_STR, FILE_EXIT_ID, 0, NULL },
#endif
		ALLEGRO_END_OF_MENU,

	ALLEGRO_START_OF_MENU("Edit", EDIT_ID),
		{ (const char *)EDIT_UNDO_ID_STR, EDIT_UNDO_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_REDO_ID_STR, EDIT_REDO_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		ALLEGRO_MENU_SEPARATOR,
		{ (const char *)EDIT_COPY_ID_STR, EDIT_COPY_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_COPY_ALL_ID_STR, EDIT_COPY_ALL_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_CUT_ID_STR, EDIT_CUT_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_CUT_ALL_ID_STR, EDIT_CUT_ALL_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_DELETE_ID_STR, EDIT_DELETE_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_DELETE_ALL_ID_STR, EDIT_DELETE_ALL_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_PASTE_ID_STR, EDIT_PASTE_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		ALLEGRO_MENU_SEPARATOR,
		{ (const char *)EDIT_SELECT_ALL_ID_STR, EDIT_SELECT_ALL_ID, 0, NULL },
		{ (const char *)EDIT_SELECT_NONE_ID_STR, EDIT_SELECT_NONE_ID, ALLEGRO_MENU_ITEM_DISABLED, NULL },
		{ (const char *)EDIT_INVERT_ID_STR, EDIT_INVERT_ID, 0, NULL },
			ALLEGRO_END_OF_MENU,

	ALLEGRO_START_OF_MENU("Layer", LAYERS_ID),
		{ (const char *)LAYER_BEFORE_ID_STR, LAYER_BEFORE_ID, 0, NULL },
		{ (const char *)LAYER_AFTER_ID_STR, LAYER_AFTER_ID, 0, NULL },
		{ (const char *)LAYER_DELETE_ID_STR, LAYER_DELETE_ID, 0, NULL },
			ALLEGRO_END_OF_MENU,

	ALLEGRO_START_OF_MENU("Scale", SCALE_ID),
		{ (const char *)SCALE_1_ID_STR, SCALE_1_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_2_ID_STR, SCALE_2_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_3_ID_STR, SCALE_3_ID, ALLEGRO_MENU_ITEM_CHECKBOX | ALLEGRO_MENU_ITEM_CHECKED, NULL },
		{ (const char *)SCALE_4_ID_STR, SCALE_4_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_5_ID_STR, SCALE_5_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_6_ID_STR, SCALE_6_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_7_ID_STR, SCALE_7_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_8_ID_STR, SCALE_8_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_9_ID_STR, SCALE_9_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)SCALE_10_ID_STR, SCALE_10_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
			ALLEGRO_END_OF_MENU,

#ifdef SUPPORT_GROUPS
	ALLEGRO_START_OF_MENU("Group", GROUP_TYPE_ID),
		{ (const char *)GROUP_1_ID_STR, GROUP_1_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_2_ID_STR, GROUP_2_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_3_ID_STR, GROUP_3_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_4_ID_STR, GROUP_4_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_5_ID_STR, GROUP_5_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_6_ID_STR, GROUP_6_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_7_ID_STR, GROUP_7_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_8_ID_STR, GROUP_8_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_9_ID_STR, GROUP_9_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_10_ID_STR, GROUP_10_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_11_ID_STR, GROUP_11_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_12_ID_STR, GROUP_12_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_13_ID_STR, GROUP_13_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_14_ID_STR, GROUP_14_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_15_ID_STR, GROUP_15_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_16_ID_STR, GROUP_16_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_17_ID_STR, GROUP_17_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_18_ID_STR, GROUP_18_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_19_ID_STR, GROUP_19_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_20_ID_STR, GROUP_20_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_21_ID_STR, GROUP_21_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_22_ID_STR, GROUP_22_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_23_ID_STR, GROUP_23_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_24_ID_STR, GROUP_24_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_25_ID_STR, GROUP_25_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_26_ID_STR, GROUP_26_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_27_ID_STR, GROUP_27_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_28_ID_STR, GROUP_28_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_29_ID_STR, GROUP_29_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_30_ID_STR, GROUP_30_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_31_ID_STR, GROUP_31_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		{ (const char *)GROUP_32_ID_STR, GROUP_32_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
		ALLEGRO_END_OF_MENU,
#endif

	ALLEGRO_START_OF_MENU("Help", HELP_ID),
		{ (const char *)HELP_QUICK_REFERENCE_ID_STR, HELP_QUICK_REFERENCE_ID, 0, NULL },
		ALLEGRO_END_OF_MENU,

	ALLEGRO_END_OF_MENU
};

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
static bool resize;
std::vector<ALLEGRO_BITMAP *> tileSheets;
A_Combobox *layerCombo, *sheetCombo;
A_Tileselector *tileSelector;
A_Leveleditor *levelEditor;
static bool draw_yellow_and_purple = true;
A_Scrollpane *levelScrollpane;
static int levelX = -1, levelY = -1;
A_Scrollpane *tileScrollpane;
ALLEGRO_TIMER *timer;
ALLEGRO_TIMER *menu_item_timer;
double timer_end_time = 0;
bool timer_ticking = false;
bool menu_item_timer_ticking = false;
bool marquee_marked = false;
std::vector<ALLEGRO_VERTEX> marquee_v;
bool reverse_scroll = true;
bool one_solids_layer = false;
int window_x = -1;
int window_y = -1;
const int DEFAULT_WINDOW_W = 1280;
const int DEFAULT_WINDOW_H = 800;
int window_w = DEFAULT_WINDOW_W;
int window_h = DEFAULT_WINDOW_H;
bool window_maximized = true;
int my_tile_size = -1;
int save_tile_size = -1;
int last_scale = -1;

std::vector<bool> draw_solids;
#ifdef SUPPORT_GROUPS_XXX
std::vector<bool> draw_groups;
#endif
#ifdef SUPPORT_WALLS
bool draw_walls;
#endif
uint64_t new_mouse_pos = -1;

static int mouse_x = 0, mouse_y = 0;

std::string tile_load_path = ".";
static std::string save_tile_load_path = ".";
#if defined ALLEGRO_MACOSX && defined MAS
static int num_tile_sheets = -1;
static bool png_sheets = true;
#endif

ALLEGRO_MENU *menu;

struct EnableDisableMenuItem {
	int id;
	double when;
};

std::vector<EnableDisableMenuItem> enable_menu_items;
std::vector<EnableDisableMenuItem> disable_menu_items;

void enable_menu_item(int id)
{
	if (al_get_timer_started(menu_item_timer)) {
		al_resume_timer(menu_item_timer);
	}
	else {
		al_start_timer(menu_item_timer);
	}
	menu_item_timer_ticking = true;

	int i;
	for (i = 0; i < (int)disable_menu_items.size(); i++) {
		if (disable_menu_items[i].id == id) {
			disable_menu_items.erase(disable_menu_items.begin() + i);
			break;
		}
	}

	for (i = 0; i < (int)enable_menu_items.size(); i++) {
		if (enable_menu_items[i].id == id) {
			enable_menu_items[i].when = al_get_time() + 0.1;
			return;
		}
	}

	EnableDisableMenuItem m;
	m.id = id;
	m.when = al_get_time() + 0.1;
	enable_menu_items.push_back(m);
}

void disable_menu_item(int id)
{
	if (al_get_timer_started(menu_item_timer)) {
		al_resume_timer(menu_item_timer);
	}
	else {
		al_start_timer(menu_item_timer);
	}
	menu_item_timer_ticking = true;

	int i;
	for (i = 0; i < (int)enable_menu_items.size(); i++) {
		if (enable_menu_items[i].id == id) {
			enable_menu_items.erase(enable_menu_items.begin() + i);
			break;
		}
	}

	for (i = 0; i < (int)disable_menu_items.size(); i++) {
		if (disable_menu_items[i].id == id) {
			disable_menu_items[i].when = al_get_time() + 0.1;
			return;
		}
	}

	EnableDisableMenuItem m;
	m.id = id;
	m.when = al_get_time() + 0.1;
	disable_menu_items.push_back(m);
}

void enable_undo_menu_item()
{
	enable_menu_item(EDIT_UNDO_ID);
}

void disable_undo_menu_item()
{
	disable_menu_item(EDIT_UNDO_ID);
}

void enable_redo_menu_item()
{
	enable_menu_item(EDIT_REDO_ID);
}

void disable_redo_menu_item()
{
	disable_menu_item(EDIT_REDO_ID);
}

void enable_copy_menu_item()
{
	enable_menu_item(EDIT_COPY_ID);
}

void disable_copy_menu_item()
{
	disable_menu_item(EDIT_COPY_ID);
}

void enable_copy_all_menu_item()
{
	enable_menu_item(EDIT_COPY_ALL_ID);
}

void disable_copy_all_menu_item()
{
	disable_menu_item(EDIT_COPY_ALL_ID);
}

void enable_cut_menu_item()
{
	enable_menu_item(EDIT_CUT_ID);
}

void disable_cut_menu_item()
{
	disable_menu_item(EDIT_CUT_ID);
}

void enable_cut_all_menu_item()
{
	enable_menu_item(EDIT_CUT_ALL_ID);
}

void disable_cut_all_menu_item()
{
	disable_menu_item(EDIT_CUT_ALL_ID);
}

void enable_delete_menu_item()
{
	enable_menu_item(EDIT_DELETE_ID);
}

void disable_delete_menu_item()
{
	disable_menu_item(EDIT_DELETE_ID);
}

void enable_delete_all_menu_item()
{
	enable_menu_item(EDIT_DELETE_ALL_ID);
}

void disable_delete_all_menu_item()
{
	disable_menu_item(EDIT_DELETE_ALL_ID);
}

void enable_paste_menu_item()
{
	enable_menu_item(EDIT_PASTE_ID);
}

void disable_paste_menu_item()
{
	disable_menu_item(EDIT_PASTE_ID);
}

void enable_select_all_menu_item()
{
	enable_menu_item(EDIT_SELECT_ALL_ID);
}

void disable_select_all_menu_item()
{
	disable_menu_item(EDIT_SELECT_ALL_ID);
}

void enable_select_none_menu_item()
{
	enable_menu_item(EDIT_SELECT_NONE_ID);
}

void disable_select_none_menu_item()
{
	disable_menu_item(EDIT_SELECT_NONE_ID);
}

void enable_layer_before_menu_item()
{
	enable_menu_item(LAYER_BEFORE_ID);
}

void disable_layer_before_menu_item()
{
	disable_menu_item(LAYER_BEFORE_ID);
}

void enable_layer_after_menu_item()
{
	enable_menu_item(LAYER_AFTER_ID);
}

void disable_layer_after_menu_item()
{
	disable_menu_item(LAYER_AFTER_ID);
}

void enable_layer_delete_menu_item()
{
	enable_menu_item(LAYER_DELETE_ID);
}

void disable_layer_delete_menu_item()
{
	disable_menu_item(LAYER_DELETE_ID);
}

void enable_quickref_menu_item()
{
	enable_menu_item(HELP_QUICK_REFERENCE_ID);
}

void disable_quickref_menu_item()
{
	disable_menu_item(HELP_QUICK_REFERENCE_ID);
}

ALLEGRO_FONT *font;
ALLEGRO_FONT *help_font;
ALLEGRO_FONT *help_font_tiny;
ALLEGRO_FONT *help_font_small;
ALLEGRO_FONT *help_font_big;

ALLEGRO_BITMAP *moved_tile_bmp;
ALLEGRO_BITMAP *solid_bmp;

std::vector< std::vector< std::vector<bool > > > blank; // is tile in sheet blank?

std::vector<std::string> recent_files;

#ifdef DUMP
bool dump = false;
LEVEL_TYPE dump_type;
#endif

LEVEL_TYPE command_line_level_type = WM2;
bool use_filename_based_level_types = false;

#include "images/pencil.png.h"
#include "images/clear.png.h"
#include "images/clone.png.h"
#include "images/fill.png.h"
#include "images/macro.png.h"
#include "images/magic_wand.png.h"
#include "images/marquee.png.h"
#include "images/mover.png.h"
#include "images/record.png.h"
#include "images/screenshot.png.h"
#include "images/solids.png.h"
#include "images/toggle_layer.png.h"
#include "images/toggle_solids.png.h"

ALLEGRO_BITMAP *pencil_icon;
ALLEGRO_BITMAP *clear_icon;
ALLEGRO_BITMAP *clone_icon;
ALLEGRO_BITMAP *fill_icon;
ALLEGRO_BITMAP *macro_icon;
ALLEGRO_BITMAP *magic_wand_icon;
ALLEGRO_BITMAP *marquee_icon;
ALLEGRO_BITMAP *mover_icon;
ALLEGRO_BITMAP *record_icon;
ALLEGRO_BITMAP *screenshot_icon;
ALLEGRO_BITMAP *solids_icon;
ALLEGRO_BITMAP *toggle_layer_icon;
ALLEGRO_BITMAP *toggle_solids_icon;

/* Quick reference window stuff
 */
A_Frame *quickRefFrame;
A_Splitter *quickRefSplitter;
A_Splitter *quickRefBottomSplitter;
A_Label *quickRefContent1, *quickRefContent2;
A_Titlebar *quickRefTitlebar;
ALLEGRO_MOUSE_CURSOR *mouse_cursor;

/* Tile size window stuff
 */
InputGetterDummy *tileSizeGetterDummy;
TileSizeGetter *tileSizeGetterContent;
A_Splitter *tileSizeGetterSplitter;
A_Titlebar *tileSizeGetterTitlebar;

/* New level size window stuff
 */
InputGetterDummy *levelSizeGetterDummy;
LevelSizeGetter *levelSizeGetterContent;
A_Splitter *levelSizeGetterSplitter;
A_Titlebar *levelSizeGetterTitlebar;

A_Splitter *buffer1;
A_Splitter *buffer2;
A_Splitter *buffer3;
A_Splitter *buffer4;
A_Splitter *buffer5;
A_Splitter *buffer6;

void layer_callback(int selected)
{
	levelEditor->setLayer(selected);
}

#if defined ALLEGRO_IPHONE
/*
 * Return the path to user resources (save states, configuration)
 */
static char *userResourcePath()
{
	static char path[MAX_PATH];

	ALLEGRO_PATH *user_path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
	sprintf(path, "%s/", al_path_cstr(user_path, '/'));
	al_destroy_path(user_path);
	return path;
}
#endif

// NOTE: a5 changed path from home/Library/Preferences to Documents before 5.0.0
const char *getUserResource(const char *fmt, ...)
{
	va_list ap;

#ifdef ALLEGRO_IPHONE
	char file[MAX_PATH];
	static char result[MAX_PATH];
	char old[MAX_PATH];

	// This stuff is for backwards compatibility when
	// saves and screenshots etc were stored in Library/Preferences
	sprintf(file, "%s/AshEdit", userResourcePath());
	if (!al_filename_exists(file))
		mkdir(file, 0755);

	va_start(ap, fmt);
	vsnprintf(file, MAX_PATH, fmt, ap);
	va_end(ap);

	sprintf(old, "%s/Library/Preferences/%s", getenv("HOME"), file);
	sprintf(result, "%s/AshEdit/%s", userResourcePath(), file);

	if (al_filename_exists(old)) {
		rename(old, result);
	}
#else
	char s1[MAX_PATH];
	char s2[MAX_PATH];
	static char result[MAX_PATH];

	ALLEGRO_PATH *user_path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
	strcpy_s(s1, MAX_PATH, al_path_cstr(user_path, ALLEGRO_NATIVE_PATH_SEP));
	al_drop_path_tail(user_path);
	strcpy_s(s2, MAX_PATH, al_path_cstr(user_path, ALLEGRO_NATIVE_PATH_SEP));
	al_destroy_path(user_path);

	if (!al_filename_exists(s2))
		mkdir(s2, 0755);
	if (!al_filename_exists(s1))
		mkdir(s1, 0755);

	va_start(ap, fmt);
#ifdef ALLEGRO_WINDOWS
	vsnprintf_s(s2, MAX_PATH, MAX_PATH, fmt, ap);
#else
	vsnprintf(s2, MAX_PATH, fmt, ap);
#endif
	va_end(ap);

#ifdef ALLEGRO_MACOSX
	sprintf_s(result, MAX_PATH, "%s%s", s1, s2);
#else
	sprintf_s(result, MAX_PATH, "%s/%s", s1, s2);
#endif
#endif
	
	return result;
}

void setTitle()
{
	std::string filename = levelEditor->getOperatingFilename();
	if (filename == "") {
		al_set_window_title(display, "AshEdit");
	}
	else {
		std::string name = std::string("AshEdit") + " - " + filename;
		al_set_window_title(display, name.c_str());
	}
}

static void draw(bool fake = false)
{
	if (resize) {
		tgui::resize(NULL);
		resize = false;
	}
	al_clear_to_color(al_map_rgb(255, 255, 255));
	if (fake == true) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
	tgui::draw();
	al_flip_display();
}

static void tileDrawCallback(int ox, int oy, int dx, int dy, int w, int h, int layer)
{
	(void)layer;

	ALLEGRO_BITMAP *sheet = 0;
	
	if (!tileSheets.size() || sheetCombo->getSelected() > (int)tileSheets.size())
		return;
	
	sheet = tileSheets[sheetCombo->getSelected()];

	int bmp_w = al_get_bitmap_width(sheet);
	int bmp_h = al_get_bitmap_height(sheet);

	int wt = bmp_w / General::tileSize;
	int ht = bmp_h / General::tileSize;

	wt = MIN(256, wt);
	ht = MIN(256, ht);

	bmp_w = wt * General::tileSize;
	bmp_h = ht * General::tileSize;

	al_draw_scaled_bitmap(
		sheet,
		0,
		0,
		bmp_w,
		bmp_h,
		dx-ox,
		dy-oy,
		bmp_w*General::scale,
		bmp_h*General::scale,
		0
	);

	int sel_x, sel_y, sel_w, sel_h;
	tileSelector->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);

	sel_w = MIN(sel_w, wt-sel_x);
	sel_h = MIN(sel_h, ht-sel_y);

	int rx = dx + ((sel_x*General::tileSize*General::scale) - ox);
	int ry = dy + ((sel_y*General::tileSize*General::scale) - oy);
		
	al_draw_rectangle(
		rx+0.5,
		ry+0.5,
		rx+General::tileSize*sel_w*General::scale-0.5,
		ry+General::tileSize*sel_h*General::scale-0.5,
		al_map_rgb(0, 0, 0),
		1
	);
	al_draw_rectangle(
		rx+1.5,
		ry+1.5,
		rx+General::tileSize*sel_w*General::scale-1.5,
		ry+General::tileSize*sel_h*General::scale-1.5,
		al_map_rgb(255, 216, 0x00),
		1
	);
	al_draw_rectangle(
		rx+2.5,
		ry+2.5,
		rx+General::tileSize*sel_w*General::scale-2.5,
		ry+General::tileSize*sel_h*General::scale-2.5,
		al_map_rgb(0, 0, 0),
		1
	);
}

static int get_solids_layer(int layer)
{
	if (levelEditor->get_level_type() != CP) {
		return 0;
	}
	else {
		return layer;
	}
}

static void levelDrawCallback(int ox, int oy, int dx, int dy, int w, int h, int layer)
{
	int xx = dx - (ox % (General::tileSize*General::scale));
	int yy = dy - (oy % (General::tileSize*General::scale));
	int tx = ox / (General::tileSize*General::scale);
	int ty = oy / (General::tileSize*General::scale);
	int wt = w / (General::tileSize*General::scale)+2;
	int ht = h / (General::tileSize*General::scale)+2;
	int layers = levelEditor->getLayers();
	int savedx = dx;
	int savedy = dy;

	std::vector<bool> solids(layers);
		
	int maxx = MIN(w, levelEditor->getWidth()*(General::tileSize*General::scale)-ox);
	int maxy = MIN(h, levelEditor->getHeight()*(General::tileSize*General::scale)-oy);

	if (layer == -1) {
		if (draw_yellow_and_purple) {
			al_draw_filled_rectangle(
				dx,
				dy,
				dx+w*General::scale,
				dy+h*General::scale,
				al_map_rgb(255, 0, 255)
			);
		}
		if (draw_yellow_and_purple) {
			al_draw_filled_rectangle(
				dx,
				dy,
				dx+maxx,
				dy+maxy,
				al_map_rgb(0, 255, 0)
			);
		}
	}

	dy = yy;
	for (int y = 0; y < ht; y++) {
		if (ty+y >= levelEditor->getHeight() || ty+y < 0)
			continue;
		dx = xx;
		for (int x = 0; x < wt; x++) {
			if (tx+x >= levelEditor->getWidth() || tx+x < 0)
				continue;
			int lstart, lend;
			if (layer == -1) {
				lstart = 0;
				lend = layers;
			}
			else {
				lstart = layer;
				lend = layer+1;
			}
			for (int l = lstart; l < lend; l++) {
				int tile, sheet;
				bool tint;
				bool solids_tmp = solids[l];
				if (levelEditor->getTile(tx+x, ty+y, l, &tile, &sheet, &solids_tmp, &tint) == false) {
					continue;
				}
				solids[l] = solids_tmp;
				if (tile < 0 || sheet < 0) {
					continue;
				}
				if (sheet >= (int)tileSheets.size()) {
					continue;
				}
				if (levelEditor->isVisible(l) == false)
					continue;
				ALLEGRO_BITMAP *bmp = tileSheets[sheet];
				int widthInTiles = al_get_bitmap_width(bmp) / General::tileSize;
				int tileX = tile % widthInTiles;
				int tileY = tile / widthInTiles;
				al_draw_scaled_bitmap(
					bmp,
					tileX*General::tileSize,
					tileY*General::tileSize,
					General::tileSize,
					General::tileSize,
					dx,
					dy,
					General::tileSize*General::scale,
					General::tileSize*General::scale,
					0
				);
			}
			bool is_solid = false;
			if (levelEditor->get_level_type() != CP) {
				int nl = layerCombo->getSize();
				for (int i = 0; i < nl; i++) {
					if (solids[i]) {
						is_solid = true;
						break;
					}
				}
			}
			else {
				is_solid = solids[layerCombo->getSelected()];
			}
			if (draw_solids[get_solids_layer(levelEditor->getCurrentLayer())] && is_solid) {
				int bmp_w = al_get_bitmap_width(solid_bmp);
				int bmp_h = al_get_bitmap_height(solid_bmp);
				int icon_scale = General::tileSize / 12;

				al_draw_scaled_bitmap(
					solid_bmp,
					0,
					0,
					bmp_w,
					bmp_h,
					dx+(General::tileSize/2.0f-bmp_w*icon_scale/2.0f)*General::scale,
					dy+(General::tileSize/2.0f-bmp_h*icon_scale/2.0f)*General::scale,
					bmp_w*General::scale*icon_scale,
					bmp_h*General::scale*icon_scale,
					0
				);
			}
			dx += (General::tileSize*General::scale);
		}
		dy += (General::tileSize*General::scale);
	}
	
	dy = yy;
	for (int y = 0; y < ht; y++) {
		if (ty+y >= levelEditor->getHeight() || ty+y < 0)
			continue;
		dx = xx;
		for (int x = 0; x < wt; x++) {
			if (tx+x >= levelEditor->getWidth() || tx+x < 0)
				continue;
			int lstart, lend;
			if (layer == -1) {
				lstart = 0;
				lend = layers;
			}
			else {
				lstart = layer;
				lend = layer+1;
			}
			for (int l = lstart; l < lend; l++) {
				int tile, sheet;
				bool tint;
				bool solids_tmp = solids[l];
				if (levelEditor->getTile(tx+x, ty+y, l, &tile, &sheet, &solids_tmp, &tint) == false) {
					continue;
				}
				solids[l] = solids_tmp;
				if (tile < 0 || sheet < 0) {
					continue;
				}
				if (sheet >= (int)tileSheets.size()) {
					continue;
				}
				/*
				if (levelEditor->isVisible(l) == false)
					continue;
				*/
				ALLEGRO_BITMAP *bmp = tileSheets[sheet];
				int widthInTiles = al_get_bitmap_width(bmp) / General::tileSize;
				int tileX = tile % widthInTiles;
				int tileY = tile / widthInTiles;

				/*
				int bmp_w = al_get_bitmap_width(moved_tile_bmp);
				int bmp_h = al_get_bitmap_height(moved_tile_bmp);

				if (tint) {
					al_draw_scaled_bitmap(
						moved_tile_bmp,
						0,
						0,
						bmp_w,
						bmp_h,
						dx,
						dy,
						bmp_w*General::scale,
						bmp_h*General::scale,
						0
					);
				}
				*/
			
				if (tint) {	
					int bmp_w = al_get_bitmap_width(moved_tile_bmp);
					int bmp_h = al_get_bitmap_height(moved_tile_bmp);
					int icon_scale = General::tileSize / 12;

					al_draw_scaled_bitmap(
						moved_tile_bmp,
						0,
						0,
						bmp_w,
						bmp_h,
						dx+(General::tileSize/2.0f-bmp_w*icon_scale/2.0f)*General::scale,
						dy+(General::tileSize/2.0f-bmp_h*icon_scale/2.0f)*General::scale,
						bmp_w*General::scale*icon_scale,
						bmp_h*General::scale*icon_scale,
						0
					);
				}
			}
			dx += (General::tileSize*General::scale);
		}
		dy += (General::tileSize*General::scale);
	}
	
	if (mouse_x > -1 && mouse_y > -1 && draw_yellow_and_purple) {
		int xx = mouse_x - (mouse_x % (General::tileSize*General::scale));
		int yy = mouse_y - (mouse_y % (General::tileSize*General::scale));
		xx -= ox;
		yy -= oy;
		int abs_x, abs_y;
		determineAbsolutePosition(levelEditor, &abs_x, &abs_y);
		xx += abs_x;
		yy += abs_y;
		al_draw_rectangle(
			xx+0.5f,
			yy+0.5f,
			xx + (General::tileSize*General::scale)-0.5f,
			yy + (General::tileSize*General::scale)-0.5f,
			al_map_rgb(0, 0, 0), 1.0);
		al_draw_rectangle(
			xx+1+0.5f,
			yy+1+0.5f,
			xx + (General::tileSize*General::scale)-1-0.5f,
			yy + (General::tileSize*General::scale)-1-0.5f,
			al_map_rgb(255, 216, 0), 1.0);
		al_draw_rectangle(
			xx+2+0.5f,
			yy+2+0.5f,
			xx + (General::tileSize*General::scale)-2-0.5f,
			yy + (General::tileSize*General::scale)-2-0.5f,
			al_map_rgb(0, 0, 0), 1.0);
	}

#ifdef SUPPORT_GROUPS
	std::vector<A_Leveleditor::Group> &groups = levelEditor->getGroups();

	for (size_t i = 0; i < groups.size(); i++) {
		A_Leveleditor::Group &g = groups[i];
		//if (draw_groups[g.layer] && g.layer == levelEditor->getCurrentLayer()) {
			ALLEGRO_COLOR colour;
			colour = al_map_rgb(0, 255, 255);
			al_draw_rectangle(savedx + (g.x * General::tileSize * General::scale) - ox, savedy + (g.y * General::tileSize * General::scale) - oy, savedx + ((g.x + g.w) * General::tileSize * General::scale) - ox, savedy + ((g.y + g.h) * General::tileSize * General::scale) - oy, colour, General::scale);
		//}
	}
#endif

#ifdef SUPPORT_WALLS
	if (draw_walls) {
		std::vector<A_Leveleditor::Wall> &walls = levelEditor->getWalls();

		for (size_t i = 0; i < walls.size(); i++) {
			A_Leveleditor::Wall &w = walls[i];
			ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
			al_draw_rectangle(
				savedx + (w.x * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z) * General::tileSize * General::scale) - oy,
				savedx + ((w.x + w.size_x) * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z + w.size_y) * General::tileSize * General::scale) - oy,
				white,
				1.0f
			);
			al_draw_rectangle(
				savedx + (w.x * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z - w.size_z) * General::tileSize * General::scale) - oy,
				savedx + ((w.x + w.size_x) * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z + w.size_y - w.size_z) * General::tileSize * General::scale) - oy,
				white,
				1.0f
			);
			al_draw_line(
				savedx + (w.x * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z + w.size_y) * General::tileSize * General::scale) - oy,
				savedx + (w.x * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z - w.size_z) * General::tileSize * General::scale) - oy,
				white,
				1.0f
			);
			al_draw_line(
				savedx + ((w.x + w.size_x) * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z + w.size_y) * General::tileSize * General::scale) - oy,
				savedx + ((w.x + w.size_x) * General::tileSize * General::scale) - ox,
				savedy + ((w.y - w.z - w.size_z) * General::tileSize * General::scale) - oy,
				white,
				1.0f
			);
		}
	}
#endif

	if (levelEditor->is_marquee_marked() || levelEditor->is_marquee_floating()) {
		const int stiple_sz = 4;
		float f2 = fmod(al_get_time(), 2);
		float f = (f2 >= 1.0f) ? f2 - 1.0f : f2;
		int o = int(f * stiple_sz); // 0-3
		if (o == stiple_sz) {
			o = 0;
		}

		int x1, y1, x2, y2;

		std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > marquee;

		if (levelEditor->is_marquee_floating()) {
			marquee = levelEditor->get_floating_bounds(savedx, savedy, ox, oy);
		}
		else {
			if (levelEditor->is_marquee_marked()) {
				marquee = levelEditor->get_marked_bounds(savedx, savedy, ox, oy);
			}
		}

		levelEditor->get_marquee(&x1, &y1, &x2, &y2);

		if (marquee.size() > 0) {
			int sz = marquee.size()*2*(((General::tileSize*General::scale)/stiple_sz)+1);
			int sz2 = marquee_v.size();
			ALLEGRO_VERTEX v;
			for (int i = sz2; i < sz; i++) {
				marquee_v.push_back(v);
			}
			int vi = 0;
			for (size_t i = 0; i < marquee.size(); i++) {
				std::pair< std::pair<float, float>, std::pair<float, float> > &p = marquee[i];

				std::pair<float, float> &a = p.first;
				std::pair<float, float> &b = p.second;

				a.first = savedx + a.first - ox;
				a.second = savedy + a.second - oy;
				b.first = savedx + b.first - ox;
				b.second = savedy + b.second - oy;

				if (a.first > b.first) {
					float tmp = a.first;
					a.first = b.first;
					b.first = tmp;
				}
				if (a.second > b.second) {
					float tmp = a.second;
					a.second = b.second;
					b.second = tmp;
				}

				int n = (General::tileSize*General::scale) / stiple_sz;
				if (o != 0) {
					n++;
				}

				float x = a.first;
				float y = a.second;
				bool white;
				if (o != 0) {
					if (f2 < 1.0f) {
						white = false;
					}
					else {
						white = true;
					}
				}
				else {
					if (f2 < 1.0f) {
						white = true;
					}
					else {
						white = false;
					}
				}
				ALLEGRO_COLOR c_w = al_map_rgb(255, 255, 255);
				ALLEGRO_COLOR c_b = al_map_rgb(0, 0, 0);
				for (int i = 0; i < n; i++) {
					float l;
					if (i == 0 && o != 0) {
						l = o;
					}
					else if (i == n-1 && o != 0) {
						l = stiple_sz - o;
					}
					else {
						l = stiple_sz;
					}

					ALLEGRO_VERTEX v1, v2;

					v1.x = x;
					v1.y = y;
					v1.z = 0.0f;
					v1.color = white ? c_w : c_b;

					v2.x = (a.first != b.first) ? x+l : x;
					v2.y = (a.second != b.second) ? y+l : y;
					v2.z = 0.0f;
					v2.color = white ? c_w : c_b;

					if (!(v1.x < savedx || v1.y < savedy || v2.x < savedx || v2.y < savedy || v1.x > savedx+maxx || v1.y > savedy+maxy || v2.x > savedx+maxx || v2.y > savedy+maxy)) {
						marquee_v[vi] = v1;
						vi++;
						marquee_v[vi] = v2;
						vi++;
					}

					if (a.first != b.first) {
						x += l;
					}
					else {
						y += l;
					}
					white = !white;
				}
			}
			al_draw_prim(&marquee_v[0], NULL, NULL, 0, vi, ALLEGRO_PRIM_LINE_LIST);
		}
	}
}

std::string selectDir(const std::string &start)
{
	std::string tmp = start;
	tmp += std::string("/");
	ALLEGRO_FILECHOOSER *diag = al_create_native_file_dialog(tmp.c_str(), "Select Tile Set", 0, ALLEGRO_FILECHOOSER_FOLDER | ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	al_show_native_file_dialog(display, diag);

	tgui::clearKeyState();

	if (al_get_native_file_dialog_count(diag) != 1)
		return "";
	
	std::string path = al_get_native_file_dialog_path(diag, 0);

#ifdef __linux__NOPEDOESNTWORKANYMOREDOESNTNEEDTHIS
	int slash = strrchr(path.c_str(), '/') - path.c_str();
	if (slash >= 0) {
		path = path.substr(0, slash);
	}
#endif

	al_destroy_native_file_dialog(diag);
	return path;
}

bool tilesSort(ALLEGRO_PATH *a, ALLEGRO_PATH *b)
{
	const char *a_fn = al_get_path_filename(a);
	const char *b_fn = al_get_path_filename(b);
	
	int a_int = atoi(a_fn+5);
	int b_int = atoi(b_fn+5);
	
	return a_int < b_int;
}

#if defined ALLEGRO_MACOSX && defined MAS
void deleteTmpTiles(std::string path)
{
	mkdir(getUserResource("tmptiles"), 0777);
	
	ALLEGRO_FS_ENTRY *path_entry = al_create_fs_entry(getUserResource("tmptiles"));
	if (!al_open_directory(path_entry)) {
		char err[1024];
#ifdef ALLEGRO_WINDOWS
		strerror_s(err, 1024, errno);
#else
		strcpy(err, strerror(errno));
#endif
		fprintf(stderr, "failed to list directory '%s': %s\n", path.c_str(), err);
		return;
	}
	
	ALLEGRO_FS_ENTRY *ent = 0;
	while((ent = al_read_directory(path_entry))) {
		const char *ent_path = al_get_fs_entry_name(ent);
		
		ALLEGRO_PATH *path = al_create_path(ent_path);
		if (!path) {
			char err[1024];
#ifdef ALLEGRO_WINDOWS
			strerror_s(err, 1024, errno);
#else
			strcpy(err, strerror(errno));
#endif
			fprintf(stderr, "failed to create path object for loading tile sheet '%s': %s\n", ent_path, err);
			continue;
		}
		
		const char *ent_name = al_get_path_filename(path);
		if (ent_name[0] == '.') continue; // skip hidden files, the parent ref, and self ref entries

		unlink(ent_name);
	}
}
#endif

std::vector<ALLEGRO_PATH *> readSheetNames(const char *path, const char *ext)
{
	std::vector<ALLEGRO_PATH *> items;

/*
#if defined ALLEGRO_MACOSX && defined MAS
	if (num_tile_sheets != -1) {
		for (int i = 0; i < num_tile_sheets; i++) {
			char buf[MAX_PATH];
			snprintf(buf, MAX_PATH, "%s/tiles%d.%s", getUserResource("tmptiles"), i, png_sheets ? "png" : "tga");
			ALLEGRO_PATH *path = al_create_path(buf);
			items.push_back(path);
		}
		num_tile_sheets = 0;
		return items;
	}
#endif
*/

	ALLEGRO_FS_ENTRY *path_entry = al_create_fs_entry(path);
	if (!al_open_directory(path_entry)) {
		char err[1024];
#ifdef ALLEGRO_WINDOWS
		strerror_s(err, 1024, errno);
#else
		strcpy(err, strerror(errno));
#endif
		fprintf(stderr, "failed to list directory '%s': %s\n", path, err);
		return items;
	}
	
	ALLEGRO_FS_ENTRY *ent = 0;
	while((ent = al_read_directory(path_entry))) {
		const char *ent_path = al_get_fs_entry_name(ent);
		
		ALLEGRO_PATH *path = al_create_path(ent_path);
		if (!path) {
			char err[1024];
#ifdef ALLEGRO_WINDOWS
			strerror_s(err, 1024, errno);
#else
			strcpy(err, strerror(errno));
#endif
			fprintf(stderr, "failed to create path object for loading tile sheet '%s': %s\n", ent_path, err);
			continue;
		}
		
		const char *ent_name = al_get_path_filename(path);
		if (ent_name[0] == '.') continue; // skip hidden files, the parent ref, and self ref entries
		
		const char *ent_ext = al_get_path_extension(path);
		if (strcmp(ent_ext, ext) == 0 && strncmp(ent_name, "tiles", 5) == 0) {
			items.push_back(path);
		
			std::ifstream  src(ent_path, std::ios::binary);
			std::string dest = std::string(getUserResource("tmptiles")) + "/" + al_get_path_filename(path);
			std::ofstream  dst(dest,   std::ios::binary);

			dst << src.rdbuf();
		}
	}

#if defined ALLEGRO_MACOSX && defined MAS
	if (items.size() > 0) {
		if (std::string(ext) == ".png") {
			png_sheets = true;
		}
		else {
			png_sheets = false;
		}
	}
#endif

	return items;
}

void loadTileSheets(const char *path)
{
	std::string lastSaveName = levelEditor ? levelEditor->getLastSaveName() : "";
	bool maybe_reload_map = lastSaveName != "";
	int ts_w = 0;
	int ts_h = 0;
	if (maybe_reload_map) {
		if (tileSheets.size() > 0) {
			ts_w = al_get_bitmap_width(tileSheets[0]);
			ts_h = al_get_bitmap_height(tileSheets[0]);
		}
	}
	int new_w = 0;
	int new_h = 0;

	std::vector<ALLEGRO_PATH *> items = readSheetNames(path, ".tga");

	if (items.size() == 0) {
		items = readSheetNames(path, ".png");
	}
	
	std::sort(items.begin(), items.end(), tilesSort);
	
	tileSheets.clear();

	sheetCombo->clearValues();
	for(unsigned int i = 0; i < items.size(); i++) {
		ALLEGRO_BITMAP *bmp = al_load_bitmap(al_path_cstr(items[i], ALLEGRO_NATIVE_PATH_SEP));
		if (!bmp)
			break;

#if defined ALLEGRO_MACOSX && defined MAS
		if (num_tile_sheets != -1) {
			num_tile_sheets++;
		}
#endif
	
		al_convert_mask_to_alpha(bmp, al_map_rgb(255, 0, 255));
		tileSheets.push_back(bmp);
		al_destroy_path(items[i]);
		
		char buf[20];
		sprintf_s(buf, 20, "Sheet %d", i);
		sheetCombo->addValue(std::string(buf));
	}

	if (levelEditor) {
		levelEditor->tilesLoaded();
	}

	if (tileSheets.size() > 0) {
		int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
		int th = al_get_bitmap_height(tileSheets[0]) / General::tileSize;

		blank.clear();
			
		for (size_t i = 0; i < tileSheets.size(); i++) {
			al_lock_bitmap(tileSheets[i], ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
		}

		for (int y = 0; y < th; y++) {
			std::vector< std::vector<bool> > a;
			for (int x = 0; x < tw; x++) {
				std::vector<bool> b;
				for (size_t i = 0; i < tileSheets.size(); i++) {
					bool is_blank = true;
					for (int py = 0; py < General::tileSize; py++) {
						for (int px = 0; px < General::tileSize; px++) {
							ALLEGRO_COLOR pix = al_get_pixel(tileSheets[i], x*General::tileSize+px, y*General::tileSize+py);
							if (pix.a != 0.0f) {
								is_blank = false;
								break;
							}
						}
						if (is_blank == false) {
							break;
						}
					}
					if (is_blank) {
						b.push_back(true);
					}
					else {
						b.push_back(false);
					}
				}
				a.push_back(b);
			}
			blank.push_back(a);
		}

		for (size_t i = 0; i < tileSheets.size(); i++) {
			al_unlock_bitmap(tileSheets[i]);
		}

		new_w = al_get_bitmap_width(tileSheets[0]);
		new_h = al_get_bitmap_width(tileSheets[0]);
	}
}

static void add_help()
{
	if (quickRefFrame != NULL) {
		return;
	}

	disable_quickref_menu_item();

	int qr_w;
	int qr_h = 51*al_get_font_line_height(help_font);

	quickRefSplitter = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	quickRefBottomSplitter = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	quickRefContent1 = new A_Label(
		"\n"
		"P\n"
		"F\n"
		"C\n"
		"S\n"
		"K\n"
		"V\n"
		"M\n"
		"R\n"
		"\n"
		"Q\n"
		"W\n"
		HELP_CTRL "+A\n"
		HELP_CTRL "+D\n"
		HELP_CTRL "+Shift+I\n"
		HELP_CTRL "+C\n"
		HELP_CTRL "+X\n"
		"Delete\n"
		HELP_CTRL "+V\n"
		"\n"
#ifdef SUPPORT_GROUPS
		"G\n"
		HELP_ALT "+G\n"
		"\n"
#endif
		"T\n"
		HELP_CTRL "+T\n"
#ifdef SUPPORT_GROUPS
		//HELP_ALT "+T\n"
#endif
		"\n"
		"F1→F12\n"
		"1→0\n"
		"\n"
		HELP_CTRL "+Left\n"
		HELP_CTRL "+Right\n"
		HELP_ALT "+X\n"
		HELP_CTRL "+Up\n"
		HELP_CTRL "+Down\n"
		HELP_ALT "+Y\n"
		HELP_ALT "+Up\n"
		HELP_ALT "+Down\n"
		HELP_ALT "+L\n"
		"\n"
		HELP_CTRL "+N\n"
		HELP_CTRL "+O\n"
		HELP_CTRL "+S\n"
		HELP_CTRL "+Shift+S\n"
		"\n"
		HELP_CTRL "+Z\n"
		HELP_CTRL "+Y\n"
		"\n"
		"Enter\n"
		"\n",
		al_map_rgb(0xff, 0, 0x27)
	);
	quickRefContent2 = new A_Label(
		"\n"
		"Switch to the pencil tool (RMB to pick tile)\n"
		"Switch to fill tool (" HELP_ALT " for clear fill, " HELP_CTRL " for non-contiguous, RMB to test visible layers)\n"
		"Switch to the clear tool (Shift clears all layers)\n"
		"Switch to the solids tool\n"
		"Switch to the clone tool\n"
		"Switch to the layer mover tool\n"
		"Switch to the macro tool\n"
		"Start/stop recording a macro\n"
		"\n"
		"Switch to the marquee tool (Shift to add, " HELP_ALT " to subtract)\n"
		"Switch to the magic wand tool (Shift to add, " HELP_ALT " to subtract, " HELP_CTRL " for non-contiguous, RMB to test visible layers)\n"
		"Select all\n"
		"Select none\n"
		"Invert selection\n"
		"Copy (Shift for all layers)\n"
		"Cut (Shift for all layers)\n"
		"Delete (Shift for all layers)\n"
		"Paste\n"
		"\n"
#ifdef SUPPORT_GROUPS
		"Add a group (using marquee)\n"
		"Delete a group (using marquee)\n"
		"\n"
#endif
		"Toggle current layer drawing\n"
		"Toggle current layer solids drawing\n"
#ifdef SUPPORT_GROUPS
		//"Toggle current layer groups drawing\n"
#endif
		"\n"
		"Jump to layer/Toggle layer visibility\n"
		"Set the scale factor\n"
		"\n"
		"Insert a column at the cursor\n"
		"Insert a column after the cursor\n"
		"Delete the column at the cursor\n"
		"Insert a row at the cursor\n"
		"Insert a row after the cursor\n"
		"Delete the row at the cursor\n"
		"Insert a layer before the current layer\n"
		"Insert a layer after the current layer\n"
		"Delete the current layer\n"
		"\n"
		"Start a new map\n"
		"Open a map\n"
		"Save the map\n"
		"Save the map with a new name\n"
		"\n"
		"Undo\n"
		"Redo\n"
		"\n"
		"Save a screenshot as PNG (AshEdit-Screenshot-####.png)\n"
		"\n",
		al_map_rgb(0, 0, 0)
	);
	std::vector<std::string> lines = quickRefContent1->getLines();
	int max = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		int w = al_get_text_width(help_font, lines[i].c_str());
		if (w > max) {
			max = w;
		}
	}
	lines = quickRefContent2->getLines();
	int max2 = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		int w = al_get_text_width(help_font, lines[i].c_str());
		if (w > max2) {
			max2 = w;
		}
	}
	qr_w = max+75+max2+10+25+/*a little extra*/10;
	quickRefFrame = new A_Frame(al_map_rgb(255, 216, 0), qr_w, qr_h, quickRefContent1);
	int scr_w = al_get_display_width(display);
	int scr_h = al_get_display_height(display);
	int posx = scr_w/2 - qr_w/2;
	int posy = (scr_h/2 - qr_h/2) * 0.75f;
	if (posy < 0) {
		posy = 0;
	}
	quickRefFrame->setPosition(posx, posy);
	quickRefContent1->setFont(help_font);
	quickRefContent1->setIndent(35);
	quickRefContent2->setFont(help_font);
	quickRefTitlebar = new A_Titlebar(quickRefFrame, "Quick Reference", al_map_rgb(0, 0, 0), A_Titlebar::CLOSE_BUTTON);
	quickRefSplitter->addToFirstPane(quickRefTitlebar);
	quickRefSplitter->addToSecondPane(quickRefBottomSplitter);
	quickRefBottomSplitter->addToFirstPane(quickRefContent1);
	quickRefBottomSplitter->addToSecondPane(quickRefContent2);
	tgui::setNewWidgetParent(NULL);
	tgui::addWidget(quickRefFrame);
	tgui::setNewWidgetParent(quickRefFrame);
	tgui::addWidget(quickRefSplitter);
	quickRefSplitter->setSplitSize(28, -1);

	quickRefBottomSplitter->setSplitSize(max+75, -1);

	tgui::resize(NULL);
}

static void add_tile_size_getter()
{
	if (tileSizeGetterDummy != NULL) {
		return;
	}

	tileSizeGetterSplitter = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	tileSizeGetterContent = new TileSizeGetter(font, General::tileSize);
	tileSizeGetterDummy = new InputGetterDummy();
	int scr_w = al_get_display_width(display);
	int scr_h = al_get_display_height(display);
	int tsg_w = tileSizeGetterContent->getWidth();
	int tsg_h = tileSizeGetterContent->getHeight();
	int posx = scr_w/2 - tsg_w/2;
	int posy = (scr_h/2 - tsg_h/2) * 0.75f;
	if (posy < 0) {
		posy = 0;
	}
	tileSizeGetterDummy->setSize(tsg_w, tsg_h);
	tileSizeGetterDummy->setPosition(posx, posy);
	tileSizeGetterTitlebar = new A_Titlebar(tileSizeGetterContent, "Set Tile Size", al_map_rgb(0, 0, 0), A_Titlebar::CLOSE_BUTTON);
	tileSizeGetterSplitter->addToFirstPane(tileSizeGetterTitlebar);
	tileSizeGetterSplitter->addToSecondPane(tileSizeGetterContent);
	
	tgui::setNewWidgetParent(NULL);
	tgui::addWidget(tileSizeGetterDummy);
	tgui::setNewWidgetParent(tileSizeGetterDummy);
	tgui::addWidget(tileSizeGetterSplitter);

	tileSizeGetterSplitter->setSplitSize(28, -1);

	tgui::resize(NULL);
}

static void add_level_size_getter()
{
	if (levelSizeGetterDummy != NULL) {
		return;
	}

	levelSizeGetterSplitter = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	levelSizeGetterContent = new LevelSizeGetter(font);
	levelSizeGetterDummy = new InputGetterDummy();
	int scr_w = al_get_display_width(display);
	int scr_h = al_get_display_height(display);
	int lsg_w = levelSizeGetterContent->getWidth();
	int lsg_h = levelSizeGetterContent->getHeight();
	int posx = scr_w/2 - lsg_w/2;
	int posy = (scr_h/2 - lsg_h/2) * 0.75f;
	if (posy < 0) {
		posy = 0;
	}
	levelSizeGetterDummy->setSize(lsg_w, lsg_h);
	levelSizeGetterDummy->setPosition(posx, posy);
	levelSizeGetterTitlebar = new A_Titlebar(levelSizeGetterContent, "New Map", al_map_rgb(0, 0, 0), A_Titlebar::CLOSE_BUTTON);
	levelSizeGetterSplitter->addToFirstPane(levelSizeGetterTitlebar);
	levelSizeGetterSplitter->addToSecondPane(levelSizeGetterContent);
	
	tgui::setNewWidgetParent(NULL);
	tgui::addWidget(levelSizeGetterDummy);
	tgui::setNewWidgetParent(levelSizeGetterDummy);
	tgui::addWidget(levelSizeGetterSplitter);

	levelSizeGetterSplitter->setSplitSize(28, -1);

	tgui::resize(NULL);
}

void screenshot()
{
	float bak = General::scale;
	General::scale = 1.0f;
	levelEditor->resizeScrollpane();
	// Save whole area as png
	int w = levelEditor->getWidth()*General::tileSize*General::scale;
	int h = levelEditor->getHeight()*General::tileSize*General::scale;
	ALLEGRO_BITMAP *png = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *old_target = al_get_target_bitmap();

	al_set_target_bitmap(png);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	draw_yellow_and_purple = false;
	levelDrawCallback(0, 0, 0, 0, w, h, -1);
	draw_yellow_and_purple = true;
	al_set_target_bitmap(old_target);
	int i;
	char buf[MAX_PATH];
	for (i = 0; i < 9999; i++) {
#if defined ALLEGRO_UNIX || defined ALLEGRO_MACOSX
		sprintf_s(buf, MAX_PATH, "%s/AshEdit-Screenshot-%04d.png", getenv("HOME"), i);
#else
		sprintf_s(buf, MAX_PATH, "%s\\AshEdit-Screenshot-%04d.png", getenv("USERPROFILE"), i);
#endif
		if (al_filename_exists(buf)) continue;
		al_save_bitmap(buf, png);
		break;
	}
	al_destroy_bitmap(png);
	
	General::scale = bak;
	levelEditor->resizeScrollpane();

	char buf2[200];

	sprintf_s(buf2, 200, "(%s)", buf);

	al_show_native_message_box(display, "Screenshot Saved", "A screenshot has been saved!", buf2, 0, 0);
}

void new_map()
{
	int ret;
	if (levelEditor->getChanged()) {
		ret = al_show_native_message_box(display, "Warning", "The map has changed!", "Really create a new map?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
		tgui::clearKeyState();
	}
	else {
		ret = 1;
	}
	if (ret == 1) {
		add_level_size_getter();
	}
}

void new_map_real(int new_level_w, int new_level_h)
{
	levelEditor->new_level(new_level_w, new_level_h);
	setTitle();
	draw_solids.clear();
#ifdef SUPPORT_GROUPS
	//draw_groups.clear();
#endif
	for (int i = 0; i < levelEditor->getNumLayers(); i++) {
		draw_solids.push_back(true);
#ifdef SUPPORT_GROUPS
		//draw_groups.push_back(true);
#endif
	}
	
	layerCombo->setNumItems(levelEditor->getNumLayers());
	layerCombo->setSelected(levelEditor->getLayer());

	levelEditor->resizeScrollpane();
	levelScrollpane->scrollBy(-1000000, -1000000);
}

void do_open()
{
	int ret;
	if (levelEditor->getChanged()) {
		ret = al_show_native_message_box(display, "Warning", "The map has changed!", "Really load a new map?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
		tgui::clearKeyState();
	}
	else {
		ret = 1;
	}
	if (ret == 1) {
		if (levelEditor->load()) {
			setTitle();
			draw_solids.clear();
#ifdef SUPPORT_GROUPS
			//draw_groups.clear();
#endif
			for (int i = 0; i < levelEditor->getNumLayers(); i++) {
				draw_solids.push_back(true);
#ifdef SUPPORT_GROUPS
				//draw_groups.push_back(true);
#endif
			}

			layerCombo->setNumItems(levelEditor->getNumLayers());
			layerCombo->setSelected(levelEditor->getLayer());
			
			levelEditor->resizeScrollpane();
			levelScrollpane->scrollBy(-1000000, -1000000);
		}
	}
}

void choose_help_font()
{
	if (al_get_display_width(display) >= 1900 && al_get_display_height(display) >= 900) {
		help_font = help_font_big;
	}
	else if (al_get_display_width(display) >= 1200 && al_get_display_height(display) >= 700) {
		help_font = help_font_small;
	}
	else {
		help_font = help_font_tiny;
	}
}

void toggleCheckboxes(int selected) {
	int ids[] = {
		SCALE_1_ID,
		SCALE_2_ID,
		SCALE_3_ID,
		SCALE_4_ID,
		SCALE_5_ID,
		SCALE_6_ID,
		SCALE_7_ID,
		SCALE_8_ID,
		SCALE_9_ID,
		SCALE_10_ID
	};

	for (int i = 1; i <= 10; i++) {
		if (i == selected) {
			al_set_menu_item_flags(menu, ids[i-1], ALLEGRO_MENU_ITEM_CHECKBOX | ALLEGRO_MENU_ITEM_CHECKED);
		}
		else {
			al_set_menu_item_flags(menu, ids[i-1], ALLEGRO_MENU_ITEM_CHECKBOX);
		}
	}
}

void destroy_recent()
{
	ALLEGRO_MENU *m = al_find_menu(menu, FILE_OPEN_RECENT_ID);
	for (size_t i = 0; i < recent_files.size(); i++) {
		al_remove_menu_item(m, 0);
	}
}

void build_recent()
{
	ALLEGRO_MENU *m = al_find_menu(menu, FILE_OPEN_RECENT_ID);
	for (size_t i = 0; i < recent_files.size(); i++) {
		std::string s = recent_files[i];
		if (s.length() > 100) {
			s = s.substr(0, 100);
			s += "...";
		}
		al_append_menu_item(m, s.c_str(), int16_t(RECENT_ID_START+i), 0, nullptr, nullptr);
	}
}

void add_to_recent(std::string fn, bool build, bool at_back)
{
	if (build) {
		destroy_recent();
	}
	std::vector<std::string>::iterator it = std::find(recent_files.begin(), recent_files.end(), fn);
	if (it != recent_files.end()) {
		recent_files.erase(it);
	}
	if (at_back) {
		recent_files.push_back(fn);
	}
	else {
		recent_files.insert(recent_files.begin(), fn);
	}
	if (recent_files.size() > NUM_RECENT) {
		recent_files.erase(recent_files.begin()+NUM_RECENT);
	}
	if (build) {
		build_recent();
	}
}

#ifdef ALLEGRO_UNIX
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <allegro5/debug.h>
#define ASSERT ALLEGRO_ASSERT

extern "C" {
	GtkWidget *_al_gtk_get_window(ALLEGRO_DISPLAY *display);
}

void set_icon_linux(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *bitmap)
{
   int w, h, x, y;
   int ncolors, nrows;
   char **xpm;
   char buf[100];
   int i;

   ALLEGRO_LOCKED_REGION *lr = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READONLY);
   if (lr == NULL)
      return;

   std::vector<uint32_t> v;

   w = al_get_bitmap_width(bitmap);
   h = al_get_bitmap_height(bitmap);

   for (y = 0; y < h; y++) {
      for (x = 0; x < w; x++) {
         uint32_t c = *(uint32_t *)((((char *)lr->data) + lr->pitch * y + x * 4));
         int alpha = (c >> 24) & 0xff;
         if (alpha != 255) {
                 c = 0;
         }
         int sz = v.size();
         bool found = false;
         for (i = 0; i < sz; i++) {
            if (v[i] == c) {
               found = true;
               break;
            }
         }
         if (found == false) {
	    v.push_back(c);
         }
      }
   }

   ncolors = v.size();
   nrows = 2 + ncolors + h;

   xpm = (char **)malloc(nrows * sizeof(char *));
   if (xpm == NULL)
      return;

   snprintf(buf, 100, "%d %d %d 8", w, h, ncolors + 1);

   xpm[0] = strdup(buf);

   xpm[1] = strdup("00000000\tc None");

   for (i = 0; i < ncolors; i++) {
        uint32_t c = v[i];
        int r = c & 0xff;
        int g = (c >> 8) & 0xff;
        int b = (c >> 16) & 0xff;
        snprintf(buf, 100, "%08x\tc #%02x%02x%02x", i+1, r, g, b);
        xpm[i+2] = strdup(buf);
   }

   for (y = 0; y < h; y++) {
        int row = y + 2 + ncolors;
        xpm[row] = (char *)malloc(8 * w + 1);
        xpm[row][8 * w] = 0;
        uint32_t *p = (uint32_t *)(((char *)lr->data) + lr->pitch * y);
        for (x = 0; x < w; x++) {
                uint32_t pixel = *p;
                int alpha = (pixel >> 24) & 0xff;
                if (alpha != 255) {
                   snprintf(buf, 100, "%s", "00000000");
                }
                else {
                   for (i = 0; i < (int)v.size(); i++) {
                      uint32_t pixel2 = v[i];
                      if (pixel == pixel2)
                         break;
                   }
                   snprintf(buf, 100, "%08x", i+1);
                }
                for (i = 0; i < 8; i++) {
                        xpm[row][8*x+i] = buf[i];
                }
                p++;
        }
   }

   al_unlock_bitmap(bitmap);

   GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)xpm);
   GtkWindow *window = (GtkWindow *)_al_gtk_get_window(display);
   gtk_window_set_icon(window, pixbuf);
}
#endif

#ifdef ALLEGRO_UNIX
gboolean
window_state_callback (GtkWidget *widget,
               GdkEvent  *event,
               gpointer   user_data)
{
	GdkEventWindowState *s = (GdkEventWindowState *)event;
	if (s->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
		window_maximized = true;
	}
	else {
		window_maximized = false;
	}

	return FALSE;
}

#if 0
bool x11_is_maximized(ALLEGRO_DISPLAY *d)
{
   ALLEGRO_SYSTEM_XGLX *system = (ALLEGRO_SYSTEM_XGLX *)al_get_system_driver();
   ALLEGRO_DISPLAY_XGLX *glx = (ALLEGRO_DISPLAY_XGLX *)d;
   Window xwindow = glx->window;
   Display *display = system->x11display;
    Atom _NET_WM_STATE = X11_ATOM(_NET_WM_STATE);
    Atom _NET_WM_STATE_HIDDEN = X11_ATOM(_NET_WM_STATE_HIDDEN);
    Atom _NET_WM_STATE_FOCUSED = X11_ATOM(_NET_WM_STATE_FOCUSED);
    Atom _NET_WM_STATE_MAXIMIZED_VERT = X11_ATOM(_NET_WM_STATE_MAXIMIZED_VERT);
    Atom _NET_WM_STATE_MAXIMIZED_HORZ = X11_ATOM(_NET_WM_STATE_MAXIMIZED_HORZ);
    Atom _NET_WM_STATE_FULLSCREEN = X11_ATOM(_NET_WM_STATE_FULLSCREEN);
    Atom actualType;
    int actualFormat;
    unsigned long i, numItems, bytesAfter;
    unsigned char *propertyValue = NULL;
    long maxLength = 1024;
    bool max = false;

    if (XGetWindowProperty(display, xwindow, _NET_WM_STATE,
                           0l, maxLength, False, XA_ATOM, &actualType,
                           &actualFormat, &numItems, &bytesAfter,
                           &propertyValue) == Success) {
        Atom *atoms = (Atom *) propertyValue;
        int maximized = 0;
        int fullscreen = 0;

        for (i = 0; i < numItems; ++i) {
            if (atoms[i] == _NET_WM_STATE_HIDDEN) {
                //flags |= SDL_WINDOW_HIDDEN;
            } else if (atoms[i] == _NET_WM_STATE_FOCUSED) {
                //flags |= SDL_WINDOW_INPUT_FOCUS;
            } else if (atoms[i] == _NET_WM_STATE_MAXIMIZED_VERT) {
                maximized |= 1;
            } else if (atoms[i] == _NET_WM_STATE_MAXIMIZED_HORZ) {
                maximized |= 2;
            } else if ( atoms[i] == _NET_WM_STATE_FULLSCREEN) {
                fullscreen = 1;
            }
        }
        if (maximized == 3) {
		max = true;
        }

        if (fullscreen == 1) {
            //flags |= SDL_WINDOW_FULLSCREEN;
        }

        /* If the window is unmapped, numItems will be zero and _NET_WM_STATE_HIDDEN
         * will not be set. Do an additional check to see if the window is unmapped
         * and mark it as SDL_WINDOW_HIDDEN if it is.
         */
	/*
        {
            XWindowAttributes attr;
            SDL_memset(&attr,0,sizeof(attr));
            X11_XGetWindowAttributes(videodata->display, xwindow, &attr);
            if (attr.map_state == IsUnmapped) {
                flags |= SDL_WINDOW_HIDDEN;
            }
        }
	*/
        XFree(propertyValue);
    }

    /* FIXME, check the size hints for resizable */
    /* flags |= SDL_WINDOW_RESIZABLE; */

    return max;
}
#endif
#endif

void maybe_remove_input_getters()
{
	if (tileSizeGetterDummy != NULL) {
		tileSizeGetterDummy->remove();
		delete tileSizeGetterTitlebar;
		delete tileSizeGetterContent;
		delete tileSizeGetterDummy;
		tileSizeGetterTitlebar = NULL;
		tileSizeGetterContent = NULL;
		tileSizeGetterDummy = NULL;
	}
	if (levelSizeGetterDummy != NULL) {
		levelSizeGetterDummy->remove();
		delete levelSizeGetterTitlebar;
		delete levelSizeGetterContent;
		delete levelSizeGetterDummy;
		levelSizeGetterTitlebar = NULL;
		levelSizeGetterContent = NULL;
		levelSizeGetterDummy = NULL;
	}
}

void get_ss(int *x, int *y)
{
	int w = al_get_bitmap_width(tileSheets[0]);
	int h = al_get_bitmap_height(tileSheets[0]);

	int wt = w / General::tileSize;
	int ht = h / General::tileSize;

	// The level file format prohibits > 256 tiles per sheet
	wt = MIN(256, wt);
	ht = MIN(256, ht);

	*x = wt * General::tileSize * General::scale;
	*y = ht * General::tileSize * General::scale;
}

int main(int argc, char **argv)
{
#if !defined ALLEGRO_IPHONE
	al_set_org_name("Nooskewl");
	al_set_app_name("AshEdit");
#endif

	// Initialize Allegro
	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_native_dialog_addon();

	al_init_user_event_source(&evtsrc);

	std::string fn = getUserResource("config.txt");

	last_scale = General::scale;

	FILE *cfg;
#ifdef ALLEGRO_WINDOWS
	errno_t err = fopen_s(&cfg, fn.c_str(), "r");
	if (err == 0) {
#else
	cfg = fopen(fn.c_str(), "r");
	if (cfg) {
#endif
		char buf[1000];
		char key[1000];
		char val[1000];
		std::map<int, std::string> recent;
		while (fgets(buf, 1000, cfg)) {
			if (feof(cfg)) {
				break;
			}
			int i;
			for (i = 0; buf[i] != 0 && buf[i] != '='; i++) {
				key[i] = buf[i];
			}
			key[i] = 0;
			if (buf[i] != 0) {
				i++;
				int j = 0;
				for (; buf[i] != 0 && buf[i] != '\n'; i++) {
					val[j++] = buf[i];
				}
				val[j] = 0;
			}
			std::string k = key;
			if (k == "window_x") {
				window_x = atoi(val);
			}
			else if (k == "window_y") {
				window_y = atoi(val);
			}
			else if (k == "window_w") {
				window_w = atoi(val);
			}
			else if (k == "window_h") {
				window_h = atoi(val);
			}
			else if (k == "window_maximized") {
				window_maximized = atoi(val) == 1 ? true : false;
			}
			else if (k == "tile_size") {
				my_tile_size = atoi(val);
				save_tile_size = my_tile_size;
				if (my_tile_size > 0) {
					General::tileSize = my_tile_size;
				}
			}
			else if (k == "last_scale") {
				last_scale = atoi(val);
				General::scale = last_scale;
			}
			else if (k == "tile_path") {
				tile_load_path = val;
				save_tile_load_path = tile_load_path;
			}
#if defined ALLEGRO_MACOSX && defined MAS
			else if (k == "num_tile_sheets_mac") {
				num_tile_sheets = atoi(val);
			}
			else if (k == "png_sheets_mac") {
				png_sheets = (bool)atoi(val);
			}
#endif
			else if (k.substr(0, 6) == "recent") {
				int n = atoi(k.substr(6).c_str());
				recent[n] = val;
			}
		}
		fclose(cfg);

		int count = 0;
		std::map<int, std::string>::iterator it = recent.begin();
		
		for (; count < NUM_RECENT && it != recent.end(); count++, it++) {
			std::pair<int, std::string> p = *it;
			add_to_recent(p.second, false, true);
		}
	}
			
	ALLEGRO_MONITOR_INFO mi;
	al_get_monitor_info(0, &mi);

#ifdef ALLEGRO_GTK_TOPLEVEL
	/* ALLEGRO_GTK_TOPLEVEL is necessary for menus with GTK. */
	al_set_new_display_flags(LLEGRO_RESIZABLE | ALLEGRO_GTK_TOPLEVEL | ALLEGRO_GENERATE_EXPOSE_EVENTS);
#elif defined ALLEGRO_MACOSX
	int max_flag = window_maximized ? ALLEGRO_MAXIMIZED : 0;
	al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS | max_flag);
#else
	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS);
#endif

	if (window_maximized) {
		window_w = DEFAULT_WINDOW_W;
		window_h = DEFAULT_WINDOW_H;
	}

	if (window_maximized == false) {
		if (window_x >= 0 || window_y >= 0) {
			if (window_x < mi.x2-window_w/2 && window_y < mi.y2-window_h/2) {
				al_set_new_window_position(window_x, window_y);
			}
		}
	}

	display = al_create_display(window_w, window_h);
	
	try {

#ifdef ALLEGRO_WINDOWS
	// This reads the registry to determine if reverse mouse wheel scrolling is on/off
	HKEY key;
	DWORD disp;
	LSTATUS status0 = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PrecisionTouchPad",
		0,
		NULL,
		0,
		KEY_QUERY_VALUE,
		NULL,
		&key,
		&disp
	);
	if (status0 == ERROR_SUCCESS) {
		DWORD dw_sz = sizeof(DWORD);
		DWORD dw_res = 0;
		LSTATUS status = RegQueryValueExA(
			key,
			"ScrollDirection",
			0,
			NULL,
			reinterpret_cast<LPBYTE>(&dw_res),
			&dw_sz
		);
		if (status == ERROR_SUCCESS) {
			if (dw_res == 0) {
				reverse_scroll = true;
			}
			else {
				reverse_scroll = false;
			}
		}
	}
#endif
 
	ALLEGRO_FILE *pencil_f = al_open_memfile(pencil_png, pencil_png_len, "rb");
	ALLEGRO_FILE *clear_f = al_open_memfile(clear_png, clear_png_len, "rb");
	ALLEGRO_FILE *clone_f = al_open_memfile(clone_png, clone_png_len, "rb");
	ALLEGRO_FILE *fill_f = al_open_memfile(fill_png, fill_png_len, "rb");
	ALLEGRO_FILE *macro_f = al_open_memfile(macro_png, macro_png_len, "rb");
	ALLEGRO_FILE *magic_wand_f = al_open_memfile(magic_wand_png, magic_wand_png_len, "rb");
	ALLEGRO_FILE *marquee_f = al_open_memfile(marquee_png, marquee_png_len, "rb");
	ALLEGRO_FILE *mover_f = al_open_memfile(mover_png, mover_png_len, "rb");
	ALLEGRO_FILE *record_f = al_open_memfile(record_png, record_png_len, "rb");
	ALLEGRO_FILE *screenshot_f = al_open_memfile(screenshot_png, screenshot_png_len, "rb");
	ALLEGRO_FILE *solids_f = al_open_memfile(solids_png, solids_png_len, "rb");
	ALLEGRO_FILE *toggle_layer_f = al_open_memfile(toggle_layer_png, toggle_layer_png_len, "rb");
	ALLEGRO_FILE *toggle_solids_f = al_open_memfile(toggle_solids_png, toggle_solids_png_len, "rb");

	int flags = al_get_new_bitmap_flags();
	al_set_new_bitmap_flags(flags | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	pencil_icon = al_load_bitmap_f(pencil_f, NULL);
	clear_icon = al_load_bitmap_f(clear_f, NULL);
	clone_icon = al_load_bitmap_f(clone_f, NULL);
	fill_icon = al_load_bitmap_f(fill_f, NULL);
	macro_icon = al_load_bitmap_f(macro_f, NULL);
	magic_wand_icon = al_load_bitmap_f(magic_wand_f, NULL);
	marquee_icon = al_load_bitmap_f(marquee_f, NULL);
	mover_icon = al_load_bitmap_f(mover_f, NULL);
	record_icon = al_load_bitmap_f(record_f, NULL);
	screenshot_icon = al_load_bitmap_f(screenshot_f, NULL);
	solids_icon = al_load_bitmap_f(solids_f, NULL);
	toggle_layer_icon = al_load_bitmap_f(toggle_layer_f, NULL);
	toggle_solids_icon = al_load_bitmap_f(toggle_solids_f, NULL);

	al_set_new_bitmap_flags(flags);

	if (pencil_icon == NULL ||
		clear_icon == NULL ||
		clone_icon == NULL ||
		fill_icon == NULL ||
		macro_icon == NULL ||
		magic_wand_icon == NULL ||
		marquee_icon == NULL ||
		mover_icon == NULL ||
		record_icon == NULL ||
		screenshot_icon == NULL ||
		solids_icon == NULL ||
		toggle_layer_icon == NULL ||
		toggle_solids_icon == NULL) {
		exit(1);
	}

	al_fclose(pencil_f);
	al_fclose(clear_f);
	al_fclose(clone_f);
	al_fclose(fill_f);
	al_fclose(macro_f);
	al_fclose(magic_wand_f);
	al_fclose(marquee_f);
	al_fclose(mover_f);
	al_fclose(record_f);
	al_fclose(screenshot_f);
	al_fclose(solids_f);
	al_fclose(toggle_layer_f);
	al_fclose(toggle_solids_f);

	menu = al_build_menu(main_menu_info);
	build_recent();
	al_set_display_menu(display, menu);

	al_set_window_constraints(display, 960, 540, 0, 0);
	al_apply_window_constraints(display, true);

#ifdef ALLEGRO_WINDOWS
	font = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 16, 0);
	help_font_tiny = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 9, 0);
	help_font_small = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 13, 0);
	help_font_big = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 16, 0);
#elif defined ALLEGRO_MACOSX
	font = al_load_ttf_font("font.ttf", 16, 0);
	help_font_tiny = al_load_ttf_font("font.ttf", 9, 0);
	help_font_small = al_load_ttf_font("font.ttf", 11, 0);
	help_font_big = al_load_ttf_font("font.ttf", 16, 0);
#else
	font = al_load_ttf_font("font.ttf", 16, 0);
	help_font_tiny = al_load_ttf_font("font.ttf", 9, 0);
	help_font_small = al_load_ttf_font("font.ttf", 13, 0);
	help_font_big = al_load_ttf_font("font.ttf", 16, 0);
#endif
	choose_help_font();

	queue = al_create_event_queue();

	al_register_event_source(queue, &evtsrc);

	ALLEGRO_FILE *f = al_open_memfile(icon_png, sizeof(icon_png), "rb");
	ALLEGRO_BITMAP *icon_bmp = al_load_bitmap_f(f, ".png");
#ifndef __APPLE__
#ifdef ALLEGRO_UNIX
	GtkWindow *gtkwindow = (GtkWindow *)_al_gtk_get_window(display);
	g_signal_connect (gtkwindow, "window-state-event", (GCallback) window_state_callback, NULL);
	set_icon_linux(display, icon_bmp);
#else
	al_set_display_icon(display, icon_bmp);
#endif
#endif
	al_destroy_bitmap(icon_bmp);
	al_fclose(f);

	f = al_open_memfile(mouse_cursor_png, sizeof(mouse_cursor_png), "rb");
	ALLEGRO_BITMAP *mouse_cursor_bmp = al_load_bitmap_f(f, ".png");
	mouse_cursor = al_create_mouse_cursor(mouse_cursor_bmp, 0, 0);
	al_set_mouse_cursor(display, mouse_cursor);
	al_destroy_bitmap(mouse_cursor_bmp);
	al_fclose(f);

	f = al_open_memfile(moved_tile_png, sizeof(moved_tile_png), "rb");
	moved_tile_bmp = al_load_bitmap_f(f, ".png");
	al_fclose(f);

	f = al_open_memfile(solid_png, sizeof(solid_png), "rb");
	solid_bmp = al_load_bitmap_f(f, ".png");
	al_fclose(f);

	timer = al_create_timer(1.0f/10.0f);
	menu_item_timer = al_create_timer(1.0f/10.0f);

	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(menu_item_timer));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_default_menu_event_source());

	tgui::init(display);
	tgui::setFont(font);

#if !defined ALLEGRO_MACOSX && !defined ALLEGRO_UNIX // on Mac, this causes the window to jump behind all others so we use the flag above
	if (window_maximized) {
		al_set_display_flag(display, ALLEGRO_MAXIMIZED, true);
	}
	else 
#endif
	{
		// The set_new_window_pos above doesn't quite work, but it gets it closer... need both :(
		if (window_x >= 0 || window_y >= 0) {
			if (window_x < mi.x2-window_w/2 && window_y < mi.y2-window_h/2) {
				al_set_window_position(display, window_x, window_y);
			}
		}
	}

	buffer1 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	buffer2 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	buffer3 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	buffer4 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	buffer5 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	buffer6 = new A_Splitter(A_Splitter::SPLIT_VERTICAL);

	A_Splitter *toolbarSplitter = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	A_Splitter *mainSplitBottom= new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	A_Splitter *canvasSplit = new A_Splitter(A_Splitter::SPLIT_VERTICAL);
	A_Splitter *canvasSplitLeft = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	A_Splitter *canvasSplitRight = new A_Splitter(A_Splitter::SPLIT_HORIZONTAL);
	canvasSplit->setResizable(true);

	layerCombo = new A_Combobox(
		7,
		al_map_rgb(0, 216, 255),
		al_map_rgb(0, 0, 0),
		al_map_rgb(0, 0xa2, 0xbf)
	);
	layerCombo->set_callback(layer_callback);
	sheetCombo = new A_Combobox(
		7,
		al_map_rgb(0, 216, 255),
		al_map_rgb(0, 0, 0),
		al_map_rgb(0, 0xa2, 0xbf)
	);
	for (int i = 0; i < General::startLayers; i++) {
		char buf[20];
		sprintf_s(buf, 20, "Layer %d", i);
		layerCombo->addValue(std::string(buf));
	}

	int tile_arg_index = -1;
	int level_arg_index = -1;

	int i = 1;
	while (argc > i && argv[i][0] == '-') {
		if (!strcmp(argv[i], "-tile_size") && argc > i+1) {
			my_tile_size = atoi(argv[i+1]);
			General::tileSize = my_tile_size;
			i += 2;
		}
		else if (!strcmp(argv[i], "-tiles") && argc > i+1) {
			tile_arg_index = i + 1;
			i += 2;
		}
		else if (!strcmp(argv[i], "-load") && argc > i+1) {
			level_arg_index = i + 1;
			i += 2;
		}
		else if (!strcmp(argv[i], "-mo3")) {
			command_line_level_type = MO3;
			i++;
		}
		else if (!strcmp(argv[i], "-mo2")) {
			command_line_level_type = MO2;
			i++;
		}
		else if (!strcmp(argv[i], "-cp")) {
			command_line_level_type = CP;
			i++;
		}
#ifdef DUMP
		else if (!strcmp(argv[i], "-dump") && argc > i+1) {
			dump = true;
			std::string type = argv[i+1];
			if (type == "mo3") {
				dump_type = MO3;
			}
			else if (type == "mo2") {
				dump_type = MO2;
			}
			else if (type == "cp") {
				dump_type = CP;
			}
			else {
				dump_type = WM2;
			}
			i += 2;
		}
#endif
		else if (!strcmp(argv[i], "-use-filename-based-level-types")) {
			use_filename_based_level_types = true;
			i++;
		}
	}

	if (tile_arg_index > 0) {
		tile_load_path = argv[tile_arg_index];
	}
	loadTileSheets(tile_load_path.c_str());

	levelScrollpane = new A_Scrollpane(
		al_map_rgb_f(0.5f, 0.5f, 0.5f),
		al_map_rgb(0xcc, 0xcc, 0xcc)
	);
	tileScrollpane = new A_Scrollpane(
		al_map_rgb_f(0.5f, 0.5f, 0.5f),
		al_map_rgb(0xcc, 0xcc, 0xcc)
	);
	tileSelector = new A_Tileselector(tileDrawCallback);
	tileScrollpane->addScrollable(tileSelector);
	if (tileSheets.size()) {
		int pane_size_x, pane_size_y;
		get_ss(&pane_size_x, &pane_size_y);
		tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
	}
	levelEditor = new A_Leveleditor(levelDrawCallback, General::startLayers, tileSelector);
	levelScrollpane->addScrollable(levelEditor);
	levelScrollpane->setScrollSize(
		levelEditor->getWidth()*(General::tileSize*General::scale),
		levelEditor->getHeight()*(General::tileSize*General::scale)
	);

	A_Label *statusLabel = new A_Label("Status", al_map_rgb(0, 0, 0));
	statusLabel->setStatus(true);

	setTitle();
	for (int i = 0; i < levelEditor->getNumLayers(); i++) {
		draw_solids.push_back(true);
#ifdef SUPPORT_GROUPS
		//draw_groups.push_back(true);
#endif
	}

#ifdef SUPPORT_WALLS
	draw_walls = true;
#endif

	A_Toolbar *toolbar = new A_Toolbar();

	tgui::addPostDrawWidget(toolbar);

	tgui::setNewWidgetParent(0);
	tgui::addWidget(toolbarSplitter);
	toolbarSplitter->addToFirstPane(buffer3);
	buffer3->addToFirstPane(new A_Blank(true, false));
	buffer3->addToSecondPane(buffer4);
	buffer4->addToFirstPane(toolbar);
	buffer4->addToSecondPane(new A_Blank(false, false));
	toolbarSplitter->addToSecondPane(mainSplitBottom);
	mainSplitBottom->addToSecondPane(buffer5);
	buffer5->addToFirstPane(new A_Blank(true, false));
	buffer5->addToSecondPane(buffer6);
	buffer6->addToFirstPane(statusLabel);
	buffer6->addToSecondPane(new A_Blank(false, false));
	mainSplitBottom->addToFirstPane(buffer1);
	buffer1->addToFirstPane(new A_Blank(true, true));
	buffer1->addToSecondPane(buffer2);
	buffer2->addToFirstPane(canvasSplit);
	buffer2->addToSecondPane(new A_Blank(false, true));
	canvasSplit->addToFirstPane(canvasSplitLeft);
	canvasSplit->addToSecondPane(canvasSplitRight);
	canvasSplitLeft->addToFirstPane(layerCombo);
	canvasSplitLeft->addToSecondPane(levelScrollpane);
	canvasSplitRight->addToFirstPane(sheetCombo);
	canvasSplitRight->addToSecondPane(tileScrollpane);

	tgui::setFocus(mainSplitBottom);

	int ss = window_maximized ? 5 : 0;

	buffer1->setSplitSize(ss, -1);
	buffer2->setSplitSize(-1, ss);
	buffer3->setSplitSize(ss, -1);
	buffer4->setSplitSize(-1, ss);
	buffer5->setSplitSize(ss, -1);
	buffer6->setSplitSize(-1, ss);
	toolbarSplitter->setSplitSize(51, -1);
	mainSplitBottom->setSplitSize(-1, 30);
	canvasSplitLeft->setSplitSize(28, -1);
	canvasSplitRight->setSplitSize(28, -1);

#ifdef ALLEGRO_UNIX
	gtk_widget_set_size_request((GtkWidget *)gtkwindow, 960, 540);
	al_acknowledge_resize(display); // hack: I think it's the native menu that makes this required - if I don't do this, the combo boxes are missing and mouse coords are messed up until the display is resized
	tgui::resize(NULL); // similar to java pack(), on all widgets
	if (level_arg_index > 0) {
		std::string level_data = argv[level_arg_index];
		levelEditor->load(level_data);
		levelEditor->setLastSaveName(level_data);
	}
	draw(true); // hack
	al_acknowledge_resize(display); // hack: I think it's the native menu that makes this required - if I don't do this, the combo boxes are missing and mouse coords are messed up until the display is resized
	draw();
	if (window_maximized) {
		gtk_window_maximize(gtkwindow);
	}
#else
	tgui::resize(NULL);
	if (level_arg_index > 0) {
		std::string level_data = argv[level_arg_index];
		levelEditor->load(level_data);
		levelEditor->setLastSaveName(level_data);
	}
	draw();
#endif

	toggleCheckboxes(last_scale);

#ifdef ALLEGRO_MACOSX
	[NSApp activateIgnoringOtherApps:YES];
#endif

	while (true) {
		bool do_add_help = false;
	
		bool marked = (levelEditor->is_marquee_marked() || levelEditor->is_marquee_floating());
		if (marked != marquee_marked) {
			marquee_marked = marked;
			if (marked) {
				if (al_get_timer_started(timer)) {
					al_resume_timer(timer);
				}
				else {
					al_start_timer(timer);
				}
				timer_ticking = true;
			}
			else {
				al_stop_timer(timer);
				timer_ticking = false;
			}
		}

		if (menu_item_timer_ticking) {
			std::vector<EnableDisableMenuItem>::iterator it;
			for (it = enable_menu_items.begin(); it != enable_menu_items.end();) {
				EnableDisableMenuItem m = *it;
				if (al_get_time() >= m.when) {
					al_set_menu_item_flags(menu, m.id, 0);
					it = enable_menu_items.erase(it);
				}
				else {
					it++;
				}
			}
			for (it = disable_menu_items.begin(); it != disable_menu_items.end();) {
				EnableDisableMenuItem m = *it;
				if (al_get_time() >= m.when) {
					al_set_menu_item_flags(menu, m.id, ALLEGRO_MENU_ITEM_DISABLED);
					it = disable_menu_items.erase(it);
				}
				else {
					it++;
				}
			}
			if (enable_menu_items.size() == 0 && disable_menu_items.size() == 0) {
				al_stop_timer(menu_item_timer);
				menu_item_timer_ticking = false;
			}
		}

		ALLEGRO_EVENT event;
		do {
			bool skip = false;

			al_wait_for_event(queue, &event);

			// this hides tooltips when the mouse hovers over the menubar
#ifdef ALLEGRO_WINDOWS
			{
				WINDOWINFO wi;
				POINT p;

				GetWindowInfo(al_get_win_window_handle(display), &wi);
				GetCursorPos(&p);

				if (p.y < wi.rcClient.top) {
					toolbar->mouseMove(-1, -1, -1, -1); // a bit of a hack
				}
			}
#endif

#ifdef ALLEGRO_MACOSX
			{
				// hide tooltips when mouse hovers over menubar
				ALLEGRO_MOUSE_STATE s;
				al_get_mouse_state(&s);
				if (s.y < 0) {
					toolbar->mouseMove(-1, -1, -1, -1);
				}
			}
#endif

			if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
				timer_end_time = al_get_time() + 5.0;
				if (timer_ticking == false) {
					if (al_get_timer_started(timer)) {
						al_resume_timer(timer);
					}
					else {
						al_start_timer(timer);
					}
					timer_ticking = true;
				}
			}

			if (marked == false && timer_ticking && al_get_time() > timer_end_time) {
				al_stop_timer(timer);
				timer_ticking = false;
			}
			
			if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
				maybe_remove_input_getters();
				al_acknowledge_resize(event.display.source);
				resize = true;
				draw();
				choose_help_font();
				if (quickRefFrame != NULL) {
					pushEvent(TGUI_EVENT_OBJECT, quickRefTitlebar);
					do_add_help = true;
				}
				// FIXME:!
				/*
				if (newAreaFrame != NULL) {
					pushEvent(TGUI_EVENT_OBJECT, newAreaTitlebar);
				}
				if (rightClickPopup != NULL) {
					pushEvent(TGUI_EVENT_OBJECT, rightClickTitlebar);
				}
				if (addRemoveRowCol != NULL) {
					pushEvent(TGUI_EVENT_OBJECT, addRemoveRowColTitlebar);
				}
				if (tileSizePopup != NULL) {
					pushEvent(TGUI_EVENT_OBJECT, tileSizePopupTitlebar);
				}
				*/
			}
#ifdef ALLEGRO_MACOSX
			// FIXME: this doesn't work (anymore?)
			else if (event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
				al_set_mouse_cursor(display, mouse_cursor);
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
				// hide tooltips when mouse goes over menubar
				// not needed on Mac, see above
				//toolbar->mouseMove(-1, -1, -1, -1);
			}
#endif
#ifdef ALLEGRO_UNIX
			else if (event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
				// hide tooltips when mouse goes over menubar
				toolbar->mouseMove(-1, -1, -1, -1);
			}
#endif
			else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				maybe_remove_input_getters();
				
				int ret;
				if (levelEditor->getChanged()) {
					ret = al_show_native_message_box(display, "Confirm Exit", "The map has changed!", "Really exit?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
					tgui::clearKeyState();
				}
				else {
					ret = 1;
				}
				if (ret == 1) {
					goto done;
				}
			}
			else if (event.type == ALLEGRO_EVENT_MENU_CLICK) {
				maybe_remove_input_getters();

				if (event.user.data1 == FILE_NEW_ID) {
					new_map();
				}
				if (event.user.data1 == FILE_OPEN_ID) {
					do_open();
				}
				else if (event.user.data1 >= intptr_t(RECENT_ID_START) && event.user.data1 < intptr_t(RECENT_ID_START+recent_files.size())) {
					std::string s = recent_files[event.user.data1-RECENT_ID_START];
					int ret;
					if (levelEditor->getChanged()) {
						ret = al_show_native_message_box(display, "Warning", "The map has changed!", "Really load a new map?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
						tgui::clearKeyState();
					}
					else {
						ret = 1;
					}
					if (ret == 1) {
						if (levelEditor->load(s)) {
							levelEditor->setLastSaveName(s);
							setTitle();
							draw_solids.clear();
#ifdef SUPPORT_GROUPS
							//draw_groups.clear();
#endif
							for (int i = 0; i < levelEditor->getNumLayers(); i++) {
								draw_solids.push_back(true);
#ifdef SUPPORT_GROUPS
								//draw_groups.push_back(true);
#endif
							}

							layerCombo->setNumItems(levelEditor->getNumLayers());
							layerCombo->setSelected(levelEditor->getLayer());
							
							levelEditor->resizeScrollpane();
							levelScrollpane->scrollBy(-1000000, -1000000);
						}
					}
				}
				else if (event.user.data1 == FILE_SAVE_ID) {
					levelEditor->save(false);
					setTitle();
				}
				else if (event.user.data1 == FILE_SAVE_AS_ID) {
					levelEditor->save(true);
					setTitle();
				}
				else if (event.user.data1 == FILE_RELOAD_TILES_ID) {
#if defined ALLEGRO_MACOSX && defined MAS
					deleteTmpTiles(tile_load_path);
#endif
					loadTileSheets(tile_load_path.c_str());

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}

					sheetCombo->setSelected(0);
				}
				else if (event.user.data1 == FILE_LOAD_TILES_ID) {
					const char *start = tile_arg_index < 0 ? al_get_current_directory() : (argc > tile_arg_index ? argv[tile_arg_index] : al_get_current_directory());
					std::string dir = selectDir(start);
					if (dir != "") {
						tile_load_path = dir;
						save_tile_load_path = tile_load_path;
#if defined ALLEGRO_MACOSX && defined MAS
						deleteTmpTiles(tile_load_path);
						num_tile_sheets = -1;
#endif
						loadTileSheets(tile_load_path.c_str());

						if (tileSheets.size()) {
							int pane_size_x, pane_size_y;
							get_ss(&pane_size_x, &pane_size_y);
							tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
						}

						sheetCombo->setSelected(0);
					}
				}
				else if (event.user.data1 == FILE_SET_TILE_SIZE_ID) {
					add_tile_size_getter();
				}
				else if (event.user.data1 == FILE_EXIT_ID) {
					int ret;
					if (levelEditor->getChanged()) {
						ret = al_show_native_message_box(display, "Confirm Exit", "The map has changed!", "Really exit?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
						tgui::clearKeyState();
					}
					else {
						ret = 1;
					}
					if (ret == 1) {
						goto done;
					}
				}
				else if (event.user.data1 == EDIT_COPY_ID) {
					levelEditor->copy_cut(ALLEGRO_KEY_C, false);
				}
				else if (event.user.data1 == EDIT_CUT_ID) {
					levelEditor->copy_cut(ALLEGRO_KEY_X, false);
				}
				else if (event.user.data1 == EDIT_COPY_ALL_ID) {
					levelEditor->copy_cut(ALLEGRO_KEY_C, true);
				}
				else if (event.user.data1 == EDIT_CUT_ALL_ID) {
					levelEditor->copy_cut(ALLEGRO_KEY_X, true);
				}
				else if (event.user.data1 == EDIT_PASTE_ID) {
					levelEditor->paste();
				}
				else if (event.user.data1 == EDIT_INVERT_ID) {
					levelEditor->invertSelection();
				}
				else if (event.user.data1 == EDIT_SELECT_ALL_ID) {
					levelEditor->selectAll();
				}
				else if (event.user.data1 == EDIT_SELECT_NONE_ID) {
					levelEditor->anchor_marquee();
				}
				else if (event.user.data1 == EDIT_DELETE_ID) {
					bool do_undo = true;
					if  (levelEditor->is_marquee_floating()) {
						do_undo = false;
						levelEditor->marquee_float_to_selection();
					}
					levelEditor->delete_marquee(false, do_undo);
				}
				else if (event.user.data1 == EDIT_DELETE_ALL_ID) {
					bool do_undo = true;
					if  (levelEditor->is_marquee_floating()) {
						do_undo = false;
						levelEditor->marquee_float_to_selection();
					}
					levelEditor->delete_marquee(true, do_undo);
				}
				else if (event.user.data1 == EDIT_UNDO_ID) {
					levelEditor->doUndo();
					layerCombo->close_combo();
				}
				else if (event.user.data1 == EDIT_REDO_ID) {
					levelEditor->doRedo();
					layerCombo->close_combo();
				}
				else if (event.user.data1 == SCALE_1_ID) {
					General::scale = 1;
					last_scale = 1;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(1);
				}
				else if (event.user.data1 == SCALE_2_ID) {
					General::scale = 2;
					last_scale = 2;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(2);
				}
				else if (event.user.data1 == SCALE_3_ID) {
					General::scale = 3;
					last_scale = 3;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(3);
				}
				else if (event.user.data1 == SCALE_4_ID) {
					General::scale = 4;
					last_scale = 4;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(4);
				}
				else if (event.user.data1 == SCALE_5_ID) {
					General::scale = 5;
					last_scale = 5;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(5);
				}
				else if (event.user.data1 == SCALE_6_ID) {
					General::scale = 6;
					last_scale = 6;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(6);
				}
				else if (event.user.data1 == SCALE_7_ID) {
					General::scale = 7;
					last_scale = 7;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(7);
				}
				else if (event.user.data1 == SCALE_8_ID) {
					General::scale = 8;
					last_scale = 8;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(8);
				}
				else if (event.user.data1 == SCALE_9_ID) {
					General::scale = 9;
					last_scale = 9;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(9);
				}
				else if (event.user.data1 == SCALE_10_ID) {
					General::scale = 10;
					last_scale = 10;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(10);
				}
#ifdef SUPPORT_GROUPS
				else if (
						event.user.data1 == GROUP_1_ID ||
						event.user.data1 == GROUP_2_ID ||
						event.user.data1 == GROUP_3_ID ||
						event.user.data1 == GROUP_4_ID ||
						event.user.data1 == GROUP_5_ID ||
						event.user.data1 == GROUP_6_ID ||
						event.user.data1 == GROUP_7_ID ||
						event.user.data1 == GROUP_8_ID ||
						event.user.data1 == GROUP_9_ID ||
						event.user.data1 == GROUP_10_ID ||
						event.user.data1 == GROUP_11_ID ||
						event.user.data1 == GROUP_12_ID ||
						event.user.data1 == GROUP_13_ID ||
						event.user.data1 == GROUP_14_ID ||
						event.user.data1 == GROUP_15_ID ||
						event.user.data1 == GROUP_16_ID ||
						event.user.data1 == GROUP_17_ID ||
						event.user.data1 == GROUP_18_ID ||
						event.user.data1 == GROUP_19_ID ||
						event.user.data1 == GROUP_20_ID ||
						event.user.data1 == GROUP_21_ID ||
						event.user.data1 == GROUP_22_ID ||
						event.user.data1 == GROUP_23_ID ||
						event.user.data1 == GROUP_24_ID ||
						event.user.data1 == GROUP_25_ID ||
						event.user.data1 == GROUP_26_ID ||
						event.user.data1 == GROUP_27_ID ||
						event.user.data1 == GROUP_28_ID ||
						event.user.data1 == GROUP_29_ID ||
						event.user.data1 == GROUP_30_ID ||
						event.user.data1 == GROUP_31_ID ||
						event.user.data1 == GROUP_32_ID
					) {
					bool checked_1 = al_get_menu_item_flags(menu, GROUP_1_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_2 = al_get_menu_item_flags(menu, GROUP_2_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_3 = al_get_menu_item_flags(menu, GROUP_3_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_4 = al_get_menu_item_flags(menu, GROUP_4_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_5 = al_get_menu_item_flags(menu, GROUP_5_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_6 = al_get_menu_item_flags(menu, GROUP_6_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_7 = al_get_menu_item_flags(menu, GROUP_7_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_8 = al_get_menu_item_flags(menu, GROUP_8_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_9 = al_get_menu_item_flags(menu, GROUP_9_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_10 = al_get_menu_item_flags(menu, GROUP_10_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_11 = al_get_menu_item_flags(menu, GROUP_11_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_12 = al_get_menu_item_flags(menu, GROUP_12_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_13 = al_get_menu_item_flags(menu, GROUP_13_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_14 = al_get_menu_item_flags(menu, GROUP_14_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_15 = al_get_menu_item_flags(menu, GROUP_15_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_16 = al_get_menu_item_flags(menu, GROUP_16_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_17 = al_get_menu_item_flags(menu, GROUP_17_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_18 = al_get_menu_item_flags(menu, GROUP_18_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_19 = al_get_menu_item_flags(menu, GROUP_19_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_20 = al_get_menu_item_flags(menu, GROUP_20_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_21 = al_get_menu_item_flags(menu, GROUP_21_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_22 = al_get_menu_item_flags(menu, GROUP_22_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_23 = al_get_menu_item_flags(menu, GROUP_23_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_24 = al_get_menu_item_flags(menu, GROUP_24_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_25 = al_get_menu_item_flags(menu, GROUP_25_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_26 = al_get_menu_item_flags(menu, GROUP_26_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_27 = al_get_menu_item_flags(menu, GROUP_27_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_28 = al_get_menu_item_flags(menu, GROUP_28_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_29 = al_get_menu_item_flags(menu, GROUP_29_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_30 = al_get_menu_item_flags(menu, GROUP_30_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_31 = al_get_menu_item_flags(menu, GROUP_31_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					bool checked_32 = al_get_menu_item_flags(menu, GROUP_32_ID) & ALLEGRO_MENU_ITEM_CHECKED;
					int group_type = 0;
					if (checked_1) group_type |= 1;
					if (checked_2) group_type |= (1 << 1);
					if (checked_3) group_type |= (1 << 2);
					if (checked_4) group_type |= (1 << 3);
					if (checked_5) group_type |= (1 << 4);
					if (checked_6) group_type |= (1 << 5);
					if (checked_7) group_type |= (1 << 6);
					if (checked_8) group_type |= (1 << 7);
					if (checked_9) group_type |= (1 << 8);
					if (checked_10) group_type |= (1 << 9);
					if (checked_11) group_type |= (1 << 10);
					if (checked_12) group_type |= (1 << 11);
					if (checked_13) group_type |= (1 << 12);
					if (checked_14) group_type |= (1 << 13);
					if (checked_15) group_type |= (1 << 14);
					if (checked_16) group_type |= (1 << 15);
					if (checked_17) group_type |= (1 << 16);
					if (checked_18) group_type |= (1 << 17);
					if (checked_19) group_type |= (1 << 18);
					if (checked_20) group_type |= (1 << 19);
					if (checked_21) group_type |= (1 << 20);
					if (checked_22) group_type |= (1 << 21);
					if (checked_23) group_type |= (1 << 22);
					if (checked_24) group_type |= (1 << 23);
					if (checked_25) group_type |= (1 << 24);
					if (checked_26) group_type |= (1 << 25);
					if (checked_27) group_type |= (1 << 26);
					if (checked_28) group_type |= (1 << 27);
					if (checked_29) group_type |= (1 << 28);
					if (checked_30) group_type |= (1 << 29);
					if (checked_31) group_type |= (1 << 30);
					if (checked_32) group_type |= (1 << 31);
					levelEditor->set_group_type(group_type);
				}
#endif
				else if (event.user.data1 == LAYER_BEFORE_ID) {
					int layer = levelEditor->getLayer();
					levelEditor->insertLayer(layer);
					layerCombo->close_combo();
				}
				else if (event.user.data1 == LAYER_AFTER_ID) {
					int layer = levelEditor->getLayer();
					int layers = layerCombo->getSize();
					if (layer == layers-1)
						levelEditor->insertLayer(-1);
					else
						levelEditor->insertLayer(layer+1);
					layerCombo->close_combo();
				}
				else if (event.user.data1 == LAYER_DELETE_ID) {
					int layer = levelEditor->getLayer();
					levelEditor->deleteLayer(layer);
					layerCombo->close_combo();
				}
				else if (event.user.data1 == HELP_QUICK_REFERENCE_ID) {
					add_help();
				}
			}
			else if (event.type == ALLEGRO_GET_EVENT_TYPE('T', 'G', 'U', 'I')) {
				ALLEGRO_USER_EVENT *u = (ALLEGRO_USER_EVENT *)&event;
				int type = (int)u->data1;
				if (type == TGUI_EVENT_OBJECT) {
					tgui::TGUIWidget *widget = (tgui::TGUIWidget *)u->data2;
					if (widget && widget == quickRefTitlebar) {
						quickRefFrame->remove();
						delete quickRefTitlebar;
						delete quickRefContent1;
						delete quickRefContent2;
						delete quickRefSplitter;
						delete quickRefBottomSplitter;
						delete quickRefFrame;
						quickRefTitlebar = NULL;
						quickRefContent1 = NULL;
						quickRefContent2 = NULL;
						quickRefSplitter = NULL;
						quickRefBottomSplitter = NULL;
						quickRefFrame = NULL;
						enable_quickref_menu_item();
					}
					else if (widget && (widget == tileSizeGetterTitlebar || widget == tileSizeGetterContent)) {
						maybe_remove_input_getters();
					}
					else if (widget && (widget == levelSizeGetterTitlebar || widget == levelSizeGetterContent)) {
						maybe_remove_input_getters();
					}
				}
			}
			else if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN) {
				tgui::clearKeyState();
			}
			else if (tileSizeGetterDummy != NULL) {
				skip = true;
	
				if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					tgui::TGUIWidget *w = tgui::determineTopLevelOwner(event.mouse.x, event.mouse.y);
					if (w == tileSizeGetterDummy) {
						tgui::handleEvent(&event);
					}
				}

				tileSizeGetterContent->handle_event(&event);
			}
			else if (levelSizeGetterDummy != NULL) {
				skip = true;
	
				if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					tgui::TGUIWidget *w = tgui::determineTopLevelOwner(event.mouse.x, event.mouse.y);
					if (w == levelSizeGetterDummy) {
						tgui::handleEvent(&event);
					}
				}

				levelSizeGetterContent->handle_event(&event);
			}
			else {
				if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)) {
					screenshot();
				}
#ifdef SUPPORT_WALLS
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_T && (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT))) {
					draw_walls = !draw_walls;
				}
#endif
#ifdef SUPPORT_GROUPS_XXX
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_T && (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR))) {
					int layer = levelEditor->getCurrentLayer();
					draw_groups[layer] = !draw_groups[layer];
				}
#endif
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_T && (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL))) {
					int layer = levelEditor->getCurrentLayer();
					draw_solids[get_solids_layer(layer)] = !draw_solids[get_solids_layer(layer)];
				}
#ifndef ALLEGRO_MACOSX
				else if ((event.type == ALLEGRO_EVENT_KEY_CHAR && event.keyboard.keycode == ALLEGRO_KEY_O &&
							(event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL)))
				{
					do_open();
				}

				else if ((event.type == ALLEGRO_EVENT_KEY_CHAR && event.keyboard.keycode == ALLEGRO_KEY_N &&
							(event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL)))
				{
					new_map();
				}
#endif
				else if ((event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode >= ALLEGRO_KEY_F1 && event.keyboard.keycode <= ALLEGRO_KEY_F12)) {
					int layer = (event.keyboard.keycode - ALLEGRO_KEY_F1);
					if (layer < 0 || layer >= levelEditor->getNumLayers())
						continue;

					if (levelEditor->getLayer() != layer) {
						levelEditor->setLayer(layer);
						layerCombo->setSelected(layer);
					} else {
						levelEditor->toggleLayerVisibility(layer);
					}
				}
#ifndef ALLEGRO_MACOSX
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_H && (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL))) {
#else
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_H) {
#endif
					add_help();
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_1 || event.keyboard.keycode == ALLEGRO_KEY_PAD_1)) {
					General::scale = 1;
					last_scale = 1;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(1);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_2 || event.keyboard.keycode == ALLEGRO_KEY_PAD_2)) {
					General::scale = 2;
					last_scale = 2;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(2);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_3 || event.keyboard.keycode == ALLEGRO_KEY_PAD_3)) {
					General::scale = 3;
					last_scale = 3;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(3);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_4 || event.keyboard.keycode == ALLEGRO_KEY_PAD_4)) {
					General::scale = 4;
					last_scale = 4;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(4);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_5 || event.keyboard.keycode == ALLEGRO_KEY_PAD_5)) {
					General::scale = 5;
					last_scale = 5;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(5);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_6 || event.keyboard.keycode == ALLEGRO_KEY_PAD_6)) {
					General::scale = 6;
					last_scale = 6;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(6);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_7 || event.keyboard.keycode == ALLEGRO_KEY_PAD_7)) {
					General::scale = 7;
					last_scale = 7;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(7);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_8 || event.keyboard.keycode == ALLEGRO_KEY_PAD_8)) {
					General::scale = 8;
					last_scale = 8;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(8);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_9 || event.keyboard.keycode == ALLEGRO_KEY_PAD_9)) {
					General::scale = 9;
					last_scale = 9;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(9);
				}
				else if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_0 || event.keyboard.keycode == ALLEGRO_KEY_PAD_0)) {
					General::scale = 10;
					last_scale = 10;
					levelEditor->resizeScrollpane();

					if (tileSheets.size()) {
						int pane_size_x, pane_size_y;
						get_ss(&pane_size_x, &pane_size_y);
						tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
					}
					toggleCheckboxes(10);
				}
				else if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_WARPED) {
					mouse_x = mouse_y = -1;
					levelX = levelY = -1;
					if (pointOnWidget(levelEditor, event.mouse.x, event.mouse.y)) {
						int mx = event.mouse.x;
						int my = event.mouse.y;
						mx += levelScrollpane->getOffsetX();
						my += levelScrollpane->getOffsetY();
						int abs_x, abs_y;
						determineAbsolutePosition(levelScrollpane, &abs_x, &abs_y);
						mouse_x = mx - abs_x;
						mouse_y = my - abs_y;
						int xx = mouse_x - (mouse_x % General::tileSize);
						int yy = mouse_y - (mouse_y % General::tileSize);
						levelX = (xx+General::tileSize/2) / General::tileSize;
						levelY = (yy+General::tileSize/2) / General::tileSize;
					}
				}
			}
			
			if (quickRefFrame)
				quickRefFrame->raise();
			if (levelEditor->getTool() == "Layer Mover") {
				levelEditor->setMoverDestLayer(layerCombo->getSelected());
			}
			int tileX, tileY;
			tileSelector->getSelected(&tileX, &tileY, NULL, NULL);
			
			int tileNumber = 0;
			if (tileSheets.size()) {
				levelEditor->setTile(tileY*(al_get_bitmap_width(tileSheets[0])/General::tileSize)+tileX);
				levelEditor->setSheet(sheetCombo->getSelected());
				tileNumber = tileY*(al_get_bitmap_width(tileSheets[0])/General::tileSize)+tileX;
			}

			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				tgui::TGUIWidget *owner = tgui::determineTopLevelOwner(event.mouse.x, event.mouse.y);
				if (quickRefFrame != NULL) {
					if (owner != quickRefFrame) {
						pushEvent(TGUI_EVENT_OBJECT, quickRefTitlebar);
						skip = true;
					}
				}
			}

			if (skip == false) {
				tgui::handleEvent(&event);
			}

			if (do_add_help) {
				add_help();
			}
		
			bool holding_shift = false;
			if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
				holding_shift = true;
			}
			bool holding_alt = false;
			if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
				holding_alt = true;
			}
			bool holding_control = false;
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				holding_control = true;
			}
				
			// build status string
			std::stringstream ss;
			if (levelEditor->getRecording())
				ss << "(Recording) ";
			if ((levelEditor->getTool() == "Marquee" || levelEditor->getTool() == "Magic Wand") && levelEditor->is_marquee_floating() == false && levelEditor->is_marquee_marked()) {
				if (holding_shift) {
					ss << "(Add) ";
				}
				else if (holding_alt) {
					ss << "(Subtract) ";
				}
			}
			if (levelEditor->getTool() == "Magic Wand" && holding_control) {
				ss << "(Non-contiguous) ";
			}
			if (levelEditor->getTool() == "Clear" && holding_shift) {
				ss << "(All Layers) ";
			}
			if (levelEditor->getTool() == "Fill") {
				if (holding_alt) {
					ss << "(Clear) ";
				}
				if (holding_control) {
					ss << "(Non-contiguous) ";
				}
			}
			ss << "Tool: " << levelEditor->getTool() << "     Size: " << levelEditor->getWidth() << "x" << levelEditor->getHeight() << "     Level: " << (levelX/General::scale) << "," << (levelY/General::scale) << "     Pixel: " << (mouse_x/General::scale) << "," << (mouse_y/General::scale) << "     Tile: " << tileX << "," << tileY << ",#" <<  tileNumber;

			std::vector<A_Leveleditor::Group> groups = levelEditor->getGroups();
			int mx1, mx2, my1, my2;
			levelEditor->get_marquee(&mx1, &my1, &mx2, &my2);
			for (size_t i = 0; i < groups.size(); i++) {
				if (groups[i].x == mx1 && groups[i].y == my1 && groups[i].w == (mx2-mx1) && groups[i].h == (my2-my1)) {
					char buf[100];
					snprintf(buf, 100, "0x%08x", groups[i].type);
					ss << "     Group: " << std::string(buf);
					break;
				}
			}

			statusLabel->setText(ss.str());

#ifdef ALLEGRO_UNIX
			bool maxim = window_maximized;
#else	
			bool maxim = al_get_display_flags(display) & ALLEGRO_MAXIMIZED;
#endif
			if (maxim != window_maximized) {
				window_maximized = maxim;

				int ss = window_maximized ? 5 : 0;

				buffer1->setSplitSize(ss, -1);
				buffer2->setSplitSize(-1, ss);
				buffer3->setSplitSize(ss, -1);
				buffer4->setSplitSize(-1, ss);
				buffer5->setSplitSize(ss, -1);
				buffer6->setSplitSize(-1, ss);
			}
		} while (!al_event_queue_is_empty(queue));

		draw();
	}
done:

	al_get_window_position(display, &window_x, &window_y);
#ifndef ALLEGRO_UNIX
	window_maximized = al_get_display_flags(display) & ALLEGRO_MAXIMIZED;
#endif
	window_w = al_get_display_width(display);
	window_h = al_get_display_height(display);

	al_destroy_bitmap(pencil_icon);
	al_destroy_bitmap(clear_icon);
	al_destroy_bitmap(clone_icon);
	al_destroy_bitmap(fill_icon);
	al_destroy_bitmap(macro_icon);
	al_destroy_bitmap(magic_wand_icon);
	al_destroy_bitmap(marquee_icon);
	al_destroy_bitmap(mover_icon);
	al_destroy_bitmap(record_icon);
	al_destroy_bitmap(screenshot_icon);
	al_destroy_bitmap(solids_icon);
	al_destroy_bitmap(toggle_layer_icon);
	al_destroy_bitmap(toggle_solids_icon);

#if defined ALLEGRO_MACOSX && defined MAS
	if (num_tile_sheets == 0) {
		num_tile_sheets = -1;
	}
#endif

#ifdef ALLEGRO_WINDOWS
	err = fopen_s(&cfg, fn.c_str(), "w");
	if (err == 0) {
#else
	cfg = fopen(fn.c_str(), "w");
	if (cfg) {
#endif
		fprintf(cfg, "window_x=%d\n", window_x);
		fprintf(cfg, "window_y=%d\n", window_y);
		fprintf(cfg, "window_w=%d\n", window_w);
		fprintf(cfg, "window_h=%d\n", window_h);
		fprintf(cfg, "window_maximized=%d\n", window_maximized ? 1 : 0);
		fprintf(cfg, "tile_size=%d\n", save_tile_size);
		fprintf(cfg, "last_scale=%d\n", last_scale);
#if defined ALLEGRO_MACOSX && defined MAS
		if (tile_load_path != ".") {
			fprintf(cfg, "tile_path=%s\n", getUserResource("tmptiles"));
			fprintf(cfg, "num_tile_sheets_mac=%d\n", tileSheets.size());
			fprintf(cfg, "png_sheets_mac=%d\n", png_sheets ? 1 : 0);
		}
		else
#endif
		{
			fprintf(cfg, "tile_path=%s\n", save_tile_load_path.c_str());
		}
		for (size_t i = 0; i < recent_files.size(); i++) {
			fprintf(cfg, "recent%d=%s\n", i, recent_files[i].c_str());
		}
		fclose(cfg);
	}

	}
	catch (std::bad_alloc) {
		al_show_native_message_box(display, "Out of Memory", "Memory has been exhausted.", "Aborting...", 0, 0);
	}

	return 0;
}

