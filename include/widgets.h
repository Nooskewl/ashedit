#ifndef WIDGETS_H
#define WIDGETS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#include "tgui2.hpp"

#include <string>
#include <cmath>
#include <cstdio>
#include <stack>
#include <deque>

#include "general.h"

#ifdef ALLEGRO_MACOSX
#undef ALLEGRO_KEY_LCTRL
#undef ALLEGRO_KEY_RCTRL
#define ALLEGRO_KEY_LCTRL ALLEGRO_KEY_COMMAND
#define ALLEGRO_KEY_RCTRL ALLEGRO_KEY_COMMAND
#endif

#define MAX_LEVEL_SIZE 256
#define MAX_LAYERS 16

void loadTileSheets(const char *path);
extern std::string tile_load_path;
extern int my_tile_size;
extern int save_tile_size;

void new_map_real(int new_level_w, int new_level_h);
void get_ss(int *x, int *y);

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *queue;
extern std::vector<ALLEGRO_BITMAP *> tileSheets;
extern std::vector<bool> draw_solids;
#ifdef SUPPORT_GROUPS
extern std::vector<bool> draw_groups;
#endif
extern ALLEGRO_MOUSE_CURSOR *mouse_cursor;
void screenshot();
extern ALLEGRO_FONT *help_font;
extern bool reverse_scroll;
extern uint64_t new_mouse_pos;
extern bool one_solids_layer;
extern int my_tile_size;
extern std::vector< std::vector< std::vector<bool > > > blank; // is tile in sheet blank?
#ifdef DUMP
extern bool dump;
extern LEVEL_TYPE dump_type;
#endif
extern std::vector<std::string> recent_files;
extern LEVEL_TYPE command_line_level_type;
extern bool use_filename_based_level_types;

extern ALLEGRO_BITMAP *pencil_icon;
extern ALLEGRO_BITMAP *clear_icon;
extern ALLEGRO_BITMAP *clone_icon;
extern ALLEGRO_BITMAP *fill_icon;
extern ALLEGRO_BITMAP *macro_icon;
extern ALLEGRO_BITMAP *magic_wand_icon;
extern ALLEGRO_BITMAP *marquee_icon;
extern ALLEGRO_BITMAP *mover_icon;
extern ALLEGRO_BITMAP *record_icon;
extern ALLEGRO_BITMAP *screenshot_icon;
extern ALLEGRO_BITMAP *solids_icon;
extern ALLEGRO_BITMAP *toggle_layer_icon;
extern ALLEGRO_BITMAP *toggle_solids_icon;

void setTitle();
void add_to_recent(std::string fn, bool build = false, bool at_back = false);

void enable_undo_menu_item();
void disable_undo_menu_item();
void enable_redo_menu_item();
void disable_redo_menu_item();
void enable_copy_menu_item();
void disable_copy_menu_item();
void enable_copy_all_menu_item();
void disable_copy_all_menu_item();
void enable_cut_menu_item();
void disable_cut_menu_item();
void enable_cut_all_menu_item();
void disable_cut_all_menu_item();
void enable_delete_menu_item();
void disable_delete_menu_item();
void enable_delete_all_menu_item();
void disable_delete_all_menu_item();
void enable_paste_menu_item();
void disable_paste_menu_item();
void enable_select_all_menu_item();
void disable_select_all_menu_item();
void enable_select_none_menu_item();
void disable_select_none_menu_item();
void enable_layer_before_menu_item();
void disable_layer_before_menu_item();
void enable_layer_after_menu_item();
void disable_layer_after_menu_item();
void enable_layer_delete_menu_item();
void disable_layer_delete_menu_item();
void enable_quickref_menu_item();
void disable_quickref_menu_item();

struct ASH_Point {
	float x, y;
};

class A_Splitter_Resizable : public tgui::TGUIWidget {
public:
	virtual void paneResize(tgui::TGUIWidget *pane, float *xx, float *yy, float *w, float *h) = 0;
};

class A_Splitter_Panel : public tgui::TGUIWidget {
public:
	bool getAbsoluteChildPosition(tgui::TGUIWidget *widget, int *x, int *y)
	{
		if (child == widget) {
			int xx, yy;
			tgui::determineAbsolutePosition(this, &xx, &yy);
			*x = xx;
			*y = yy;
			return true;
		}

		if (child->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}

		return false;
	}

	virtual void translate(int xx, int yy) {
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (!child) return;
		child->mouseDown(rel_x, rel_y, abs_x, abs_y, mb);
	}

	virtual void mouseScroll(int z, int w, int dz, int dw) {
		if (!child) return;
		child->mouseScroll(z, w, dz, dw);
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (!child) return;
		child->mouseMove(rel_x, rel_y, abs_x, abs_y);
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (!child) return;
		child->mouseUp(rel_x, rel_y, abs_x, abs_y, mb);
	}

	virtual void draw(int abs_x, int abs_y) {
		int prev_x, prev_y, prev_w, prev_h;
		tgui::getClip(&prev_x, &prev_y, &prev_w, &prev_h);
		tgui::setClip(abs_x, abs_y, width, height);
		if (child) {
			child->draw(abs_x, abs_y);
		}
		tgui::setClip(prev_x, prev_y, prev_w, prev_h);
	}

	virtual void postDraw(int abs_x, int abs_y) {
		int prev_x, prev_y, prev_w, prev_h;
		tgui::getClip(&prev_x, &prev_y, &prev_w, &prev_h);
		tgui::setClip(abs_x, abs_y, width, height);
		if (child) {
			child->postDraw(abs_x, abs_y);
		}
		tgui::setClip(prev_x, prev_y, prev_w, prev_h);
	}

	virtual void resize(void) {
		splitter->paneResize(this, &x, &y, &width, &height);
		resize_child();
	}

	A_Splitter_Panel(A_Splitter_Resizable *splitter) :
		splitter(splitter)
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	A_Splitter_Resizable *splitter;
};



class A_Splitter : public A_Splitter_Resizable {
public:
	static const int MOUSE_MOVE = 0;
	static const int MOUSE_UP = 1;
	static const int MOUSE_DOWN = 2;
	static const int MOUSE_SCROLL = 3;

	static const int SPLIT_HORIZONTAL = 0;
	static const int SPLIT_VERTICAL = 1;

	int get_div_size()
	{
		if (resizable) {
			return 5;
		}
		else {
			return 0;
		}
	}

	bool getAbsoluteChildPosition(tgui::TGUIWidget *widget, int *x, int *y)
	{
		if (first_pane == widget || second_pane == widget) {
			int own_x, own_y;
			tgui::determineAbsolutePosition(this, &own_x, &own_y);

			int xx, yy;

			if (first_pane == widget) {
				*x = own_x + first_pane->getX();
				*y = own_y + first_pane->getY();
			}
			else {
				if (split_type == SPLIT_HORIZONTAL) {
					xx = 0;
					yy = first_pixels+get_div_size();
				}
				else {
					xx = first_pixels+get_div_size();
					yy = 0;
				}
				*x = own_x + xx + second_pane->getX();
				*y = own_y + yy + second_pane->getY();
			}

			return true;
		}

		if (first_pane->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}
		if (second_pane->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}

		return false;
	}

	virtual void chainKeyDown(int chainKeycode) {
		if (first_pane) {
			first_pane->chainKeyDown(chainKeycode);
		}
		if (second_pane) {
			second_pane->chainKeyDown(chainKeycode);
		}
	}

	virtual void chainKeyUp(int chainKeycode) {
		if (first_pane) {
			first_pane->chainKeyUp(chainKeycode);
		}
		if (second_pane) {
			second_pane->chainKeyUp(chainKeycode);
		}
	}

	virtual void remove(void) {
		TGUIWidget::remove();

		if (first_pane) {
			first_pane->remove();
		}
		if (second_pane) {
			second_pane->remove();
		}
	}

	virtual void translate(int xx, int yy) {
		resize();
	}

	virtual void raise(void) {
		TGUIWidget::raise();

		if (first_pane) {
			first_pane->raise();
		}
		if (second_pane) {
			second_pane->raise();
		}
	}

	virtual void lower(void) {
		TGUIWidget::lower();

		if (first_pane && first_pane->getChild()) {
			first_pane->getChild()->lower();
		}
		if (second_pane && second_pane->getChild()) {
			second_pane->getChild()->lower();
		}
	}

	virtual void draw(int abs_x, int abs_y) {
		int xx, yy;
		if (first_pane) {
			determineAbsolutePosition(first_pane, &xx, &yy);
			first_pane->draw(xx, yy);
		}
		if (second_pane) {
			determineAbsolutePosition(second_pane, &xx, &yy);
			second_pane->draw(xx, yy);
		}

		if (!resizable) return;

		// Draw the divider

		int cx, cy, cw, ch;

		tgui::getClip(&cx, &cy, &cw, &ch);
		tgui::clearClip();

		ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);

		for (int i = 0; i < get_div_size(); i++) {
			if (split_type == SPLIT_HORIZONTAL) {
				float yy = abs_y+first_pixels+0.5+i;
				al_draw_line(abs_x, yy, abs_x+width, yy,
					color, 1);
			}
			else {
				float xx = abs_x+first_pixels+0.5+i;
				al_draw_line(xx, abs_y, xx, abs_y+height,
					color, 1);
			}
		}
		
		tgui::setClip(cx, cy, cw, ch);
	}

	virtual void chainDraw()
	{
		int abs_x, abs_y;
		determineAbsolutePosition(this, &abs_x, &abs_y);
		draw(abs_x, abs_y);
	}

	virtual void postDraw(int abs_x, int abs_y)
	{
		if (first_pane) {
			first_pane->postDraw(abs_x, abs_y);
		}
		if (second_pane) {
			if (split_type == SPLIT_HORIZONTAL) {
				second_pane->postDraw(abs_x, abs_y+first_pixels+get_div_size());
			}
			else {
				second_pane->postDraw(abs_x+first_pixels+get_div_size(), abs_y);
			}
		}
	}


	void passMouseOn(int type, int rel_x, int rel_y, int abs_x, int abs_y, int mb, int z = 0, int w = 0, int dz = 0, int dw = 0) {
		int rx1, rx2, rx3;
		int ry1, ry2, ry3;
		TGUIWidget *target;

		if (down_on_slider) {
			target = this;
		}
		else {
			if (split_type == SPLIT_HORIZONTAL) {
				if (rel_y < first_pixels) {
					target = first_pane;
				}
				else if (rel_y >= first_pixels+get_div_size()) {
					target = second_pane;
				}
				else {
					target = this;
				}
			}
			else {
				if (rel_x < first_pixels) {
					target = first_pane;
				}
				else if (rel_x >= first_pixels+get_div_size()) {
					target = second_pane;
				}
				else {
					target = this;
				}
			}
		}

		if (target == this) {
			rx1 = rel_x;
			ry1 = rel_y;
			rx2 = ry2 = rx3 = ry3 = -1;
		}
		else if (target == first_pane) {
			rx2 = rel_x;
			ry2 = rel_y;
			rx1 = ry1 = rx3 = ry3 = -1;
			if (type == MOUSE_SCROLL) {
				first_pane->mouseScroll(z, w, dz, dw);
			}
		}
		else {
			if (split_type == SPLIT_HORIZONTAL) {
				rx3 = rel_x;
				ry3 = rel_y - first_pixels - get_div_size();
			}
			else {
				rx3 = rel_x - first_pixels - get_div_size();
				ry3 = rel_y;
			}
			rx1 = ry1 = rx2 = ry2 = -1;
			if (type == MOUSE_SCROLL) {
				second_pane->mouseScroll(z, w, dz, dw);
			}
		}

		switch (type) {
			case MOUSE_MOVE:
				_mouseMove(rx1, ry1, abs_x, abs_y);
				break;
			case MOUSE_DOWN:
				_mouseDown(rx1, ry1, abs_x, abs_y, mb);
				break;
			case MOUSE_UP:
				_mouseUp(rx1, ry1, abs_x, abs_y, mb);
				break;
		}

		if (type == MOUSE_MOVE) {
			first_pane->mouseMove(rx2, ry2, abs_x, abs_y);
		}
		else if (type == MOUSE_DOWN) {
			first_pane->mouseDown(rx2, ry2, abs_x, abs_y, mb);
		}
		else if (type == MOUSE_UP) {
			first_pane->mouseUp(rx2, ry2, abs_x, abs_y, mb);
		}

		if (type == MOUSE_MOVE) {
			second_pane->mouseMove(rx3, ry3, abs_x, abs_y);
		}
		else if (type == MOUSE_DOWN) {
			second_pane->mouseDown(rx3, ry3, abs_x, abs_y, mb);
		}
		else if (type == MOUSE_UP) {
			second_pane->mouseUp(rx3, ry3, abs_x, abs_y, mb);
		}
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y)
	{
		passMouseOn(MOUSE_MOVE, rel_x, rel_y, abs_x, abs_y, 0);
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb)
	{
		passMouseOn(MOUSE_DOWN, rel_x, rel_y, abs_x, abs_y, mb);
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb)
	{
		passMouseOn(MOUSE_UP, rel_x, rel_y, abs_x, abs_y, mb);
	}

	virtual void mouseScroll(int z, int w, int dz, int dw) {
		if (parent == NULL) {
			return;
		}

		int px, py;
		determineAbsolutePosition(parent, &px, &py);
		ALLEGRO_MOUSE_STATE s;
		al_get_mouse_state(&s);
		int rel_x = s.x - px;
		int rel_y = s.y - py;
		int abs_x = s.x;
		int abs_y = s.y;

		passMouseOn(MOUSE_SCROLL, rel_x, rel_y, abs_x, abs_y, 0, z, w, dz, dw);
	}

	void _mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (down_on_slider) {
			int slider_delta;
			int size;
			if (split_type == SPLIT_HORIZONTAL) {
				slider_delta = abs_y - last_slider_pos;
				size = height;
			}
			else {
				slider_delta = abs_x - last_slider_pos;
				size = width;
			}
			if (first_pixels + slider_delta >= 32 && second_pixels - slider_delta >= 32) {
				first_pixels += slider_delta;
				second_pixels -= slider_delta;
				last_slider_pos += slider_delta;
				ratio = (float)first_pixels / (size-get_div_size());
				resize();
			}
		}
		else {
			int my_x, my_y;
			bool on;
			tgui::determineAbsolutePosition(this, &my_x, &my_y);
			if (split_type == SPLIT_HORIZONTAL) {
				if (abs_x >= my_x && abs_x < my_x+width)
					on = (abs_y >= my_y+first_pixels && abs_y < my_y+first_pixels+get_div_size());
				else
					on = false;
			}
			else {
				if (abs_y >= my_y && abs_y < my_y+height)
					on = (abs_x >= my_x+first_pixels && abs_x < my_x+first_pixels+get_div_size());
				else
					on = false;
			}
			if (resizable && on) {
				ALLEGRO_SYSTEM_MOUSE_CURSOR type = (split_type == SPLIT_HORIZONTAL) ? ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N : ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W;
				if (cursor_type != type) {
					al_set_system_mouse_cursor(al_get_current_display(), type);
					cursor_type = type;
				}
			}
			else {
				if (cursor_type != ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT) {
					al_set_mouse_cursor(display, mouse_cursor);
					cursor_type = ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT;
				}
			}
		}
	}

	void _mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		int my_x, my_y;
		bool on;
		tgui::determineAbsolutePosition(this, &my_x, &my_y);
		if (split_type == SPLIT_HORIZONTAL) {
			if (abs_x >= my_x && abs_x < my_x+width)
				on = (abs_y >= my_y+first_pixels && abs_y < my_y+first_pixels+get_div_size());
			else
				on = false;
		}
		else {
			if (abs_y >= my_y && abs_y < my_y+height) {
				on = (abs_x >= my_x+first_pixels && abs_x < my_x+first_pixels+get_div_size());
			}
			else
				on = false;
		}
		if (resizable && on) {
			down_on_slider = true;
			last_slider_pos = (split_type == SPLIT_HORIZONTAL) ? abs_y : abs_x;
		}
	}

	void _mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (down_on_slider) {
			down_on_slider = false;
		}
	}

	void setSplitRatio(float p1, float p2) {
		if (split_type == SPLIT_HORIZONTAL) {
			first_pixels = (height-get_div_size()) * p1;
			second_pixels = (height-get_div_size()) * p2;
		}
		else {
			first_pixels = (width-get_div_size()) * p1;
			second_pixels = (width-get_div_size()) * p2;
		}
		ratio = p1;
	};

	void setSplitSize(int a, int b) {
		if (a != -1 && b != -1) {
			first_pixels = a;
			second_pixels = b;
			first_fixed = second_fixed = true;
		}
		else if (split_type == SPLIT_HORIZONTAL) {
			if (a == -1 && b == -1) {
				first_pixels = (height-get_div_size())/2;
				second_pixels = (height-get_div_size()) - first_pixels;
			}
			else if (a == -1) {
				second_pixels = b;
				first_pixels = (height-get_div_size()) - second_pixels;
				second_fixed = true;
			}
			else if (b == -1) {
				first_pixels = a;
				second_pixels = (height-get_div_size()) - first_pixels;
				first_fixed = true;
			}
			ratio = first_pixels / (height-get_div_size());
		}
		else {
			if (a == -1 && b == -1) {
				first_pixels = (width-get_div_size())/2;
				second_pixels = (width-get_div_size()) - first_pixels;
			}
			else if (a == -1) {
				second_pixels = b;
				first_pixels = (width-get_div_size()) - second_pixels;
				second_fixed = true;
			}
			else if (b == -1) {
				first_pixels = a;
				second_pixels = (width-get_div_size()) - first_pixels;
				first_fixed = true;
			}
			ratio = first_pixels / (width-get_div_size());
		}
	}

	void setResizable(bool r) {
		resizable = r;
	}

	// you should not add widgets to things retrieved from these 2 functions
	A_Splitter_Panel *getFirstPane(void) { return first_pane; }
	A_Splitter_Panel *getSecondPane(void) { return second_pane; }

	void addToFirstPane(TGUIWidget *widget) {
		TGUIWidget *oldParent = tgui::getNewWidgetParent();
		tgui::setNewWidgetParent(first_pane);
		tgui::addWidget(widget);
		tgui::setNewWidgetParent(oldParent);
	}

	void addToSecondPane(TGUIWidget *widget) {
		second_pane->setChild(widget);
		widget->setParent(second_pane);
		TGUIWidget *oldParent = tgui::getNewWidgetParent();
		tgui::setNewWidgetParent(second_pane);
		tgui::addWidget(widget);
		tgui::setNewWidgetParent(oldParent);
	}

	virtual void resize(void) {
		resize_self();

		if (first_fixed) {
			setSplitSize(first_pixels, -1);
		}
		else if (second_fixed) {
			setSplitSize(-1, second_pixels);
		}
		else {
			setSplitRatio(ratio, 1.0-ratio);
		}

		if (first_pane)
			first_pane->resize();
		if (second_pane)
			second_pane->resize();
	}

	A_Splitter(int split_type) :
		split_type(split_type),
		first_pixels(0),
		second_pixels(0),
		resizable(false),
		down_on_slider(false),
		cursor_type(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT),
		first_fixed(false),
		second_fixed(false),
		ratio(0.5)
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;

		first_pane = new A_Splitter_Panel(this);
		second_pane = new A_Splitter_Panel(this);
		TGUIWidget *oldParent = tgui::getNewWidgetParent();
		tgui::setNewWidgetParent(this);
		tgui::addWidget(first_pane);
		tgui::addWidget(second_pane);
		tgui::setNewWidgetParent(oldParent);
	}

protected:

	void paneResize(TGUIWidget *pane, float *xx, float *yy, float *w, float *h)
	{
		int abs_x, abs_y;
		tgui::determineAbsolutePosition(this, &abs_x, &abs_y);

		if (pane == first_pane) {
			*xx = 0;
			*yy = 0;

			if (split_type == SPLIT_HORIZONTAL) {
				*w = width;
				*h = first_pixels;
			}
			else {
				*w = first_pixels;
				*h = height;
			}
		}
		else {
			*xx = 0;
			*yy = 0;

			if (split_type == SPLIT_HORIZONTAL) {
				*w = width;
				*h = second_pixels;
			}
			else {
				*w = second_pixels;
				*h = height;
			}
		}
	}

	A_Splitter_Panel *first_pane, *second_pane;
	int split_type;
	float first_pixels;
	float second_pixels;
	bool resizable;
	bool down_on_slider;
	int last_slider_pos;
	int cursor_type;
	bool first_fixed;
	bool second_fixed;
	float ratio;
};

class A_Image : public tgui::TGUIWidget {
public:
	virtual void draw(int abs_x, int abs_y) {
		al_draw_scaled_bitmap(
			image,
			0,
			0,
			al_get_bitmap_width(image),
			al_get_bitmap_height(image),
			abs_x,
			abs_y,
			width, height,
			0
		);
	}

	A_Image(ALLEGRO_BITMAP *bmp)
	{
		this->image = bmp;
		x = 0;
		y = 0;
		width = al_get_bitmap_width(bmp);
		height = al_get_bitmap_height(bmp);
	}
protected:
	ALLEGRO_BITMAP *image;
};

class A_Label : public tgui::TGUIWidget {
public:
	void setFont(ALLEGRO_FONT *font) {
		this->font = font;
	}

	ALLEGRO_FONT *getFont() {
		return font;
	}

	void setStatus(bool status) {
		this->status = status;
	}

	void real_draw(float abs_x, float abs_y)
	{
		float yy = abs_y + (status ? 5 : 0);

		if (status) {
			int cx, cy, cw, ch;
			tgui::getClip(&cx, &cy, &cw, &ch);
			tgui::clearClip();

			al_draw_line(abs_x-1, abs_y+1, abs_x+width+1, abs_y, al_map_rgb(0, 0, 0), 1);
		
			tgui::setClip(cx, cy, cw, ch);
		}

		for (unsigned int i = 0; i < lines.size(); i++) {
			if (draw_shadow) {
				al_draw_text(
					font,
					al_map_rgb(0, 0, 0),
					abs_x+1+indent, yy+1,
					0,
					lines[i].c_str()
				);
			}
			al_draw_text(
				font,
				color,
				abs_x + (status ? 5 : 0) + indent, yy,
				0,
				lines[i].c_str()
			);
			yy += al_get_font_line_height(font);
		}
	}

	virtual void draw(int abs_x, int abs_y) {
		al_hold_bitmap_drawing(true);
		real_draw(abs_x, abs_y);
		al_hold_bitmap_drawing(false);
	}

	void setLines(std::string text) {
		lines.clear();
		std::string::size_type currPos = 0;
		std::string::size_type newlinePos;
		while ((newlinePos = text.find("\n", currPos)) != std::string::npos) {
			std::string s = text.substr(currPos, newlinePos-currPos);
			currPos = newlinePos+1;
			lines.push_back(s);
		}
		if (currPos < text.length()) {
			// no newline at end
			std::string s = text.substr(currPos, text.length()-currPos);
			lines.push_back(s);
		}
	}

	void setText(std::string text) {
		setLines(text);
	}

	void setDrawShadow(bool draw_shadow) {
		this->draw_shadow = draw_shadow;
	}

	std::vector<std::string> getLines() {
		return lines;
	}

	std::string getLine(int i) {
		if (i >= 0 && i < (int)lines.size()) {
			return lines[i];
		}
		return "";
	}

	void setIndent(int indent) {
		this->indent = indent;
	}

	A_Label(std::string text, ALLEGRO_COLOR color) :
		color(color)
	{
		setLines(text);
		x = y = 0;
		draw_shadow = false;
		status = false;
		font = tgui::getFont();
		indent = 0;
	}

protected:
	std::vector<std::string> lines;
	ALLEGRO_COLOR color;
	bool draw_shadow;
	bool status;
	ALLEGRO_FONT *font;
	int indent;
};

class A_Frame : public tgui::TGUIWidget {
public:
	static const int PADDING = 5;

	bool getAbsoluteChildPosition(tgui::TGUIWidget *widget, int *x, int *y)
	{
		if (child == widget) {
			int xx, yy;
			tgui::determineAbsolutePosition(this, &xx, &yy);
			*x = xx;
			*y = yy;
			return true;
		}

		if (child->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}

		return false;
	}

	virtual void translate(int xx, int yy) {
		x += xx;
		y += yy;

		if (child)
			child->translate(xx, yy);
	}

	virtual void draw(int abs_x, int abs_y) {
		al_draw_filled_rectangle(
			abs_x+PADDING,
			abs_y+PADDING,
			abs_x+width+PADDING,
			abs_y+height+PADDING,
			al_map_rgba(0, 0, 0, 100)
		);

		al_draw_filled_rectangle(
			abs_x,
			abs_y,
			abs_x+width,
			abs_y+height,
			//color
			al_map_rgb(255, 255, 255)
		);

		ALLEGRO_FONT *font;
		if (contentLabel) {
			font = contentLabel->getFont();
		}
		else {
			font = help_font;
		}

		int line_h = al_get_font_line_height(font);
		ALLEGRO_COLOR odd = al_map_rgb(238, 238, 238);
		ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

		int i = 0;
		int j = 0;
		for (int y = abs_y+28/*title bar height*/; y-abs_y < height-line_h; y += line_h, i++) {
			if (i > 0 && i < (int)contentLabel->getLines().size()-1 && (j == 0 || contentLabel->getLine(i+1) == "")) {
				if (j == 0) {
					al_draw_line(abs_x+24, y-1, abs_x+width-25, y-1, black, 1.0f);
				}
				if (contentLabel->getLine(i+1) == "") {
					al_draw_line(abs_x+24, y+line_h+1, abs_x+width-25, y+line_h+1, odd, 1.0f);
					al_draw_line(abs_x+24, y+line_h+2, abs_x+width-25, y+line_h+2, black, 1.0f);
				}

			}
			if (contentLabel && contentLabel->getLine(i) == "") {
				j = 0;
				continue;
			}
			if (j % 2 == 1) {
				al_draw_filled_rectangle(abs_x+25, y, abs_x+width-25, y+line_h, odd);
			}
			al_draw_line(abs_x+25, y-1, abs_x+25, y+line_h+1, black, 1.0f);
			al_draw_line(abs_x+width-25, y-1, abs_x+width-25, y+line_h+1, black, 1.0f);
			j++;
		}

		al_draw_line(
			abs_x,
			abs_y+0.5,
			abs_x+width,
			abs_y+0.5,
			al_map_rgba(255, 255, 255, 150),
			1
		);

		al_draw_line(
			abs_x+0.5,
			abs_y,
			abs_x+0.5,
			abs_y+height-1,
			al_map_rgba(255, 255, 255, 150),
			1
		);

		al_draw_line(
			abs_x+width-0.5,
			abs_y+1,
			abs_x+width-0.5,
			abs_y+height,
			al_map_rgba(0, 0, 0, 150),
			1
		);

		al_draw_line(
			abs_x,
			abs_y+height-0.5,
			abs_x+width,
			abs_y+height-0.5,
			al_map_rgba(0, 0, 0, 150),
			1
		);

	}

	void setPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	virtual void resize(void) {
		resize_child();
	}

	void setSize(int w, int h) {
		width = w;
		height = h;
	}

	A_Frame(ALLEGRO_COLOR color)
	{
		this->color = color;
	}

	A_Frame(ALLEGRO_COLOR color, int w, int h, A_Label *contentLabel)
	{
		this->color = color;
		width = w;
		height = h;
		this->contentLabel = contentLabel;
	}

protected:
	ALLEGRO_COLOR color;
	A_Label *contentLabel;
};

class A_Titlebar : public tgui::TGUIWidget {
public:
	static const int CLOSE_BUTTON = 1;

	virtual void draw(int abs_x, int abs_y) {
		al_draw_filled_rectangle(
			abs_x+1,
			abs_y+1,
			abs_x+width-1,
			abs_y+height-1,
			color
		);
		al_draw_text(
			tgui::getFont(),
			al_map_rgb(255, 216, 0),
			abs_x+3,
			abs_y+3,
			0,
			("  " + caption).c_str()
		);

		if (flags & CLOSE_BUTTON) {
			int yy = 28/2 - CLOSE_BUTTON_SIZE/2;
			al_draw_line(
				abs_x+width-(CLOSE_BUTTON_SIZE+yy)-1,
				abs_y+yy,
				abs_x+width-yy-1,
				abs_y+CLOSE_BUTTON_SIZE+yy,
				al_map_rgb(0xff, 0, 0x27),
				4.0f
			);
			al_draw_line(
				abs_x+width-(CLOSE_BUTTON_SIZE+yy)-1,
				abs_y+CLOSE_BUTTON_SIZE+yy,
				abs_x+width-yy-1,
				abs_y+yy,
				al_map_rgb(0xff, 0, 0x27),
				4.0f
			);
		}
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		int a, b;
		tgui::determineAbsolutePosition(this, &a, &b);
		if (mb == 1 && rel_x >= 0) {
			int yy = 28/2 - CLOSE_BUTTON_SIZE/2;
			if (
				(flags & CLOSE_BUTTON) && 
				(rel_x >= width-(CLOSE_BUTTON_SIZE+yy)-1) &&
				(rel_x < width-yy-1) &&
				(rel_y >= yy) &&
				(rel_y < CLOSE_BUTTON_SIZE+yy)
			) {
				pushEvent(TGUI_EVENT_OBJECT, (void *)this);
				return;
			}
			down = true;
			downX = abs_x;
			downY = abs_y;
		}
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (mb == 1) {
			down = false;
		}
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		int dw = al_get_display_width(display);
		int dh = al_get_display_height(display);

		if (down && abs_x >= 0 && abs_y >= 0 && abs_x < dw && abs_y < dh) {
			int dx = abs_x - downX;
			int dy = abs_y - downY;
			downX = abs_x;
			downY = abs_y;

			int new_x = parentFrame->getX() + dx;
			int new_y = parentFrame->getY() + dy;

			int pf_w = parentFrame->getWidth();

			if (new_x+pf_w >= 32 && new_x < dw-32 && new_y >= 0 && new_y < dh-28) {
				parentFrame->translate(dx, dy);
			}
		}
		else {
			down = false;
		}
	}
	
	virtual void keyDown(int keycode) {
		if (keycode == ALLEGRO_KEY_ESCAPE) {
			pushEvent(TGUI_EVENT_OBJECT, (void *)this);
			return;
		}
	}

	A_Titlebar(TGUIWidget *parentFrame, std::string caption, ALLEGRO_COLOR color, int flags) :
		parentFrame(parentFrame),
		caption(caption),
		color(color),
		flags(flags),
		down(false)
	{
		x = y = 0;
	}

protected:
	static const int CLOSE_BUTTON_SIZE = 18;

	TGUIWidget *parentFrame;
	std::string caption;
	ALLEGRO_COLOR color;
	int flags;
	bool down;
	int downX, downY;
};

class A_Button : public A_Frame {
public:
	static const int PADDING = 5;

	virtual void draw(int abs_x, int abs_y) {
		A_Frame::draw(abs_x, abs_y);

		al_draw_text(
			tgui::getFont(),
			al_map_rgb(255, 255, 255),
			abs_x+PADDING,
			abs_y+PADDING,
			0,
			text.c_str()
		);

	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (rel_x >= 0)
			exit(0);
	}

	A_Button(std::string text, ALLEGRO_COLOR color) :
		A_Frame(color)
	{
		this->text = text;
		this->color = color;
		width = al_get_text_width(
			tgui::getFont(),
			text.c_str()
		) + PADDING*2;
		height = al_get_font_line_height(
			tgui::getFont()
		) + PADDING*2;
	}

protected:
	std::string text;
};

class A_Combobox : public tgui::TGUIWidget {
public:
	static const int HEIGHT = 28;

	int getSelected(void) {
		return selected;
	}

	void setSelected(int s) {
		selected = s;
	}

	int getShow(void)
	{
		int show;
		if ((int)values.size() < maxShown) {
			show = values.size();
		}
		else {
			show = maxShown;
		}
		return show;
	}

	int getIndex(int abs_x, int abs_y) {
		int widget_x, widget_y;
		tgui::determineAbsolutePosition(this, &widget_x, &widget_y);
		int rel_x = abs_x - widget_x;
		int rel_y = abs_y - widget_y;

		int show = getShow();

		if (rel_x < 0 || rel_y < 0 || rel_x > width || rel_y > (show+2)*HEIGHT)
			return -4;

		int index_tmp = rel_y / HEIGHT;

		if (index_tmp == 0)
			return -1;
		else if (index_tmp == show+1)
			return -2;
		else
			return index_tmp-1;
	}

	int getSize(void) {
		return (int)values.size();
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (mb != 1) return;

		if (al_get_time() < ignoreMouseDown) {
			return;
		}

		ignoreMouseDown = al_get_time() + 0.05;

		A_Splitter_Panel *panel = (A_Splitter_Panel *)parent;
		A_Splitter *splitter = (A_Splitter *)(panel->splitter);

		int show = getShow();

		if (rel_x >= 0 && !opened) {
			tgui::getTopLevelParent(this)->raise();
			height = (getShow()+2)*HEIGHT;
			splitter->setSplitSize(height, -1);
			tgui::resize(NULL);

			opened = true;

			if (selected+(show/2) >= (int)values.size()) {
				top = values.size() - show;
			}
			else {
				top = MAX(0, selected - (show/2));
			}
			hover = -1;
			return;
		}

		if (!opened)
			return;

		// was opened

		int index = getIndex(abs_x, abs_y);

		if (index == -1) {
			if (top > 0) {
				top--;
			}
		}
		else if (index == -2) {
			if (top < (int)values.size()-show) {
				top++;
			}
		}
		else if (index >= 0) {
			selected = top+index;
			if (callback) {
				callback(selected);
			}
			opened = false;
			hover = -4;
			height = HEIGHT;
			splitter->setSplitSize(height, -1);
			tgui::resize(NULL);
		}
		else {
			opened = false;
			hover = -4;
			height = HEIGHT;
			splitter->setSplitSize(height, -1);
			tgui::resize(NULL);
		}
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (!opened) {
			if (rel_x >= 0)
				hover = -3;
			else
				hover = -4;
		}
		else {
			int index = getIndex(abs_x, abs_y);

			if (index < 0)
				hover = index;
			else
				hover = top+index;
		}
	}

	virtual void draw(int abs_x, int abs_y) {
		int prev_x, prev_y, prev_w, prev_h;
		tgui::getClip(&prev_x, &prev_y, &prev_w, &prev_h);
		int w, h;
		tgui::getScreenSize(&w, &h);
		tgui::setClip(0, 0, w+2, h);

		if (opened) {
			int show = getShow();
			int y;

			y = abs_y;
			al_draw_filled_rectangle(
				abs_x,
				y,
				abs_x+width+1,
				y+HEIGHT,
				hover == -1 ? al_map_rgb(0, 0x27, 0xff) : bgColor
			);

			int tsz = (HEIGHT - 9) / 2;

			al_draw_filled_triangle(
				abs_x+width/2, y+HEIGHT/2-tsz,
				abs_x+width/2-tsz, y+HEIGHT/2+tsz,
				abs_x+width/2+tsz, y+HEIGHT/2+tsz,
				top == 0 ? highlightColor : textColor
			);
			y = abs_y+HEIGHT*(show+1);
			al_draw_filled_rectangle(
				abs_x,
				y,
				abs_x+width+1,
				y+HEIGHT,
				hover == -2 ? al_map_rgb(0, 0x27, 0xff) : bgColor
			);
			al_draw_filled_triangle(
				abs_x+width/2, y+HEIGHT/2+tsz,
				abs_x+width/2-tsz, y+HEIGHT/2-tsz,
				abs_x+width/2+tsz, y+HEIGHT/2-tsz,
				top+show >= (int)values.size() ? highlightColor : textColor
			);

			y = abs_y + HEIGHT;

			if (values.size()) {
				for (int i = 0; i < show; i++) {
					ALLEGRO_COLOR color;
					ALLEGRO_COLOR color2;

					if (hover == top+i) {
						color = highlightColor;
						color2 = al_map_rgb(255, 255, 255);
					}
					else {
						color = bgColor;
						color2 = textColor;
					}

					al_draw_filled_rectangle(
						abs_x,
						y,
						abs_x+width+1,
						y+HEIGHT,
						color
					);

					al_draw_text(
						tgui::getFont(),
						color2,
						abs_x+4,
						y+4,
						0,
						((top+i == selected ? "*" : "") + values[top+i]).c_str()
					);

					y += HEIGHT;
				}
			}
		}
		else {
			ALLEGRO_COLOR color;

			if (hover == -3) {
				color = highlightColor;
			}
			else {
				color = bgColor;
			}

			al_draw_filled_rectangle(
				abs_x,
				abs_y,
				abs_x+width+1,
				abs_y+HEIGHT,
				color
			);

			int tsz = (HEIGHT - 9) / 2;

			int tx = abs_x+width-15;
			int ty = abs_y+2;

			al_draw_filled_triangle(
				tx, ty,
				tx-tsz, ty+tsz,
				tx+tsz, ty+tsz,
				textColor
			);

			ty = abs_y + (2+5+tsz);

			al_draw_filled_triangle(
				tx-tsz, ty,
				tx+tsz, ty,
				tx, ty+tsz,
				textColor
			);

			if (values.size()) {
				al_draw_text(
					tgui::getFont(),
					textColor,
					abs_x+4,
					abs_y+4,
					0,
					values[selected].c_str()
				);
			}
		}

		tgui::setClip(prev_x, prev_y, prev_w, prev_h);
	};

	void addValue(std::string value) {
		values.push_back(value);
	}

	void clearValues(void) {
		values.clear();
	}

	void setNumItems(int n) {
		clearValues();
		for (int i = 0; i < n; i++) {
			char buf[20];
			sprintf_s(buf, 20, "Layer %d", i);
			addValue(std::string(buf));
		}
	}

	void set_callback(void (*callback)(int selected))
	{
		this->callback = callback;
	}

	void close_combo()
	{
		A_Splitter_Panel *panel = (A_Splitter_Panel *)parent;
		A_Splitter *splitter = (A_Splitter *)(panel->splitter);
		opened = false;
		hover = -4;
		height = HEIGHT;
		splitter->setSplitSize(height, -1);
		tgui::resize(NULL);
	}

	A_Combobox(
		int maxShown,
		ALLEGRO_COLOR bgColor,
		ALLEGRO_COLOR textColor,
		ALLEGRO_COLOR highlightColor
	) :
		maxShown(maxShown),
		bgColor(bgColor),
		textColor(textColor),
		highlightColor(highlightColor),
		opened(false),
		hover(-4),
		top(0),
		selected(0)
	{
		x = y = 0;
		if (this->maxShown < 0) {
			this->maxShown = 5;
		}

		callback = NULL;

		ignoreMouseDown = 0.0; // hack because we get multiple mouseDowns
	}

protected:
	std::vector<std::string> values;
	int maxShown;
	ALLEGRO_COLOR bgColor;
	ALLEGRO_COLOR textColor;
	ALLEGRO_COLOR highlightColor;
	bool opened;
	int hover;
	int top;
	int selected;
	void (*callback)(int selected);
	double ignoreMouseDown;
};

class A_Scrollable {
public:
	virtual void shelteredDraw(int offs_x, int offs_y, int w, int h) {};
};

class A_Scrollpane : public tgui::TGUIWidget {
public:
	static const int THICKNESS = 20;

	bool getAbsoluteChildPosition(tgui::TGUIWidget *widget, int *x, int *y)
	{
		if (child == widget) {
			int xx, yy;
			tgui::determineAbsolutePosition(this, &xx, &yy);
			*x = xx;
			*y = yy;
			return true;
		}

		if (child->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}

		return false;
	}

	int getBarLength(int widgetSize, int scrollSize) {
		if (widgetSize > scrollSize)
			return 0;
		if (widgetSize == 0)
			return 0;
		double screens = (double)scrollSize / widgetSize;
		if (screens == 0)
			return 0;
		int len = widgetSize / screens;
		if (len > widgetSize-1)
			len = widgetSize-1;
		return len;
	}

	int getTotalLength(int widgetSize, int barSize) {
		return widgetSize-barSize-THICKNESS-1;
	}

	void draw_self(int abs_x, int abs_y) {
		int xx = abs_x+width-THICKNESS;
		int yy = abs_y;
		int xx2 = xx+THICKNESS;
		int yy2 = yy+height-THICKNESS;

		al_draw_filled_rectangle(
			xx, yy, xx2, yy2,
			troughColor
		);

		int barSize = getBarLength(height-THICKNESS, size_y);
		int totalLength = getTotalLength(height, barSize);
		int bpos = p1 * totalLength;

		if (barSize > 0) {
			al_draw_filled_rectangle(
				xx,
				abs_y+bpos,
				xx2,
				abs_y+bpos+barSize+1,
				sliderColor
			);
		}

		xx = abs_x;
		yy = abs_y+height-THICKNESS;
		xx2 = xx+width-THICKNESS;
		yy2 = yy+THICKNESS;

		al_draw_filled_rectangle(
			xx, yy, xx2, yy2,
			troughColor
		);

		barSize = getBarLength(width-THICKNESS, size_x);
		totalLength = getTotalLength(width, barSize);
		bpos = p2 * totalLength;

		if (barSize > 0) {
			al_draw_filled_rectangle(
				abs_x+bpos,
				yy,
				abs_x+bpos+barSize+1,
				yy2,
				sliderColor
			);
		}
	}

	void setScrollSize(int w, int h) {
		size_x = w;
		size_y = h;
	}

	void getScrollSize(int *w, int *h) {
		if (w)
			*w = size_x;
		if (h)
			*h = size_y;
	}

	void addScrollable(TGUIWidget *w) {
		A_Scrollable *test = dynamic_cast<A_Scrollable *>(w);
		if (!test) {
			return;
		}

		w->setParent(this);
		child = w;
	}

	int getOffsetX(void) {
		int ox = p2 * (size_x-(width-THICKNESS));
		if (ox < 0) ox = 0;
		return ox;
	}

	int getOffsetY(void) {
		int oy = p1 * (size_y-(height-THICKNESS));
		if (oy < 0) oy = 0;
		return oy;
	}

	int getSizeX(void) {
		return size_x;
	}

	int getSizeY(void) {
		return size_y;
	}

	virtual void scrollBy(int rel_x, int rel_y) {
		int sw, sh;
		getScrollSize(&sw, &sh);
		p1 += (double)rel_y / (sh-(height-THICKNESS));
		p2 += (double)rel_x / (sw-(width-THICKNESS));
		if (p1 < 0) p1 = 0;
		else if (p1 > 1) p1 = 1;
		if (p2 < 0) p2 = 0;
		else if (p2 > 1) p2 = 1;
	}

	virtual void mouseScroll(int z, int w, int dz, int dw) {
#ifdef ALLEGRO_WINDOWS
		if (reverse_scroll) {
			dz = -dz;
		}
		else {
			dw = -dw;
		}
#elif defined ALLEGRO_UNIX
		if (reverse_scroll) {
			dz = -dz;
		}
#else
		// This works on macOS (with mouse, unsure of touchpad)
		if (reverse_scroll) {
			dz = -dz;
			dw = -dw;
		}
#endif
		scrollBy(dw*25, dz*40);
	}
   
	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (downOnSide) {
			int barSize = getBarLength(height-THICKNESS, size_y);
			int totalLength = getTotalLength(height, barSize);
			p1 = downP + ((float)(abs_y - downAt) / totalLength);
			if (p1 < 0) p1 = 0;
			else if (p1 > 1) p1 = 1;
		}
		else if (downOnBottom) {
			int barSize = getBarLength(width-THICKNESS, size_x);
			int totalLength = getTotalLength(width, barSize);
			p2 = downP + ((float)(abs_x - downAt) / totalLength);
			if (p2 < 0) p2 = 0;
			else if (p2 > 1) p2 = 1;
		}
		else if (downWithSpace) {
			int barSize = getBarLength(height-THICKNESS, size_y);
			int totalLength = getTotalLength(height, barSize);
			p1 = downPY + ((float)(downAtY - abs_y) / totalLength);
			if (p1 < 0) p1 = 0;
			else if (p1 > 1) p1 = 1;

			barSize = getBarLength(width-THICKNESS, size_x);
			totalLength = getTotalLength(width, barSize);
			p2 = downPX + ((float)(downAtX - abs_x) / totalLength);
			if (p2 < 0) p2 = 0;
			else if (p2 > 1) p2 = 1;
		}
		else if (rel_x >= 0 && child) {
			int cx = rel_x + getOffsetX();
			int cy = rel_y + getOffsetY();
			child->mouseMove(cx, cy, abs_x, abs_y);
		}
		if (down) {
			int me_x, me_y;
			tgui::determineAbsolutePosition(this, &me_x, &me_y);
			int rx = abs_x - me_x;
			int ry = abs_y - me_y;
			int amount_x;
			int amount_y;
			if (rx > (width-THICKNESS)) {
				amount_x = rx - (width-THICKNESS);
			}
			else if (rx < 0) {
				amount_x = rx;
			}
			else {
				amount_x = 0;
			}
			if (ry > (height-THICKNESS)) {
				amount_y = ry - (height-THICKNESS);
			}
			else if (ry < 0) {
				amount_y = ry;
			}
			else {
				amount_y = 0;
			}
			int warp_x = -1;
			int warp_y = -1;
			if (amount_x >= 0 && scrolled_x >= 0) {
				if (amount_x > scrolled_x) {
					amount_x = amount_x - scrolled_x;
					scrolled_x += amount_x;
					warp_x = me_x + (width - THICKNESS);
				}
				else {
					scrolled_x = amount_x;
					amount_x = 0;
				}
			}
			else if (amount_x <= 0 && scrolled_x <= 0) {
				if (amount_x < scrolled_x) {
					amount_x = -(std::abs(amount_x) - std::abs(scrolled_x));
					scrolled_x += amount_x;
					warp_x = me_x;
				}
				else {
					scrolled_x = amount_x;
					amount_x = 0;
				}
			}
			else {
				scrolled_x = amount_x;
			}
			if (amount_y >= 0 && scrolled_y >= 0) {
				if (amount_y > scrolled_y) {
					amount_y = amount_y - scrolled_y;
					scrolled_y += amount_y;
					warp_y = me_y + (height - THICKNESS);
				}
				else {
					scrolled_y = amount_y;
					amount_y = 0;
				}
			}
			else if (amount_y <= 0 && scrolled_y <= 0) {
				if (amount_y < scrolled_y) {
					amount_y = -(std::abs(amount_y) - std::abs(scrolled_y));
					scrolled_y += amount_y;
					warp_y = me_y;
				}
				else {
					scrolled_y = amount_y;
					amount_y = 0;
				}
			}
			else {
				scrolled_y = amount_y;
			}
			scrollBy(amount_x, amount_y);
			if ((warp_x != -1) || (warp_y != -1)) {
				if (warp_x == -1) {
					warp_x = abs_x;
				}
				if (warp_y == -1) {
					warp_y = abs_y;
				}
				al_set_mouse_xy(display, warp_x, warp_y);
			}
		}
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (rel_x > (width-THICKNESS)) {
			int barSize = getBarLength(height-THICKNESS, size_y);
			int totalLength = getTotalLength(height, barSize);
			int bpos = p1 * totalLength;
			if (rel_y >= bpos && rel_y <= bpos+barSize) {
				downOnSide = true;
				downAt = abs_y;
				downP = p1;
			}
			else if (rel_y >= bpos) {
				p1 += (float)barSize/totalLength;
				scrollBy(0, -General::tileSize*General::scale);
				if (p1 > 1) p1 = 1;
			}
			else {
				p1 -= (float)barSize/totalLength;
				scrollBy(0, General::tileSize*General::scale);
				if (p1 < 0) p1 = 0;
			}
		}
		else if (rel_y > (height-THICKNESS)) {
			int barSize = getBarLength(width-THICKNESS, size_x);
			int totalLength = getTotalLength(width, barSize);
			int bpos = p2 * totalLength;
			if (rel_x >= bpos && rel_x <= bpos+barSize) {
				downOnBottom = true;
				downAt = abs_x;
				downP = p2;
			}
			else if (rel_x >= bpos) {
				p2 += (float)barSize/totalLength;
				scrollBy(-General::tileSize*General::scale, 0);
				if (p2 > 1) p2 = 1;
			}
			else {
				p2 -= (float)barSize/totalLength;
				scrollBy(General::tileSize*General::scale, 0);
				if (p2 < 0) p2 = 0;
			}
		}
		else if (tgui::isKeyDown(ALLEGRO_KEY_SPACE) && rel_x >= 0 && rel_x <= (width-THICKNESS) && rel_y >= 0 && rel_y <= (height-THICKNESS)) {
			downWithSpace = true;
			downAtX = abs_x;
			downAtY = abs_y;
			downPY = p1;
			downPX = p2;
		}
		else if (rel_x >= 0 && child) {
			down = true;
			scrolled_x = 0;
			scrolled_y = 0;
			int cx = rel_x + getOffsetX();
			int cy = rel_y + getOffsetY();
			child->mouseDown(cx, cy, abs_x, abs_y, mb);
		}
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		down = false;
		downWithSpace = false;
		if (downOnSide || downOnBottom) {
			downOnSide = downOnBottom = false;
		}
		else {
			int cx = rel_x + getOffsetX();
			int cy = rel_y + getOffsetY();
			child->mouseUp(cx, cy, abs_x, abs_y, mb);
		}
	}

	virtual void draw(int abs_x, int abs_y)
	{
		draw_self(abs_x, abs_y);

		if (!child)
			return;
		int prev_x, prev_y, prev_w, prev_h;
		tgui::getClip(&prev_x, &prev_y, &prev_w, &prev_h);
		tgui::setClip(abs_x, abs_y, width-THICKNESS, height-THICKNESS);
		int offs_x = getOffsetX();
		int offs_y = getOffsetY();
		A_Scrollable *scrollable = dynamic_cast<A_Scrollable *>(child);
		scrollable->shelteredDraw(offs_x, offs_y, width-THICKNESS, height-THICKNESS);
		tgui::setClip(prev_x, prev_y, prev_w, prev_h);
	}

	A_Scrollpane(ALLEGRO_COLOR troughColor, ALLEGRO_COLOR sliderColor) :
		size_x(0),
		size_y(0),
		p1(0),
		p2(0),
		troughColor(troughColor),
		sliderColor(sliderColor),
		downOnBottom(false),
		downOnSide(false),
		down(false),
		downWithSpace(false)
	{
		x = y = 0;
	}

protected:
	int size_x;
	int size_y;
	float p1, p2;
	ALLEGRO_COLOR troughColor;
	ALLEGRO_COLOR sliderColor;
	bool downOnBottom;
	bool downOnSide;
	int downAt;
	float downP;
	bool down;
	int scrolled_x;
	int scrolled_y;
	bool downWithSpace;
	float downPX;
	float downPY;
	int downAtX;
	int downAtY;
};

extern A_Scrollpane *levelScrollpane;
extern A_Scrollpane *tileScrollpane;

class A_Canvas : public A_Scrollable, public tgui::TGUIWidget {
public:
	typedef void (*drawCallback)(int ox, int oy, int dx, int dy, int w, int h, int layer);

	virtual void shelteredDraw(int offs_x, int offs_y, int w, int h) {
		int abs_x, abs_y;
		tgui::determineAbsolutePosition(this, &abs_x, &abs_y);
		callback(offs_x, offs_y, abs_x, abs_y, w, h, -1);
	}

	A_Canvas(drawCallback callback) :
		callback(callback)
	{
		x = y = 0;
	}

protected:
	drawCallback callback;
};

class A_Tileselector : public A_Canvas {
public:
	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (rel_x >= 0) {
			down = true;
			selected_x = rel_x / General::tileSize / General::scale;
			selected_y = rel_y / General::tileSize / General::scale;
			selected_w = 1;
			selected_h = 1;
		}
	}
	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (down) {
			int x = rel_x / General::tileSize / General::scale;
			int y = rel_y / General::tileSize / General::scale;
			selected_w = x - selected_x;
			selected_h = y - selected_y;
			if (x < selected_x) {
				selected_w--;
			}
			else {
				selected_w++;
			}
			if (y < selected_y) {
				selected_h--;
			}
			else {
				selected_h++;
			}
		}
	}
	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb)
	{
		down = false;
	}

	void getSelected(int *x, int *y, int *w, int *h) {
		if (selected_w < 0) {
			if (x)
				*x = selected_x + selected_w + 1;
			if (w)
				*w = -selected_w;
		}
		else {
			if (x)
				*x = selected_x;
			if (w)
				*w = selected_w;\
		}

		if (selected_h < 0) {
			if (y)
				*y = selected_y + selected_h + 1;
			if (h)
				*h = -selected_h;
		}
		else {
			if (y)
				*y = selected_y;
			if (h)
				*h = selected_h;
		}
	}

	void setSelected(int x, int y, int w, int h)
	{
		selected_x = x;
		selected_y = y;
		selected_w = w;
		selected_h = h;
	}

	A_Tileselector(drawCallback callback) :
		A_Canvas(callback),
		selected_x(0),
		selected_y(0),
		selected_w(1),
		selected_h(1),
		down(false)
	{
	}

protected:
	int selected_x;
	int selected_y;
	int selected_w;
	int selected_h;
	bool down;
};

extern A_Tileselector *tileSelector;

extern A_Combobox *layerCombo, *sheetCombo;

class A_Leveleditor : public A_Canvas {
public:
	static const int TOOL_PENCIL = 0;
	static const int TOOL_CLEAR = 1;
	static const int TOOL_SOLID = 2;
	static const int TOOL_MACRO = 3;
	static const int TOOL_CLONE = 4;
	static const int TOOL_MOVER = 5;
	static const int TOOL_FILL = 6;
	static const int TOOL_MARQUEE = 7;
	static const int TOOL_MAGIC_WAND = 8;
#ifdef SUPPORT_GROUPS
	static const int TOOL_WALL = 9;
#endif
	static const int TOOL_RMB_FILL = 10;

	struct _TilePlusPlus {
		int x, y, layer, number, sheet;
		bool solid;
		int tool;
		int ts_x, ts_y, ts_w, ts_h;
	};

	// tile flags are an optimization for the editor, not used in the saved levels
	static const int TF_USED = 1; // for flood fills, this bit determines if it has been spread to yet. only on layer 0
	static const int TF_MARKED = 2; // for solids, this bit tells if the tile is marked. only on layer 0

	struct _Tile {
		int16_t number;
		int16_t sheet;
		bool solid;
		unsigned char x, y;
		unsigned char flags;
	};

#ifdef SUPPORT_GROUPS
	struct Group {
		int type, layer, x, y, w, h;
	};

	struct Wall {
		int x, y, z;
		int size_x, size_y, size_z;
	};
#endif

	std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > get_floating_bounds(int savedx, int savedy, int ox, int oy)
	{
		std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > v;

		int mx, my;
		get_marquee_float_xy(&mx, &my);

		for (size_t yy = 0; yy < marquee_buffer.size(); yy++) {
			for (size_t xx = 0; xx < marquee_buffer[0].size(); xx++) {
				if (marquee_buffer[yy][xx].size() == 0) {
					continue;
				}

				int tx = xx + mx;
				int ty = yy + my;

				std::pair< std::pair<float, float>, std::pair<float, float> > p;
				std::pair<float, float> a, b;

				//top
				if (yy == 0 || marquee_buffer[yy-1][xx].size() == 0) {
					a.first = (tx*General::tileSize*General::scale);
					a.second = (ty*General::tileSize*General::scale)+1;
					b.first = ((tx+1)*General::tileSize*General::scale);
					b.second = (ty*General::tileSize*General::scale)+1;
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//left
				if (xx == 0 || marquee_buffer[yy][xx-1].size() == 0) {
					a.first = (tx*General::tileSize*General::scale)+1;
					a.second = (ty*General::tileSize*General::scale);
					b.first = (tx*General::tileSize*General::scale)+1;
					b.second = ((ty+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//right
				if (xx == marquee_buffer[0].size()-1 || marquee_buffer[yy][xx+1].size() == 0) {
					a.first = ((tx+1)*General::tileSize*General::scale);
					a.second = (ty*General::tileSize*General::scale);
					b.first = ((tx+1)*General::tileSize*General::scale);
					b.second = ((ty+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//bottom
				if (yy == marquee_buffer.size()-1 || marquee_buffer[yy+1][xx].size() == 0) {
					a.first = (tx*General::tileSize*General::scale);
					a.second = ((ty+1)*General::tileSize*General::scale);
					b.first = ((tx+1)*General::tileSize*General::scale);
					b.second = ((ty+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}
			}
		}

		return v;
	}

	std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > get_marked_bounds(int savedx, int savedy, int ox, int oy)
	{
		std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > v;

		for (size_t my = 0; my < tiles.size(); my++) {
			for (size_t mx = 0; mx < tiles[0].size(); mx++) {
				if ((tiles[my][mx][0].flags & TF_MARKED) == 0) {
					continue;
				}

				std::pair< std::pair<float, float>, std::pair<float, float> > p;
				std::pair<float, float> a, b;

				//top
				if (my == 0 || (tiles[my-1][mx][0].flags & TF_MARKED) == 0) {
					a.first = (mx*General::tileSize*General::scale);
					a.second = (my*General::tileSize*General::scale)+1;
					b.first = ((mx+1)*General::tileSize*General::scale);
					b.second = (my*General::tileSize*General::scale)+1;
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//left
				if (mx == 0 || (tiles[my][mx-1][0].flags & TF_MARKED) == 0) {
					a.first = (mx*General::tileSize*General::scale)+1;
					a.second = (my*General::tileSize*General::scale);
					b.first = (mx*General::tileSize*General::scale)+1;
					b.second = ((my+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//right
				if (mx == tiles[0].size()-1 || (tiles[my][mx+1][0].flags & TF_MARKED) == 0) {
					a.first = ((mx+1)*General::tileSize*General::scale);
					a.second = (my*General::tileSize*General::scale);
					b.first = ((mx+1)*General::tileSize*General::scale);
					b.second = ((my+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}

				//bottom
				if (my == tiles.size()-1 || (tiles[my+1][mx][0].flags & TF_MARKED) == 0) {
					a.first = (mx*General::tileSize*General::scale);
					a.second = ((my+1)*General::tileSize*General::scale);
					b.first = ((mx+1)*General::tileSize*General::scale);
					b.second = ((my+1)*General::tileSize*General::scale);
					p.first = a;
					p.second = b;
					//if (on_screen(a, savedx, savedy, ox, oy) || on_screen(b, savedx, savedy, ox, oy)) {
						v.push_back(p);
					//}
				}
			}
		}

		return v;
	}

	bool getRecording(void) {
		return recording;
	}

	int getNumLayers(void) {
		return layers;
	}

	int getLayers(void) {
		return layers;
	}

#ifdef SUPPORT_GROUPS
	std::vector<Group> &getGroups() {
		return groups;
	}

	std::vector<Wall> &getWalls() {
		return walls;
	}
#endif

	_Tile createEmpty_Tile(void) {
		_Tile t;
		t.number = -1;
		t.sheet = -1;
		t.solid = false;
		t.x = -1;
		t.y = -1;
		t.flags = 0;
		return t;
	}

	std::vector<_Tile> createEmptyTile(void) {
		std::vector<_Tile> tile;
		for (int k = 0; k < layers; k++) {
			_Tile t = createEmpty_Tile();
			tile.push_back(t);
		}
		return tile;
	}

	std::vector< std::vector<_Tile> > createRow(int w) {
		std::vector< std::vector<_Tile> > row;
		std::vector<_Tile> tile = createEmptyTile();
		for (int j = 0; j < w; j++) {
			row.push_back(tile);
		}
		return row;
	}

	void resizeScrollpane(void) {
		A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
		if (scrollpane) {
			scrollpane->setScrollSize(
				tiles[0].size()*(General::tileSize*General::scale),
				tiles.size()*(General::tileSize*General::scale)
			);
		}
	}

	void deleteLayer(int i) {
		if (layers < 2) {
			al_show_native_message_box(display, "Error", "Invalid input!", "Cannot delete the only layer!", 0, 0);
			return;
		}


		push_undo();

		if (i == 0) {
			for (size_t y = 0; y < tiles.size(); y++) {
				for (size_t x = 0; x < tiles[0].size(); x++) {
					tiles[y][x][1].flags = tiles[y][x][0].flags;
				}
			}
		}

		if (marquee_buffer_filled && marquee_all) {
			for (size_t y = 0; y < marquee_buffer.size(); y++) {
				for (size_t x = 0; x < marquee_buffer[y].size(); x++) {
					if (i < (int)marquee_buffer[y][x].size()) {
						std::vector<_Tile>::iterator it = marquee_buffer[y][x].begin() + i;
						marquee_buffer[y][x].erase(it);
					}
				}
			}
		}

		for (unsigned int y = 0; y < tiles.size(); y++) {
			for (unsigned int x = 0; x < tiles[y].size(); x++) {
				std::vector<_Tile>::iterator it = tiles[y][x].begin() + i;
				tiles[y][x].erase(it);
			}
		}

		visible.erase(visible.begin() + i);

#ifdef SUPPORT_GROUPS
		std::vector<Group>::iterator it;
		for (it = groups.begin(); it != groups.end();) {
			Group &g = *it;
			if (g.layer == i) {
				it = groups.erase(it);
			}
			else {
				if (g.layer > i) {
					g.layer--;
				}
				it++;
			}
		}

		// FIXME!!!: erase walls
		
		draw_groups.erase(draw_groups.begin() + i);
#endif

		if (level_type != CP) {
			draw_solids.erase(draw_solids.begin() + 1);
		}
		else {
			draw_solids.erase(draw_solids.begin() + i);
		}

		layers--;
		if (layer >= layers) {
			layer = layers-1;
		}

		layerCombo->setNumItems(layers);
		layerCombo->setSelected(layer);
		
		if (layers < 2) {
			disable_layer_delete_menu_item();
		}
		enable_layer_before_menu_item();
		enable_layer_after_menu_item();
	
		changed = true;
	}

	void insertLayer(int i) {
		if (tiles[0][0].size() >= MAX_LAYERS) {
			char buf[1000];
			sprintf(buf, "Maps cannot have more than %d layers!", MAX_LAYERS);
			al_show_native_message_box(display, "Error", "Invalid input!", buf, 0, 0);
			return;
		}

		push_undo();
		
		_Tile t = createEmpty_Tile();

		if (marquee_buffer_filled && marquee_all) {
			for (size_t y = 0; y < marquee_buffer.size(); y++) {
				for (size_t x = 0; x < marquee_buffer[y].size(); x++) {
					if (marquee_buffer[y][x].size() > 0) {
						std::vector<_Tile>::iterator it;
						if (i < 0)
							it = marquee_buffer[y][x].end();
						else
							it = marquee_buffer[y][x].begin() + i;
						marquee_buffer[y][x].insert(it, t);
					}
				}
			}
		}
		
		for (unsigned int y = 0; y < tiles.size(); y++) {
			for (unsigned int x = 0; x < tiles[y].size(); x++) {
				std::vector<_Tile>::iterator it;
				if (i < 0)
					it = tiles[y][x].end();
				else
					it = tiles[y][x].begin() + i;
				tiles[y][x].insert(it, t);
			}
		}

		if (i == 0) {
			for (size_t y = 0; y < tiles.size(); y++) {
				for (size_t x = 0; x < tiles[0].size(); x++) {
					tiles[y][x][0].flags = tiles[y][x][1].flags;
					tiles[y][x][1].flags = 0;
				}
			}
		}

		std::vector<bool>::iterator it;
		if (i < 0)
			it = visible.end();
		else
			it = visible.begin() + i;
		visible.insert(it, true);

#ifdef SUPPORT_GROUPS
		if (i >= 0) {
			for (size_t j = 0; j < groups.size(); j++) {
				Group &g = groups[j];
				if (g.layer >= i) {
					g.layer++;
				}
			}
		}
		
		if (i < 0)
			it = draw_groups.end();
		else
			it = draw_groups.begin() + i;
		draw_groups.insert(it, true);

		// FIXME!!!: walls
#endif

		if (i < 0 || level_type != CP) {
			it = draw_solids.end();
		}
		else {
			it = draw_solids.begin() + i;
		}
		draw_solids.insert(it, true);

		layers++;

		if (i == -1) {
			layer = layers-1;
		}
		else {
			layer = i;
		}
		
		layerCombo->setNumItems(layers);
		layerCombo->setSelected(layer);
		
		if (tiles[0][0].size() >= MAX_LAYERS) {
			disable_layer_before_menu_item();
			disable_layer_after_menu_item();
		}
		enable_layer_delete_menu_item();
	
		changed = true;
	}

	void deleteRow(int i) {
		if ((int)tiles.size() <= i) return;

		for (int x = 0; x < tiles[i].size(); x++) {
			if (tiles[i][x][0].flags & TF_MARKED) {
				tiles[i][x][0].flags &= ~TF_MARKED;
				num_marked--;
				cols_marked[x]--;
				rows_marked[i]--;
			}
		}

		if (num_marked == 0) {
			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
		}
		else {
			enable_copy_menu_item();
			enable_copy_all_menu_item();
			enable_cut_menu_item();
			enable_cut_all_menu_item();
			enable_delete_menu_item();
			enable_delete_all_menu_item();
			enable_select_none_menu_item();
		}
		if (num_marked == tiles.size()*tiles[0].size()) {
			disable_select_all_menu_item();
		}
		else {
			enable_select_all_menu_item();
		}

		push_undo();
		tiles.erase(tiles.begin() + i);
		rows_marked.erase(rows_marked.begin() + i);
		resizeScrollpane();

#ifdef SUPPORT_GROUPS
		for (size_t j = 0; j < groups.size(); j++) {
			Group &g = groups[j];
			if (g.y > i) {
				g.y--;
			}
			else if (i >= g.y && i < g.y+g.h) {
				g.h--;
			}
		}

		for (size_t j = 0; j < walls.size(); j++) {
			Wall &w = walls[j];
			if (w.y > i) {
				w.y--;
			}
			else if (i >= w.y && i < w.y+w.size_y) {
				w.size_y--;
			}
		}
#endif

		changed = true;
	}

	void deleteColumn(int i) {
		if ((int)tiles[0].size() <= i) return;
		push_undo();
		for (unsigned int y = 0; y < tiles.size(); y++) {
			if (tiles[y][i][0].flags & TF_MARKED) {
				tiles[y][i][0].flags &= ~TF_MARKED;
				num_marked--;
				cols_marked[i]--;
				rows_marked[y]--;
			}

			tiles[y].erase(tiles[y].begin() + i);
		}
		cols_marked.erase(cols_marked.begin() + i);
		resizeScrollpane();

		if (num_marked == 0) {
			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
		}
		else {
			enable_copy_menu_item();
			enable_copy_all_menu_item();
			enable_cut_menu_item();
			enable_cut_all_menu_item();
			enable_delete_menu_item();
			enable_delete_all_menu_item();
			enable_select_none_menu_item();
		}
		if (num_marked == tiles.size()*tiles[0].size()) {
			disable_select_all_menu_item();
		}
		else {
			enable_select_all_menu_item();
		}

#ifdef SUPPORT_GROUPS
		for (size_t j = 0; j < groups.size(); j++) {
			Group &g = groups[j];
			if (g.x > i) {
				g.x--;
			}
			else if (i >= g.x && i < g.x+g.w) {
				g.w--;
			}
		}

		for (size_t j = 0; j < walls.size(); j++) {
			Wall &w = walls[j];
			if (w.x > i) {
				w.x--;
			}
			else if (i >= w.x && i < w.x+w.size_x) {
				w.size_x--;
			}
		}
#endif

		changed = true;
	}

	void insertRow(int i) {
		if (tiles.size() >= MAX_LEVEL_SIZE) {
			char buf[1000];
			sprintf(buf, "Maps cannot be more than %d rows high!", MAX_LEVEL_SIZE);
			al_show_native_message_box(display, "Error", "Invalid input!", buf, 0, 0);
			return;
		}

		push_undo();
		std::vector< std::vector<_Tile> > row = createRow(tiles[0].size());
		std::vector< std::vector< std::vector<_Tile> > >::iterator it;
		if (i < 0)
			it = tiles.end();
		else
			it = tiles.begin() + i;
		tiles.insert(it, row);

		std::vector<int>::iterator it2;
		if (i < 0)
			it2 = rows_marked.end();
		else
			it2 = rows_marked.begin() + i;
		rows_marked.insert(it2, 0);
		resizeScrollpane();

#ifdef SUPPORT_GROUPS
		for (size_t j = 0; j < groups.size(); j++) {
			Group &g = groups[j];
			if (g.y >= i) {
				g.y++;
			}
			else if (i > g.y && i < g.y+g.h) {
				g.h++;
			}
		}

		for (size_t j = 0; j < walls.size(); j++) {
			Wall &w = walls[j];
			if (w.y >= i) {
				w.y++;
			}
			else if (i > w.y && i < w.y+w.size_y) {
				w.size_y++;
			}
		}
#endif

		changed = true;
	}

	void insertColumn(int i) {
		if (tiles[0].size() >= MAX_LEVEL_SIZE) {
			char buf[1000];
			sprintf(buf, "Maps cannot be more than %d columns wide!", MAX_LEVEL_SIZE);
			al_show_native_message_box(display, "Error", "Invalid input!", buf, 0, 0);
			return;
		}

		push_undo();
		std::vector<_Tile> t = createEmptyTile();
		for (unsigned int y = 0; y < tiles.size(); y++) {
			if (i < 0) {
				tiles[y].insert(tiles[y].end(), t);
			}
			else {
				tiles[y].insert(tiles[y].begin() + i, t);
			}
		}

		if (i < 0) {
			cols_marked.insert(cols_marked.end(), 0);
		}
		else {
			cols_marked.insert(cols_marked.begin() + i, 0);
		}
		resizeScrollpane();

#ifdef SUPPORT_GROUPS
		for (size_t j = 0; j < groups.size(); j++) {
			Group &g = groups[j];
			if (g.x >= i) {
				g.x++;
			}
			else if (i > g.x && i < g.x+g.w) {
				g.w++;
			}
		}

		for (size_t j = 0; j < walls.size(); j++) {
			Wall &w = walls[j];
			if (w.x >= i) {
				w.x++;
			}
			else if (i > w.x && i < w.x+w.size_x) {
				w.size_x++;
			}
		}
#endif

		changed = true;
	}

	bool anchor_marquee(bool only_if_floating = false)
	{
		if (only_if_floating == false || marquee_floating) {
			push_undo();
			if (marquee_floating) {
				for (size_t yy = 0; yy < marquee_buffer.size(); yy++) {
					for (size_t xx = 0; xx < marquee_buffer[0].size(); xx++) {
						int layer_start;
						int layer_end;
						int sz = marquee_buffer[yy][xx].size();
						if (sz == 0) {
							continue;
						}
						else if (marquee_all) {
							layer_start = 0;
							layer_end = sz;
						}
						else {
							layer_start = layer;
							layer_end = layer+1;
						}
						for (int l = layer_start; l < layer_end; l++) {
							int tx = xx+marquee_float_x;
							int ty = yy+marquee_float_y;

							if (tx < 0 || ty < 0 || tx >= (int)tiles[0].size() || ty >= (int)tiles.size()) {
								continue;
							}

							if (marquee_buffer[yy][xx][marquee_all ? l : 0].number >= 0) {
								tiles[ty][tx][l].number = marquee_buffer[yy][xx][marquee_all ? l : 0].number;
								tiles[ty][tx][l].x = marquee_buffer[yy][xx][marquee_all ? l : 0].x;
								tiles[ty][tx][l].y = marquee_buffer[yy][xx][marquee_all ? l : 0].y;
								tiles[ty][tx][l].sheet = marquee_buffer[yy][xx][marquee_all ? l : 0].sheet;
							}

							if (one_solids_layer) {
								tiles[ty][tx][0].solid = marquee_buffer[yy][xx][0].solid;
							}
							else {
								tiles[ty][tx][l].solid = marquee_buffer[yy][xx][marquee_all ? l : 0].solid;
							}
							changed = true;
						}
					}
				}
			}
			marquee_marked = false;
			marquee_floating = false;
			dragging_marquee = false;

			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
			enable_select_all_menu_item();

			return true;
		}

		return false;
	}

	void delete_marquee(bool all_layers, bool do_undo = true)
	{
		if (!marquee_marked) {
			return;
		}
		if (do_undo) {
			push_undo();
		}
		int layer_start;
		int layer_end;
		if (all_layers) {
			layer_start = 0;
			layer_end = tiles[0][0].size();
		}
		else {
			layer_start = layer;
			layer_end = layer+1;
		}
		int x1, y1, x2, y2;
		get_marquee(&x1, &y1, &x2, &y2);
		// delete stuff from tiles
		for (int yy = y1; yy < y2; yy++) {
			for (int xx = x1; xx < x2; xx++) {
				for (int i = layer_start; i < layer_end; i++) {
					std::pair<int, int> p(xx, yy);
					if ((tiles[yy][xx][0].flags & TF_MARKED) == 0) {
						continue;
					}
					tiles[yy][xx][i].number = -1;
					tiles[yy][xx][i].x = -1;
					tiles[yy][xx][i].y = -1;
					tiles[yy][xx][i].sheet = -1;
					if (one_solids_layer) {
						tiles[yy][xx][0].solid = false;
					}
					else {
						tiles[yy][xx][i].solid = false;
					}
					changed = true;
				}
			}
		}

		selectNone();
	}

	void selectNone()
	{
		disable_copy_menu_item();
		disable_copy_all_menu_item();
		disable_cut_menu_item();
		disable_cut_all_menu_item();
		disable_delete_menu_item();
		disable_delete_all_menu_item();
		disable_select_none_menu_item();
		enable_select_all_menu_item();

		for (size_t y = 0; y < tiles.size(); y++) {
			for (size_t x = 0; x < tiles[0].size(); x++) {
				unsel(x, y);
			}
		}
	}

	void marquee_float_to_selection()
	{
		anchor_marquee();
		marquee_floating = false;
		marquee_marked = true;
		selectNone();
		for (size_t y = 0; y < marquee_buffer.size(); y++) {
			for (size_t x = 0; x < marquee_buffer[y].size(); x++) {
				if (marquee_buffer[y][x].size() == 0) {
					continue;
				}
				int xx = x + marquee_float_x;
				int yy = y + marquee_float_y;
				if (xx < 0 || yy < 0 || xx >= (int)tiles[0].size() || yy >= (int)tiles.size()) {
					continue;
				}
				sel(xx, yy);
			}
		}
		marquee_x1 = marquee_float_x;
		marquee_y1 = marquee_float_y;
		marquee_x2 = marquee_x1 + marquee_buffer[0].size() - 1;
		marquee_y2 = marquee_y1 + marquee_buffer.size() - 1;
	}

	void copy_cut(int keycode, bool all_layers)
	{
		bool did_undo = false;

		if  (marquee_floating) {
			did_undo = true;
			marquee_float_to_selection();
		}
		if (!marquee_marked) {
			return;
		}
		int layer_start;
		int layer_end;
		if (all_layers) {
			layer_start = 0;
			layer_end = tiles[0][0].size();
			marquee_all = true;
		}
		else {
			layer_start = layer;
			layer_end = layer+1;
			marquee_all = false;
		}
		marquee_buffer_filled = true;
		enable_paste_menu_item();
		marquee_buffer.clear();
		int x1, y1, x2, y2;
		get_marquee(&x1, &y1, &x2, &y2);
		for (int yy = y1; yy < y2; yy++) {
			std::vector< std::vector<_Tile> > r;
			for (int xx = x1; xx < x2; xx++) {
				std::pair<int, int> p(xx, yy);
				std::vector<_Tile> g;
				if ((tiles[yy][xx][0].flags & TF_MARKED) != 0) {
					for (int i = layer_start; i < layer_end; i++) {
						_Tile t;
						t.number = tiles[yy][xx][i].number;
						t.x = tiles[yy][xx][i].x;
						t.y = tiles[yy][xx][i].y;
						t.sheet = tiles[yy][xx][i].sheet;
						if (one_solids_layer) {
							t.solid = tiles[yy][xx][0].solid;
						}
						else {
							t.solid = tiles[yy][xx][i].solid;
						}
						g.push_back(t);
					}
				}
				r.push_back(g);
			}
			marquee_buffer.push_back(r);
		}
		if (keycode == ALLEGRO_KEY_X) {
			delete_marquee(all_layers, did_undo ? false : true);
		}
	}

	void paste()
	{
		if (marquee_buffer_filled) {
			anchor_marquee();

			A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
			int scroll_x = scrollpane->getOffsetX();
			int scroll_y = scrollpane->getOffsetY();

			if (scroll_x == marquee_copy_cut_scroll_x && scroll_y == marquee_copy_cut_scroll_y) {
				int x1, y1, x2, y2;
				get_marquee(&x1, &y1, &x2, &y2);
				int topx = x1;
				int topy = y1;
				marquee_float_x = topx;
				marquee_float_y = topy;
			}
			else {
				int offsx = ((A_Scrollpane *)parent)->getOffsetX();
				int offsy = ((A_Scrollpane *)parent)->getOffsetY();
				int spw = ((A_Scrollpane *)parent)->getWidth();
				int sph = ((A_Scrollpane *)parent)->getHeight();
				int rows = marquee_buffer.size();
				int cols = marquee_buffer[0].size();
				int pixw = rows * General::tileSize * General::scale;
				int pixh = cols * General::tileSize * General::scale;
				int topx = (offsx+spw/2) - pixw/2;
				int topy = (offsy+sph/2) - pixh/2;
				marquee_float_x = topx / General::tileSize / General::scale;
				marquee_float_y = topy / General::tileSize / General::scale;
			}
			marquee_floating = true;
			marquee_marked = false;
			enable_copy_menu_item();
			enable_copy_all_menu_item();
			enable_cut_menu_item();
			enable_cut_all_menu_item();
			enable_delete_menu_item();
			enable_delete_all_menu_item();
			enable_select_none_menu_item();
			if (tool != TOOL_MAGIC_WAND) {
				tool = TOOL_MARQUEE;
			}
		}
	}

	virtual void keyDown(int keycode) {
		bool using_mover = (tool == TOOL_MOVER);

#ifndef ALLEGRO_MACOSX
		if (keycode == ALLEGRO_KEY_I) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
					invertSelection();
				}
			}
		}
		else if (keycode == ALLEGRO_KEY_A) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				selectAll();
			}
		}
		else if (keycode == ALLEGRO_KEY_D) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				anchor_marquee();
			}
		}
#endif
		if (keycode == ALLEGRO_KEY_S) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
#ifndef ALLEGRO_MACOSX
				if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
					if (save(true) == false) {
						al_show_native_message_box(display, "Save Error", "Invalid input!", "File could not be saved at this location!", 0, 0);
					}
				}
				else {
					if (save(false) == false) {
						al_show_native_message_box(display, "Save Error", "Invalid input!", "File could not be saved at this location!", 0, 0);
					}
				}
				setTitle();
				al_flush_event_queue(queue);
#endif
			}
			else
			{
				tool = TOOL_SOLID;
			}
		}
		else if (keycode == ALLEGRO_KEY_Z) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
#ifndef ALLEGRO_MACOSX
				doUndo();
				layerCombo->close_combo();
#endif
			}
		}
		else if (keycode == ALLEGRO_KEY_Y) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
#ifndef ALLEGRO_MACOSX
				doRedo();
				layerCombo->close_combo();
#endif
			}
			else if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
				if (statusX < 0 || statusY < 0)
					return;
				deleteRow(statusY);
			}
		}
		else if (keycode == ALLEGRO_KEY_T) {
			if (!(tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL))) {
				visible[layer] = !visible[layer];
			}
		}
		else if (keycode == ALLEGRO_KEY_UP) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				if (statusX < 0 || statusY < 0)
					return;
				// insert row before cursor
				insertRow(statusY);
			}
			else if ((tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) && General::can_add_and_delete_layers) {
				insertLayer(layer);
				layerCombo->close_combo();
			}
		}
		else if (keycode == ALLEGRO_KEY_DOWN) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				if (statusX < 0 || statusY < 0)
					return;
				// insert row after cursor
				if (statusY == (int)tiles.size()-1)
					insertRow(-1);
				else
					insertRow(statusY+1);
			}
			else if ((tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) && General::can_add_and_delete_layers) {
				if (layer == layers-1)
					insertLayer(-1);
				else
					insertLayer(layer+1);
				layerCombo->close_combo();
			}
		}
		else if (keycode == ALLEGRO_KEY_R) {
			record();
		}
#ifndef ALLEGRO_MACOSX
		else if ((tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) && (keycode == ALLEGRO_KEY_C || keycode == ALLEGRO_KEY_X)) {
			copy_cut(keycode, tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT));
		}
		else if (keycode == ALLEGRO_KEY_DELETE) {
			bool do_undo = true;
			if  (marquee_floating) {
				do_undo = false;
				marquee_float_to_selection();
			}
			if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
				delete_marquee(true, do_undo);
			}
			else {
				delete_marquee(false, do_undo);
			}
		}
#endif
		else if (keycode == ALLEGRO_KEY_LEFT) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				if (statusX < 0 || statusY < 0)
					return;
				// insert row before cursor
				insertColumn(statusX);
			}
		}
		else if (keycode == ALLEGRO_KEY_RIGHT) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
				// insert row after cursor
				if (statusX == (int)tiles[0].size()-1)
					insertColumn(-1);
				else
					insertColumn(statusX+1);
			}
		}
		else if (keycode == ALLEGRO_KEY_X) {
			if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
				if (statusX < 0 || statusY < 0)
					return;
				deleteColumn(statusX);
			}
		}
		else if (keycode == ALLEGRO_KEY_C) {
			tool = TOOL_CLEAR;
		}
		else if (keycode == ALLEGRO_KEY_P) {
			tool = TOOL_PENCIL;
		}
		else if (keycode == ALLEGRO_KEY_Q) {
			tool = TOOL_MARQUEE;
		}
		else if (keycode == ALLEGRO_KEY_W) {
			tool = TOOL_MAGIC_WAND;
		}
		else if (keycode == ALLEGRO_KEY_M) {
			tool = TOOL_MACRO;
		}
		else if (keycode == ALLEGRO_KEY_F) {
			tool = TOOL_FILL;
		}
		else if (keycode == ALLEGRO_KEY_K) {
			if (statusX >= 0) {
				tool = TOOL_CLONE;
			}
		}
		else if (keycode == ALLEGRO_KEY_V) {
			if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
#ifndef ALLEGRO_MACOSX
				paste();
#endif
			}
			else {
				tool = TOOL_MOVER;
				mover_src_layer = layer;
				mover_dest_layer = -1;
				already_moved.clear();
			}
		}
		else if (keycode == ALLEGRO_KEY_L) {
			if ((tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) && General::can_add_and_delete_layers) {
				deleteLayer(layer);
				layerCombo->close_combo();
			}
		}
#ifdef SUPPORT_GROUPS
		else if (keycode == ALLEGRO_KEY_G) {
			if (is_marquee_marked()) {
				if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
					Group g = { group_type, layer, marquee_x1, marquee_y1, marquee_x2 - marquee_x1 + 1, marquee_y2 - marquee_y1 + 1};
					for (size_t i = 0; i < groups.size(); i++) {
						if (groups[i].layer == g.layer && groups[i].x == g.x && groups[i].y == g.y && groups[i].w == g.w && groups[i].h == g.h) {
							groups.erase(groups.begin() + i);

							break;
						}
					}
				}
				else {
					bool found = false;
					Group g = { group_type, layer, marquee_x1, marquee_y1, marquee_x2 - marquee_x1 + 1, marquee_y2 - marquee_y1 + 1};
					for (size_t i = 0; i < groups.size(); i++) {
						if (groups[i].layer == g.layer && groups[i].x == g.x && groups[i].y == g.y && groups[i].w == g.w && groups[i].h == g.h) {
							found = true;
							groups[i].type = group_type;
							break;
						}
					}
					if (!found) {
						groups.push_back(g);
					}
				}
				changed = true;
			}
		}
		else if (level_type == MO3 && keycode == ALLEGRO_KEY_XXXFIXME) {
			int found = -1;

			for (size_t i = 0; i < walls.size(); i++) {
				if (walls[i].x == statusX && walls[i].y == statusY) {
					found = i;
					break;
				}
			}

			if (found >= 0) {
				if (selected_wall == found) {
					walls.erase(walls.begin() + selected_wall);
					selected_wall = -1;
				}
				else {
					selected_wall = found;
				}
			}
			else {
				Wall w;
				w.x = statusX;
				w.y = statusY;
				w.z = 0;
				w.size_x = 1;
				w.size_y = 1;
				w.size_z = 1;
				walls.push_back(w);
				selected_wall = walls.size() - 1;
			}

			tool = TOOL_WALL;
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_LEFT) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					if (walls[selected_wall].size_x > 1) {
						walls[selected_wall].size_x--;
					}
				}
				else {
					walls[selected_wall].x--;
				}
			}
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_RIGHT) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					walls[selected_wall].size_x++;
				}
				else {
					walls[selected_wall].x++;
				}
			}
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_UP) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					if (walls[selected_wall].size_y > 1) {
						walls[selected_wall].size_y--;
					}
				}
				else {
					walls[selected_wall].y--;
				}
			}
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_DOWN) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					walls[selected_wall].size_y++;
				}
				else {
					walls[selected_wall].y++;
				}
			}
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_PGUP) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					walls[selected_wall].size_z++;
				}
				else {
					walls[selected_wall].z++;
				}
			}
		}
		else if (tool == TOOL_WALL && keycode == ALLEGRO_KEY_PGDN) {
			if ((int)walls.size() > selected_wall) {
				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					if (walls[selected_wall].size_z > 1) {
						walls[selected_wall].size_z--;
					}
				}
				else {
					walls[selected_wall].z--;
				}
			}
		}
#endif

		if (using_mover && tool != TOOL_MOVER) {
			already_moved.clear();
		}
	}

	void setMoverDestLayer(int l) {
		mover_dest_layer = l;
	}

	std::string getTool(void) {
		switch (tool) {
			case TOOL_PENCIL:
				return "Pencil";
			case TOOL_CLEAR:
				return "Clear";
			case TOOL_SOLID:
				return "Solids";
			case TOOL_MACRO:
				return "Macro";
			case TOOL_CLONE:
				return "Clone";
			case TOOL_MOVER:
				return "Layer Mover";
			case TOOL_FILL:
				return "Fill";
			case TOOL_MARQUEE:
				return "Marquee";
			case TOOL_MAGIC_WAND:
				return "Magic Wand";
		}
		return "?";
	}

	void getCloneStart(int *cx, int *cy)
	{
		*cx = cloneStartX;
		*cy = cloneStartY;
	}

	void getHoverPosition(int *xx, int *yy) {
		if (xx)
			*xx = statusX;
		if (yy)
			*yy = statusY;
	}

	void doUndo(void) {
		if (undoes.size() <= 0) return;

		int tool_bak = tool;

		push_redo();

		tiles = undoes.back();
		undoes.pop_back();
		
		sel_changed = true;

		layers = tiles[0][0].size();
		layerCombo->setNumItems(layers);

#ifdef SUPPORT_GROUPS
		groups = group_undoes.back();
		group_undoes.pop_back();

		walls = wall_undoes.back();
		wall_undoes.pop_back();
#endif

		Marquee m = marquee_undoes.back();
		marquee_x1 = m.marquee_x1;
		marquee_y1 = m.marquee_y1;
		marquee_x2 = m.marquee_x2;
		marquee_y2 = m.marquee_y2;
		marquee_marked = m.marquee_marked;
		marquee_buffer_filled = m.marquee_buffer_filled;
		marquee_buffer = m.marquee_buffer;
		marquee_all = m.marquee_all;
		marquee_floating = m.marquee_floating;
		marquee_float_x = m.marquee_float_x;
		marquee_float_y = m.marquee_float_y;
		dragging_marquee = m.dragging_marquee;
		marquee_drag_x = m.marquee_drag_x;
		marquee_drag_y = m.marquee_drag_y;
		tiles_bak = m.tiles_bak;
		marquee_curr_x1 = m.marquee_curr_x1;
		marquee_curr_y1 = m.marquee_curr_y1;
		marquee_curr_x2 = m.marquee_curr_x2;
		marquee_curr_y2 = m.marquee_curr_y2;
		marquee_undoes.pop_back();

		tool = tool_undoes.back();
		tool_undoes.pop_back();

		draw_solids = draw_solids_undoes.back();
		draw_solids_undoes.pop_back();

		visible = visible_undoes.back();
		visible_undoes.pop_back();

		changed = changed_undoes.back();
		changed_undoes.pop_back();

#ifdef SUPPORT_GROUPS
		draw_groups = draw_groups_undoes.back();
		draw_groups_undoes.pop_back();
#endif

		layer = layer_undoes.back();
		layer_undoes.pop_back();

		layerCombo->setSelected(layer);

		num_marked = num_marked_undoes.back();
		num_marked_undoes.pop_back();

		if (marquee_buffer_filled) {
			enable_paste_menu_item();
		}
		else {
			disable_paste_menu_item();
		}
		
		if (num_marked == 0) {
			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
		}
		else {
			enable_copy_menu_item();
			enable_copy_all_menu_item();
			enable_cut_menu_item();
			enable_cut_all_menu_item();
			enable_delete_menu_item();
			enable_delete_all_menu_item();
			enable_select_none_menu_item();
		}
		if (num_marked == tiles.size()*tiles[0].size()) {
			disable_select_all_menu_item();
		}
		else {
			enable_select_all_menu_item();
		}

		cols_marked = cols_marked_undoes.back();
		cols_marked_undoes.pop_back();

		rows_marked = rows_marked_undoes.back();
		rows_marked_undoes.pop_back();

		tool = tool_bak;

		enable_redo_menu_item();
		if (undoes.size() == 0) {
			disable_undo_menu_item();
		}
	}

	void doRedo(void) {
		if (redoes.size() <= 0) return;

		int tool_bak = tool;

		push_undo(false);

		tiles = redoes.back();
		redoes.pop_back();
		
		sel_changed = true;

		layers = tiles[0][0].size();
		layerCombo->setNumItems(layers);

#ifdef SUPPORT_GROUPS
		groups = group_redoes.back();
		group_redoes.pop_back();

		walls = wall_redoes.back();
		wall_redoes.pop_back();
#endif

		Marquee m = marquee_redoes.back();
		marquee_x1 = m.marquee_x1;
		marquee_y1 = m.marquee_y1;
		marquee_x2 = m.marquee_x2;
		marquee_y2 = m.marquee_y2;
		marquee_marked = m.marquee_marked;
		marquee_buffer_filled = m.marquee_buffer_filled;
		marquee_buffer = m.marquee_buffer;
		marquee_all = m.marquee_all;
		marquee_floating = m.marquee_floating;
		marquee_float_x = m.marquee_float_x;
		marquee_float_y = m.marquee_float_y;
		dragging_marquee = m.dragging_marquee;
		marquee_drag_x = m.marquee_drag_x;
		marquee_drag_y = m.marquee_drag_y;
		tiles_bak = m.tiles_bak;
		marquee_curr_x1 = m.marquee_curr_x1;
		marquee_curr_y1 = m.marquee_curr_y1;
		marquee_curr_x2 = m.marquee_curr_x2;
		marquee_curr_y2 = m.marquee_curr_y2;
		marquee_redoes.pop_back();

		tool = tool_redoes.back();
		tool_redoes.pop_back();

		draw_solids = draw_solids_redoes.back();
		draw_solids_redoes.pop_back();
		
		visible = visible_redoes.back();
		visible_redoes.pop_back();
		
		changed = changed_redoes.back();
		changed_redoes.pop_back();
		
#ifdef SUPPORT_GROUPS
		draw_groups = draw_groups_redoes.back();
		draw_groups_redoes.pop_back();
#endif

		layer = layer_redoes.back();
		layer_redoes.pop_back();

		layerCombo->setSelected(layer);

		num_marked = num_marked_redoes.back();
		num_marked_redoes.pop_back();

		if (marquee_buffer_filled) {
			enable_paste_menu_item();
		}
		else {
			disable_paste_menu_item();
		}
		
		if (num_marked == 0) {
			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
		}
		else {
			enable_copy_menu_item();
			enable_copy_all_menu_item();
			enable_cut_menu_item();
			enable_cut_all_menu_item();
			enable_delete_menu_item();
			enable_delete_all_menu_item();
			enable_select_none_menu_item();
		}
		if (num_marked == tiles.size()*tiles[0].size()) {
			disable_select_all_menu_item();
		}
		else {
			enable_select_all_menu_item();
		}

		cols_marked = cols_marked_redoes.back();
		cols_marked_redoes.pop_back();

		rows_marked = rows_marked_redoes.back();
		rows_marked_redoes.pop_back();

		tool = tool_bak;

		enable_undo_menu_item();
		if (redoes.size() == 0) {
			disable_redo_menu_item();
		}
	}
	
	void push_undo(bool clear_redoes = true) {
		// clear redoes

		if (clear_redoes) {
			empty_redoes();
		}

		// push undo

		undoes.push_back(tiles);
		if (undoes.size() > MAX_UNDO) {
			undoes.pop_front();
		}

#ifdef SUPPORT_GROUPS
		group_undoes.push_back(groups);
		if (group_undoes.size() > MAX_UNDO) {
			group_undoes.pop_front();
		}

		wall_undoes.push_back(walls);
		if (wall_undoes.size() > MAX_UNDO) {
			wall_undoes.pop_front();
		}
#endif
		
		Marquee m;
		m.marquee_x1 = marquee_x1;
		m.marquee_y1 = marquee_y1;
		m.marquee_x2 = marquee_x2;
		m.marquee_y2 = marquee_y2;
		m.marquee_marked = marquee_marked;
		m.marquee_buffer_filled = marquee_buffer_filled;
		m.marquee_buffer = marquee_buffer;
		m.marquee_all = marquee_all;
		m.marquee_floating = marquee_floating;
		m.marquee_float_x = marquee_float_x;
		m.marquee_float_y = marquee_float_y;
		m.dragging_marquee = dragging_marquee;
		m.marquee_drag_x = marquee_drag_x;
		m.marquee_drag_y = marquee_drag_y;
		m.tiles_bak = tiles_bak;
		m.marquee_curr_x1 = marquee_curr_x1;
		m.marquee_curr_y1 = marquee_curr_y1;
		m.marquee_curr_x2 = marquee_curr_x2;
		m.marquee_curr_y2 = marquee_curr_y2;
		marquee_undoes.push_back(m);
		if (marquee_undoes.size() > MAX_UNDO) {
			marquee_undoes.pop_front();
		}

		tool_undoes.push_back(tool);
		if (tool_undoes.size() > MAX_UNDO) {
			tool_undoes.pop_front();
		}

		draw_solids_undoes.push_back(draw_solids);
		if (draw_solids_undoes.size() > MAX_UNDO) {
			draw_solids_undoes.pop_front();
		}

		visible_undoes.push_back(visible);
		if (visible_undoes.size() > MAX_UNDO) {
			visible_undoes.pop_front();
		}

		changed_undoes.push_back(changed);
		if (changed_undoes.size() > MAX_UNDO) {
			changed_undoes.pop_front();
		}

#ifdef SUPPORT_GROUPS
		draw_groups_undoes.push_back(draw_groups);
		if (draw_groups_undoes.size() > MAX_UNDO) {
			draw_groups_undoes.pop_front();
		}

		// FIXME!!!: walls
#endif

		layer_undoes.push_back(layer);
		if (layer_undoes.size() > MAX_UNDO) {
			layer_undoes.pop_front();
		}
		
		num_marked_undoes.push_back(num_marked);
		if (num_marked_undoes.size() > MAX_UNDO) {
			num_marked_undoes.pop_front();
		}
		
		cols_marked_undoes.push_back(cols_marked);
		if (cols_marked_undoes.size() > MAX_UNDO) {
			cols_marked_undoes.pop_front();
		}
		
		rows_marked_undoes.push_back(rows_marked);
		if (rows_marked_undoes.size() > MAX_UNDO) {
			rows_marked_undoes.pop_front();
		}
		
		enable_undo_menu_item();
	}

	void push_redo(void) {
		redoes.push_back(tiles);
		if (redoes.size() > MAX_UNDO) {
			redoes.pop_front();
		}

#ifdef SUPPORT_GROUPS
		group_redoes.push_back(groups);
		if (group_redoes.size() > MAX_UNDO) {
			group_redoes.pop_front();
		}

		wall_redoes.push_back(walls);
		if (wall_redoes.size() > MAX_UNDO) {
			wall_redoes.pop_front();
		}
#endif
	
		Marquee m;
		m.marquee_x1 = marquee_x1;
		m.marquee_y1 = marquee_y1;
		m.marquee_x2 = marquee_x2;
		m.marquee_y2 = marquee_y2;
		m.marquee_marked = marquee_marked;
		m.marquee_buffer_filled = marquee_buffer_filled;
		m.marquee_buffer = marquee_buffer;
		m.marquee_all = marquee_all;
		m.marquee_floating = marquee_floating;
		m.marquee_float_x = marquee_float_x;
		m.marquee_float_y = marquee_float_y;
		m.dragging_marquee = dragging_marquee;
		m.marquee_drag_x = marquee_drag_x;
		m.marquee_drag_y = marquee_drag_y;
		m.tiles_bak = tiles_bak;
		m.marquee_curr_x1 = marquee_curr_x1;
		m.marquee_curr_y1 = marquee_curr_y1;
		m.marquee_curr_x2 = marquee_curr_x2;
		m.marquee_curr_y2 = marquee_curr_y2;
		marquee_redoes.push_back(m);
		if (marquee_redoes.size() > MAX_UNDO) {
			marquee_redoes.pop_front();
		}

		tool_redoes.push_back(tool);
		if (tool_redoes.size() > MAX_UNDO) {
			tool_redoes.pop_front();
		}

		draw_solids_redoes.push_back(draw_solids);
		if (draw_solids_redoes.size() > MAX_UNDO) {
			draw_solids_redoes.pop_front();
		}

		visible_redoes.push_back(visible);
		if (visible_redoes.size() > MAX_UNDO) {
			visible_redoes.pop_front();
		}

		changed_redoes.push_back(changed);
		if (changed_redoes.size() > MAX_UNDO) {
			changed_redoes.pop_front();
		}

#ifdef SUPPORT_GROUPS
		draw_groups_redoes.push_back(draw_groups);
		if (draw_groups_redoes.size() > MAX_UNDO) {
			draw_groups_redoes.pop_front();
		}

		// FIXME:: walls!
#endif

		layer_redoes.push_back(layer);
		if (layer_redoes.size() > MAX_UNDO) {
			layer_redoes.pop_front();
		}

		num_marked_redoes.push_back(num_marked);
		if (num_marked_redoes.size() > MAX_UNDO) {
			num_marked_redoes.pop_front();
		}

		cols_marked_redoes.push_back(cols_marked);
		if (cols_marked_redoes.size() > MAX_UNDO) {
			cols_marked_redoes.pop_front();
		}

		rows_marked_redoes.push_back(rows_marked);
		if (rows_marked_redoes.size() > MAX_UNDO) {
			rows_marked_redoes.pop_front();
		}
		
		enable_redo_menu_item();
	}

	void empty_undoes()
	{
		disable_undo_menu_item();

		undoes.clear();
#ifdef SUPPORT_GROUPS
		group_undoes.clear();
		wall_undoes.clear();
#endif
		marquee_undoes.clear();

		tool_undoes.clear();
		draw_solids_undoes.clear();

		visible_undoes.clear();
		
		changed_undoes.clear();

#ifdef SUPPORT_GROUPS
		draw_groups_undoes.clear();
#endif

		layer_undoes.clear();

		num_marked_undoes.clear();

		cols_marked_undoes.clear();
		rows_marked_undoes.clear();
	}

	void empty_redoes()
	{
		disable_redo_menu_item();

		redoes.clear();
#ifdef SUPPORT_GROUPS
		group_redoes.clear();
		wall_redoes.clear();
#endif
		marquee_redoes.clear();

		tool_redoes.clear();
		draw_solids_redoes.clear();

		visible_redoes.clear();
		
		changed_redoes.clear();

#ifdef SUPPORT_GROUPS
		draw_groups_redoes.clear();
#endif

		layer_redoes.clear();

		num_marked_redoes.clear();

		cols_marked_redoes.clear();
		rows_marked_redoes.clear();
	}

	void use_tool(int t, int x, int y, int l, int number, int sheet, int ts_x, int ts_y, int ts_w, int ts_h) {
		bool test_visible_layers = false;
		std::pair<int, int> pr(x, y);

		switch (t) {
			case TOOL_MOVER: {
				if (mover_dest_layer >= -1 && std::find(already_moved.begin(), already_moved.end(), pr) == already_moved.end() && tiles[y][x][mover_src_layer].number != -1 && tiles[y][x][mover_src_layer].sheet != -1) {
					already_moved.push_back(pr);
					tiles[y][x][mover_dest_layer].number = tiles[y][x][mover_src_layer].number;
					tiles[y][x][mover_dest_layer].x = tiles[y][x][mover_src_layer].x;
					tiles[y][x][mover_dest_layer].y = tiles[y][x][mover_src_layer].y;
					tiles[y][x][mover_dest_layer].sheet = tiles[y][x][mover_src_layer].sheet;
					tiles[y][x][mover_src_layer].number = -1;
					tiles[y][x][mover_src_layer].sheet = -1;
					changed = true;
				}
				break;
			}
			case TOOL_CLONE:
			case TOOL_PENCIL: {
				int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
				int th = al_get_bitmap_height(tileSheets[0]) / General::tileSize;
				int tx = number % tw;
				int ty = number / tw;
				if (tx >= 0 && tx < tw && ty >= 0 && ty < th && !blank[ty][tx][sheet]) {
					tiles[y][x][l].number = number;
					tiles[y][x][l].x = tx;
					tiles[y][x][l].y = ty;
					tiles[y][x][l].sheet = sheet;
					changed = true;
				}
				break;
			}
			case TOOL_CLEAR:
				if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
					for (size_t ll = 0; ll < tiles[0][0].size(); ll++) {
						tiles[y][x][ll].number = -1;
						tiles[y][x][ll].x = -1;
						tiles[y][x][ll].y = -1;
						tiles[y][x][ll].sheet = -1;
					}
				}
				else {
					tiles[y][x][l].number = -1;
					tiles[y][x][l].x = -1;
					tiles[y][x][l].y = -1;
					tiles[y][x][l].sheet = -1;
				}
				changed = true;
				break;
			case TOOL_SOLID:
				if ((last_solid_x == -1 && last_solid_y == -1) || (last_solid_x != x || last_solid_y != y)) {
					if (one_solids_layer) {
						tiles[y][x][0].solid = !tiles[y][x][0].solid;
					}
					else {
						tiles[y][x][l].solid = !tiles[y][x][l].solid;
					}
					last_solid_x = x;
					last_solid_y = y;
					changed = true;
				}
				break;
			case TOOL_FILL:
			case TOOL_RMB_FILL:
			{
				if (t == TOOL_RMB_FILL) {
					test_visible_layers = true;
				}

				bool clear;

				if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
					clear = true;
				}
				else {
					clear = false;
				}
			
				int l2;

				if (test_visible_layers) {
					for (l2 = layers-1; l2 >= 0; l2--) {
						if (visible[l2] == false) {
							continue;
						}
						if (tiles[y][x][l2].number >= 0 && tiles[y][x][l2].sheet >= 0) {
							break;
						}
					}

					if (l2 < 0) {
						 l2 = 0;
					}
				}
				else {
					l2 = l;
				}

				int tile_num = tiles[y][x][l2].number;
				int tile_sheet = tiles[y][x][l2].sheet;

				if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
					fill_non_contiguous(x, y, l, tile_num, tile_sheet, test_visible_layers, clear, ts_x, ts_y, ts_w, ts_h);
				}
				else {
					ASH_Point p;
					std::stack<ASH_Point> stack;

					p.x = x;
					p.y = y;
					stack.push(p);

					for (size_t _y = 0; _y < tiles.size(); _y++) {
						for (size_t _x = 0; _x < tiles[0].size(); _x++) {
							tiles[_y][_x][0].flags &= ~TF_USED;
						}
					}

					while (stack.size() > 0) {
						p = stack.top();
						stack.pop();

						fill(x, y, l, p.x, p.y, tile_num, tile_sheet, stack, test_visible_layers, clear, ts_x, ts_y, ts_w, ts_h);
					}
				}
			}
			break;
		}
	}

	void fill_non_contiguous(int x, int y, int layer, int tile_num, int tile_sheet, bool test_visible_layers, bool clear, int ts_x, int ts_y, int ts_w, int ts_h)
	{
		for (int yy = 0; yy < (int)tiles.size(); yy++) {
			for (int xx = 0; xx < (int)tiles[0].size(); xx++) {
				int dx = xx - x;
				int dy = yy - y;
				dx %= ts_w;
				dy %= ts_h;
				if (dx < 0) dx = ts_w + dx;
				if (dy < 0) dy = ts_h + dy;

				int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
				int new_tile;
				int new_sheet;
				if (clear) {
					new_tile = -1;
					new_sheet = -1;
				}
				else {
					new_tile = (ts_x+dx) + ((ts_y+dy)*tw);
					new_sheet = sheet;
				}

				int l;

				if (test_visible_layers) {
					for (l = layers-1; l >= 0; l--) {
						if (visible[l] == false) {
							continue;
						}
						if (tiles[yy][xx][l].number >= 0 && tiles[yy][xx][l].sheet >= 0) {
							break;
						}
					}

					if (l < 0 && tile_num < 0) {
						 l = 0;
					}
				}
				else {
					l = layer;
				}

				if (l >= 0) {
					_Tile &t = tiles[yy][xx][layer];
					_Tile &t2 = tiles[yy][xx][l];

					if (t2.number == tile_num && t2.sheet == tile_sheet) {
						bool go;
						int tx;
						int ty;

						if (clear) {
							go = true;
							tx = -1;
							ty = -1;
						}
						else {
							int th = al_get_bitmap_height(tileSheets[0]) / General::tileSize;

							tx = new_tile % tw;
							ty = new_tile / tw;

							if (tx >= 0 && tx < tw && ty >= 0 && ty < th && !blank[ty][tx][sheet]) {
								go = true;
							}
							else {
								go = false;
							}
						}

						if (go) {
							t.number = new_tile;
							t.x = new_tile % tw;
							t.y = new_tile / tw;
							t.sheet = new_sheet;

							changed = true;
						}
					}
				}
			}
		}
	}

	void doMacro(int xx, int yy) {
		for (unsigned int i = 0; i < macro.size(); i++) {
			_TilePlusPlus t = macro[i];
			t.x += xx;
			t.y += yy;
			if (t.x < 0 || t.x >= (int)tiles[0].size() || t.y < 0 || t.y >= (int)tiles.size()) {
				continue;
			}
			use_tool(t.tool, t.x, t.y, t.layer, t.number, t.sheet, t.ts_x, t.ts_y, t.ts_w, t.ts_h);
		}
	}

	void placeTile(int x, int y, bool rmb) {
		if (x < 0 || y < 0 || x >= (int)tiles[0].size() || y >= (int)tiles.size()) {
			return;
		}
		int t = tool;
		if (t == TOOL_MACRO) {
			doMacro(x, y);
			return;
		}
		if (t == TOOL_FILL && rmb) {
			t = TOOL_RMB_FILL;
		}
		int sel_x, sel_y, sel_w, sel_h;
		ts->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);
		_TilePlusPlus mt;
		mt.x = x;
		mt.y = y;
		mt.layer = layer;
		mt.tool = t;
		mt.ts_x = sel_x;
		mt.ts_y = sel_y;
		mt.ts_w = sel_w;
		mt.ts_h = sel_h;
		use_tool(t, x, y, layer, number, sheet, sel_x, sel_y, sel_w, sel_h);
		if (t == TOOL_CLONE || t == TOOL_PENCIL) {
			mt.number = number;
			mt.sheet = sheet;
		}
		if (recording) {
			if (macroStartX == -1) {
				macroStartX = x;
				macroStartY = y;
			}
			mt.x -= macroStartX;
			mt.y -= macroStartY;
			macro.push_back(mt);
		}
	}

	void setLayer(int l) {
		if (l < 0) {
			l = 0;
		}
		if (l >= layers) {
			l = layers-1;
		}
		if (l != layer)
			already_moved.clear();
		layer = l;
	}

	int getLayer(void) {
		return layer;
	}

	void setTile(int n) {
		number = n;
	}

	void setSheet(int s) {
		sheet = s;
	}

	bool getTile(int xx, int yy, int l, int *number, int *sheet, bool *solid, bool *tint) {
		_Tile t;
		if (marquee_floating && (l == layer || marquee_all) && (xx >= marquee_float_x && xx < (marquee_float_x+(int)marquee_buffer[0].size()) && yy >= marquee_float_y && yy < (marquee_float_y+(int)marquee_buffer.size()))) {
			int mx = xx-marquee_float_x;
			int my = yy-marquee_float_y;
			int layer = marquee_all ? l : 0;
			if (layer < 0 || layer >= (int)marquee_buffer[my][mx].size()) {
				if (l >= 0 && l < (int)tiles[yy][xx].size()) {
					t = tiles[yy][xx][l];
				}
			}
			else {
				t = marquee_buffer[my][mx][layer];
				if (t.number < 0) {
					bool solid = t.solid;
					t = tiles[yy][xx][l];
					t.solid = solid;
				}
			}
		}
		else {
			t = tiles[yy][xx][l];
		}
		if (number)
			*number = t.number;
		if (sheet)
			*sheet = t.sheet;
		if (solid)
			*solid = t.solid;
		std::pair<int, int> pr(xx, yy);
		if (tool == TOOL_MOVER && l == mover_dest_layer &&
			std::find(already_moved.begin(), already_moved.end(),
			pr) != already_moved.end())
			*tint = true;
		else
			*tint = false;
		return true;
	}

	int getWidth(void) {
		return tiles[0].size();
	}

	int getHeight(void) {
		return tiles.size();
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (rel_x >= 0) {
			statusX = rel_x / General::tileSize / General::scale;
			statusY = rel_y / General::tileSize / General::scale;
			
			if (statusX < 0 || statusY < 0 || statusX >= (int)tiles[0].size() || statusY >= (int)tiles.size()) {
				return;
			}

			if (down && (tool == TOOL_MARQUEE || tool == TOOL_MAGIC_WAND)) {
				if (dragging_marquee) {
					int dx = statusX - marquee_drag_x;
					int dy = statusY - marquee_drag_y;
					marquee_drag_x = statusX;
					marquee_drag_y = statusY;
					marquee_float_x += dx;
					marquee_float_y += dy;
				}
				else if (tool == TOOL_MARQUEE) {
					int tmp_curr_x2 = marquee_curr_x2;
					int tmp_curr_y2 = marquee_curr_y2;
					marquee_curr_x2 = statusX;
					marquee_curr_y2 = statusY;

					if (tmp_curr_x2 != marquee_curr_x2 || tmp_curr_y2 != marquee_curr_y2) {
						int dx;
						int dy;
						int sx;
						int sy;
						int ex;
						int ey;

						if (marquee_curr_x1 < tmp_curr_x2) {
							dx = 1;
							sx = marquee_curr_x1;
							ex = tmp_curr_x2;
						}
						else if (marquee_curr_x1 > tmp_curr_x2) {
							dx = 1;
							sx = tmp_curr_x2;
							ex = marquee_curr_x1;
						}
						else {
							dx = 1;
							sx = marquee_curr_x1;
							ex = marquee_curr_x1;
						}

						if (marquee_curr_y1 < tmp_curr_y2) {
							dy = 1;
							sy = marquee_curr_y1;
							ey = tmp_curr_y2;
						}
						else if (marquee_curr_y1 > tmp_curr_y2) {
							dy = 1;
							sy = tmp_curr_y2;
							ey = marquee_curr_y1;
						}
						else {
							dy = 1;
							sy = marquee_curr_y1;
							ey = marquee_curr_y1;
						}

						for (int y = sy; y <= ey; y += dy) {
							for (int x = sx; x <= ex; x += dx) {
								std::pair<int, int> p(x, y);
								if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
									if (tiles_bak[p.second][p.first][0].flags & TF_MARKED) {
										sel(p.first, p.second);
									}
								}
								else {
									if ((tiles[p.second][p.first][0].flags & TF_MARKED) && (tiles_bak[p.second][p.first][0].flags & TF_MARKED) == 0) {
										unsel(p.first, p.second);
									}
								}
							}
						}

						if (marquee_curr_x1 < marquee_curr_x2) {
							dx = 1;
							sx = marquee_curr_x1;
							ex = marquee_curr_x2;
						}
						else if (marquee_curr_x1 > marquee_curr_x2) {
							dx = 1;
							sx = marquee_curr_x2;
							ex = marquee_curr_x1;
						}
						else {
							dx = 1;
							sx = marquee_curr_x1;
							ex = marquee_curr_x1;
						}

						if (marquee_curr_y1 < marquee_curr_y2) {
							dy = 1;
							sy = marquee_curr_y1;
							ey = marquee_curr_y2;
						}
						else if (marquee_curr_y1 > marquee_curr_y2) {
							dy = 1;
							sy = marquee_curr_y2;
							ey = marquee_curr_y1;
						}
						else {
							dy = 1;
							sy = marquee_curr_y1;
							ey = marquee_curr_y1;
						}

						for (int y = sy; y <= ey; y += dy) {
							for (int x = sx; x <= ex; x += dx) {
								std::pair<int, int> p(x, y);
								if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
									unsel(p.first, p.second);
								}
								else {
									sel(p.first, p.second);
								}
								sel_changed = true;
							}
						}

						if (num_marked <= 0) {
							marquee_x1 = INT_MAX;
							marquee_y1 = INT_MAX;
							marquee_x2 = INT_MIN;
							marquee_y2 = INT_MIN;
						}
						else {
							int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;

							for (int x = 0; x < (int)tiles[0].size(); x++) {
								if (cols_marked[x] > 0) {
									if (x < minx) {
										minx = x;
									}
									if (x > maxx) {
										maxx = x;
									}
								}
							}

							for (int y = 0; y < (int)tiles.size(); y++) {
								if (rows_marked[y] > 0) {
									if (y < miny) {
										miny = y;
									}
									if (y > maxy) {
										maxy = y;
									}
								}
							}

							marquee_x1 = minx;
							marquee_y1 = miny;
							marquee_x2 = maxx;
							marquee_y2 = maxy;
						}
					}
				}
			}
			else {
				if (down) {
					int sel_x, sel_y, sel_w, sel_h;
					ts->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);

					if (tool == TOOL_PENCIL && (sel_w > 1 || sel_h > 1)) {
						if (statusX != last_big_pencil_x || statusY != last_big_pencil_y) {
							last_big_pencil_x = statusX;
							last_big_pencil_y = statusY;
							do_big_pencil(statusX, statusY);
						}
					}
					else if (statusX >= 0 && tool == TOOL_CLONE) {
						int tx = cloneTileX + (statusX - cloneStartX);
						int ty = cloneTileY + (statusY - cloneStartY);
						int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
						int th = al_get_bitmap_height(tileSheets[0]) / General::tileSize;
						tx %= tw;
						ty %= th;
						ts->setSelected(tx, ty, 1, 1);
						number = tx + ty*tw;
					}
					placeTile(statusX, statusY, is_rmb);
				}
				A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
				if (scrollpane) {
					if (statusX*General::tileSize >= scrollpane->getSizeX()) {
						statusX = -1;
					}
					if (statusY*General::tileSize >= scrollpane->getSizeY()) {
						statusY = -1;
					}
				}
			}
		}
		else {
			statusX = -1;
			statusY = -1;
		}
	}

	void do_big_pencil(int xx, int yy)
	{
		int sel_x, sel_y, sel_w, sel_h;
		ts->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);

		for (int _y = 0; _y < sel_h; _y++) {
			for (int _x = 0; _x < sel_w; _x++) {
				if ((xx+_x) < 0 || (xx+_x) >= (int)tiles[0].size() || (yy+_y) < 0 || (yy+_y) >= (int)tiles.size()) {
					continue;
				}
				int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
				int number = (sel_x+_x) + ((sel_y+_y)*tw);
				int sel_x, sel_y, sel_w, sel_h;
				ts->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);
				_TilePlusPlus mt;
				mt.x = xx+_x;
				mt.y = yy+_y;
				mt.layer = layer;
				mt.tool = tool;
				mt.ts_x = sel_x;
				mt.ts_y = sel_y;
				mt.ts_w = sel_w;
				mt.ts_h = sel_h;
				use_tool(tool, xx+_x, yy+_y, layer, number, sheet, sel_x, sel_y, sel_w, sel_h);
				mt.number = number;
				mt.sheet = sheet;
				if (recording) {
					if (macroStartX == -1) {
						macroStartX = mt.x;
						macroStartY = mt.y;
					}
					mt.x -= macroStartX;
					mt.y -= macroStartY;
					macro.push_back(mt);
				}
			}
		}
		down = true;
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		is_rmb = mb == 2;

		if (tileSheets.size() == 0) {
			return;
		}

		if (mb != 1 && mb != 2) return;

		bool test_visible_layers = false;

		if (mb == 2) {
			if (tool != TOOL_MAGIC_WAND) {
				if (tool != TOOL_FILL && tool != TOOL_PENCIL) {
					return;
				}
			}
			else {
				test_visible_layers = true;
			}
		}

		if (rel_x >= 0) {
			int xx = rel_x / General::tileSize / General::scale;
			int yy = rel_y / General::tileSize / General::scale;

			bool holding_shift = false;
			bool holding_alt = false;
				
			if (tgui::isKeyDown(ALLEGRO_KEY_LSHIFT) || tgui::isKeyDown(ALLEGRO_KEY_RSHIFT)) {
				holding_shift = true;
			}
			if (tgui::isKeyDown(ALLEGRO_KEY_ALT) || tgui::isKeyDown(ALLEGRO_KEY_ALTGR)) {
				holding_alt = true;
			}

			if (tool == TOOL_MARQUEE) {
				if (xx < 0 || yy < 0 || xx >= (int)tiles[0].size() || yy >= (int)tiles.size()) {
					return;
				}

				if (marquee_floating && !(holding_shift || holding_alt)) {
					int bx = xx - marquee_float_x;
					int by = yy - marquee_float_y;
					if (bx >= 0 && by >= 0 && bx < (int)marquee_buffer[0].size() && by < (int)marquee_buffer.size() && marquee_buffer[by][bx].size() > 0) {
						push_undo();
						dragging_marquee = true;
						marquee_drag_x = xx;
						marquee_drag_y = yy;
					}
					else {
						anchor_marquee();
					}
				}

				if (dragging_marquee == false) {
					if ((marquee_marked || marquee_floating) && holding_shift) {
						if (marquee_floating) {
							marquee_float_to_selection();
						}
						else {
							push_undo();
						}
						tiles_bak = tiles;
						sel(xx, yy);

						if (xx < marquee_x1) {
							marquee_x1 = xx;
						}
						if (xx > marquee_x2) {
							marquee_x2 = xx;
						}
						if (yy < marquee_y1) {
							marquee_y1 = yy;
						}
						if (yy > marquee_y2) {
							marquee_y2 = yy;
						}
					}
					else if ((marquee_marked || marquee_floating) && holding_alt) {
						if (marquee_floating) {
							marquee_float_to_selection();
						}
						else {
							push_undo();
						}
						tiles_bak = tiles;
						unsel(xx, yy);

						unmark(std::pair<int, int>(xx, yy));
					}
					else {
						push_undo();

						A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
						marquee_copy_cut_scroll_x = scrollpane->getOffsetX();
						marquee_copy_cut_scroll_y = scrollpane->getOffsetY();

						marquee_x1 = xx;
						marquee_y1 = yy;
						marquee_x2 = xx;
						marquee_y2 = yy;

						selectNone();

						tiles_bak = tiles;
						sel(xx, yy);
					}
					marquee_curr_x1 = xx;
					marquee_curr_y1 = yy;
					marquee_curr_x2 = xx;
					marquee_curr_y2 = yy;
				}
				down = true;
				marquee_marked = true;
			}
			else if (tool == TOOL_MAGIC_WAND) {
				if (xx < 0 || yy < 0 || xx >= (int)tiles[0].size() || yy >= (int)tiles.size()) {
					return;
				}

				if (marquee_floating && !(holding_shift || holding_alt)) {
					int bx = xx - marquee_float_x;
					int by = yy - marquee_float_y;
					if (bx >= 0 && by >= 0 && bx < (int)marquee_buffer[0].size() && by < (int)marquee_buffer.size() && marquee_buffer[by][bx].size() > 0) {
						push_undo();
						dragging_marquee = true;
						marquee_drag_x = xx;
						marquee_drag_y = yy;
					}
					else {
						anchor_marquee();
					}
				}

				if (dragging_marquee == false) {
					bool subtract = false;
					bool add = false;

					if ((marquee_marked || marquee_floating) && holding_shift) {
						add = true;
						if (marquee_floating) {
							marquee_float_to_selection();
						}
						else {
							push_undo();
						}
					}
					else if ((marquee_marked || marquee_floating) && holding_alt) {
						if (marquee_floating) {
							marquee_float_to_selection();
						}
						else {
							push_undo();
						}
						subtract = true;
					}
					else {
						push_undo();
						A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
						marquee_copy_cut_scroll_x = scrollpane->getOffsetX();
						marquee_copy_cut_scroll_y = scrollpane->getOffsetY();

						selectNone();
					}

					int l2;

					if (test_visible_layers) {
						for (l2 = layers-1; l2 >= 0; l2--) {
							if (visible[l2] == false) {
								continue;
							}
							if (tiles[yy][xx][l2].number >= 0 && tiles[yy][xx][l2].sheet >= 0) {
								break;
							}
						}

						if (l2 < 0) {
							 l2 = 0;
						}
					}
					else {
						l2 = layer;
					}

					int tile_num = tiles[yy][xx][l2].number;
					int tile_sheet = tiles[yy][xx][l2].sheet;

					if (tile_sheet < 0) {
						tile_num = -1;
					}

					if (tgui::isKeyDown(ALLEGRO_KEY_LCTRL) || tgui::isKeyDown(ALLEGRO_KEY_RCTRL)) {
						magic_non_contiguous(xx, yy, layer, tile_num, tile_sheet, test_visible_layers, subtract);
					}
					else {
						ASH_Point p;
						p.x = xx;
						p.y = yy;

						std::stack<ASH_Point> stack;
						stack.push(p);

						if (add) {
							if (xx < marquee_x1) {
								marquee_x1 = xx;
							}
							if (xx > marquee_x2) {
								marquee_x2 = xx;
							}
							if (yy < marquee_y1) {
								marquee_y1 = yy;
							}
							if (yy > marquee_y2) {
								marquee_y2 = yy;
							}
						}
						else if (subtract) {
							std::pair<int, int> pt(xx, yy);
							unmark(pt);
						}
						else {
							marquee_x1 = xx;
							marquee_y1 = yy;
							marquee_x2 = xx;
							marquee_y2 = yy;
						}

						while (stack.size() > 0) {
							p = stack.top();
							stack.pop();

							magic(xx, yy, layer, p.x, p.y, tile_num, tile_sheet, stack, test_visible_layers, subtract);
						}
					}
				}

				down = true;

				marquee_marked = true;
			}
			else {
				if (tool == TOOL_PENCIL && is_rmb) {
					for (int l = layers-1; l >= 0; l--) {
						_Tile t = tiles[yy][xx][l];
						if (visible[l] && t.number >= 0) {
							ts->setSelected(t.x, t.y, 1, 1);
							break;
						}
					}
				}
				else {
					push_undo();

					if (tool == TOOL_CLONE) {
						cloneStartX = statusX;
						cloneStartY = statusY;
						ts->getSelected(&cloneTileX, &cloneTileY, NULL, NULL);
					}

					if (tool == TOOL_SOLID) {
						last_solid_x = -1;
						last_solid_y = -1;
					}

					int sel_x, sel_y, sel_w, sel_h;
					ts->getSelected(&sel_x, &sel_y, &sel_w, &sel_h);

					if (tool == TOOL_PENCIL && (sel_w > 1 || sel_h > 1)) {
						last_big_pencil_x = xx;
						last_big_pencil_y = yy;
						do_big_pencil(xx, yy);
					}
					else {
						placeTile(xx, yy, is_rmb);
						down = true;
					}
				}
			}
		}
	}
	
	void magic_non_contiguous(int x, int y, int layer, int tile_num, int tile_sheet, bool test_visible_layers, bool subtract)
	{
		for (int yy = 0; yy < (int)tiles.size(); yy++) {
			for (int xx = 0; xx < (int)tiles[0].size(); xx++) {
				int l;

				if (test_visible_layers) {
					for (l = layers-1; l >= 0; l--) {
						if (visible[l] == false) {
							continue;
						}
						if (tiles[yy][xx][l].number >= 0 && tiles[yy][xx][l].sheet >= 0) {
							break;
						}
					}

					if (l < 0 && tile_num < 0) {
						l = 0;
					}
				}
				else {
					l = layer;
				}

				if (l < 0 || tiles[yy][xx][l].number != tile_num || tiles[yy][xx][l].sheet != tile_sheet) {
					continue;
				}

				std::pair<int, int> p(xx, yy);

				add_sub_magic(p, subtract);
			}
		}
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (down) {
			empty_redoes();
		}

		down = false;
      
		dragging_marquee = false;

		cloneStartX = -1;
	}

	void size(int w, int h) {
		tiles.clear();
		visible.clear();
		rows_marked.clear();
		cols_marked.clear();

		clear_undoes_redoes();

		num_marked = 0;

		for (int i = 0; i < h; i++) {
			std::vector< std::vector<_Tile> > row = createRow(w);
			tiles.push_back(row);
			rows_marked.push_back(0);
		}
		for (int j = 0; j < w; j++) {
			cols_marked.push_back(0);
		}

		for (int i = 0; i < layers; i++) {
			visible.push_back(true);
		}
	}

	std::string getLoadSavePath(bool load) {
		ALLEGRO_FILECHOOSER *diag;
		diag = al_create_native_file_dialog(
			al_path_cstr(loadSavePath, '/'),
			load ? "Load Area" : "Save Area",
			"*.*",
			load ? ALLEGRO_FILECHOOSER_FILE_MUST_EXIST : ALLEGRO_FILECHOOSER_SAVE
		);

		al_show_native_file_dialog(display, diag);

		tgui::clearKeyState();

		if (al_get_native_file_dialog_count(diag) != 1)
			return "";

		const ALLEGRO_PATH *result = al_create_path(al_get_native_file_dialog_path(diag, 0));
		al_destroy_path(loadSavePath);
		loadSavePath = al_clone_path(result);
		al_set_path_filename(loadSavePath, NULL);

		std::string path = al_path_cstr(result, ALLEGRO_NATIVE_PATH_SEP);

		al_destroy_native_file_dialog(diag);

		return path;
	}

	#define LOAD_ERROR(a) { load_error(a); return false; }
	void load_error(std::string message, std::string msg2 = "") {
		new_level();
		al_show_native_message_box(display, "Load Error", message.c_str(), msg2.c_str(), 0, 0);
	}

	LEVEL_TYPE get_level_type()
	{
		return level_type;
	}

	LEVEL_TYPE get_level_type(std::string filename)
	{
		LEVEL_TYPE ltype = WM2;

		if (use_filename_based_level_types) {
			std::string ext;
			int dot = filename.rfind('.');
			if (dot != std::string::npos && dot != filename.length()-1) {
				ext = filename.substr(dot+1);
			}
			int slash = filename.rfind('/');
			std::string filepart;
			if (slash == std::string::npos) {
				slash = filename.rfind('\\');
			}
			if (slash == std::string::npos) {
				filepart = filename;
			}
			else if (slash != filename.length()-1) {
				filepart = filename.substr(slash+1);
			}
			ext[0] = tolower(ext[0]);
			ext[1] = tolower(ext[1]);
			ext[2] = tolower(ext[2]);
			if (filepart == "area") {
				ltype = CP;
				one_solids_layer = false;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
			else if (ext == "ash") {
				ltype = WM2;
				one_solids_layer = true;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
			else if (ext == "area") {
				ltype = MO2;
				one_solids_layer = true;
				General::can_add_and_delete_layers = false;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
			else if (ext == "map") {
				ltype = MO3;
				one_solids_layer = true;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 12;
				}
			}
		}
		else {
			if (command_line_level_type == CP) {
				ltype = CP;
				one_solids_layer = false;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
			else if (command_line_level_type == MO2) {
				ltype = MO2;
				one_solids_layer = true;
				General::can_add_and_delete_layers = false;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
			else if (command_line_level_type == MO3) {
				ltype = MO3;
				one_solids_layer = true;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 12;
				}
			}
			else {
				ltype = WM2;
				one_solids_layer = true;
				General::can_add_and_delete_layers = true;
				if (my_tile_size < 0) {
					General::tileSize = 16;
				}
			}
		}

		return ltype;
	}

	bool load(std::string filename) {
#ifdef SUPPORT_GROUPS
		groups.clear();
		walls.clear();
#endif
		const char *cFilename = filename.c_str();

		level_type = get_level_type(filename);

		ALLEGRO_FILE *f = al_fopen(cFilename, "rb");

		if (f == NULL) LOAD_ERROR("Error opening file!")

		if (level_type == WM2) {
			int b1 = al_fgetc(f);
			int b2 = al_fgetc(f);
			int b3 = al_fgetc(f);
			int b4 = al_fgetc(f);

			if (b1 != 'A' || b2 != 'S' || b3 != 'H' || b4 != '!') {
				LOAD_ERROR("Not an AshEdit map!")
			}

			int w = al_fread16le(f);
			int h = al_fread16le(f);

			if (w <= 0 || h <= 0) {
				LOAD_ERROR("Invalid width/height in map!")
			}

			layers = al_fgetc(f);

			if (layers == EOF || layers <= 0 || layers >= 4096) {
				LOAD_ERROR("Invalid number of layers!")
			}

			size(w, h);

			resizeScrollpane();

			// Read solids
			for (unsigned int y = 0; y < tiles.size(); y++) {
				for (unsigned int x = 0; x < tiles[0].size(); x++) {
					_Tile &t = tiles[y][x][0];
					int b = al_fgetc(f);
					if (b == EOF) {
						LOAD_ERROR("Premature end of file!")
					}
					t.solid = ((unsigned char)b) == 1 ? true : false;
				}
			}

			// for each layer
			for (int l = 0; l < layers; l++) {
				// read each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile &t = tiles[y][x][l];
						int b = al_fgetc(f);
						if (b == EOF) {
							LOAD_ERROR("Premature end of file!")
						}
						int flags = (unsigned char)b;
						if (flags & 1) {
							int b;
							b = al_fgetc(f);
							if (b == EOF) {
								LOAD_ERROR("Premature end of file!")
							}
							t.x = (unsigned char)b;
							b = al_fgetc(f);
							if (b == EOF) {
								LOAD_ERROR("Premature end of file!")
							}
							t.y = (unsigned char)b;
							if (tileSheets.size() > 0) {
								int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
								t.number = t.x + t.y * tw;
							}
							else {
								t.number = -777;
							}
							b = al_fgetc(f);
							if (b == EOF) {
								LOAD_ERROR("Premature end of file!")
							}
							t.sheet = (unsigned char)b;
						}
						else {
							t.number = -1;
							t.sheet = -1;
						}
					}
				}
			}

		}
		else if (level_type == MO2) {
			int w = al_fread32le(f);
			if ((w == EOF && (al_feof(f) || al_ferror(f))) || w >= 65536 || w <= 0) {
				LOAD_ERROR("Invalid width/height in map!")
			}
			int h = al_fread32le(f);
			if ((h == EOF && (al_feof(f) || al_ferror(f))) || h >= 65536 || h <= 0) {
				LOAD_ERROR("Invalid width/height in map!")
			}

			layers = General::startLayers;

			size(w, h);

			resizeScrollpane();

			int nTileTypes = al_fread32le(f);

			if ((nTileTypes == EOF && (al_feof(f) || al_ferror(f))) || nTileTypes >= 256) {
				LOAD_ERROR("Invalid nTileTypes!")
			}

			std::vector<int> tileTypes;

			for (int i = 0; i < nTileTypes; i++) {
				int tt = al_fread32le(f);
				if ((tt == EOF && (al_feof(f) || al_ferror(f))) || tt >= 4096) {
					LOAD_ERROR("Invalid tile type!")
				}
				tileTypes.push_back(tt);
			}
			
			int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;

			for (unsigned int y = 0; y < tiles.size(); y++) {
				for (unsigned int x = 0; x < tiles[0].size(); x++) {
					for (int l = 0; l < General::startLayers; l++) {
						_Tile &t = tiles[y][x][l];
						int n = al_fread32le(f);
						if (n == EOF && (al_feof(f) || al_ferror(f))) {
							LOAD_ERROR("Invalid tile in map!")
						}
						t.number = n < 0 ? -1 : tileTypes[n];
						t.sheet = n < 0 ? -1 : 0;
						t.x = t.number % tw;
						t.y = t.number / tw;
					}
					int b;
					b = al_fgetc(f);
					if (b == EOF) {
						LOAD_ERROR("Invalid solid byte in map!")
					}
					bool solid = b;
					tiles[y][x][0].solid = solid;
				}
			}
		}
		else if (level_type == MO3) {
			int w = al_fread16le(f);
			int h = al_fread16le(f);

			if (w == 20292 && h == 20295) {
				LOAD_ERROR("Can't load!")
			}

			if (w <= 0 || h <= 0) {
				LOAD_ERROR("Invalid width/height in map!")
			}

			layers = al_fgetc(f);

			if (layers == EOF || layers <= 0) {
				LOAD_ERROR("Invalid number of layers!")
			}

			size(w, h);

			resizeScrollpane();

			// for each layer
			for (int l = 0; l < layers; l++) {
				// read each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile &t = tiles[y][x][l];
						int b;
						b = al_fgetc(f);
						if (b == EOF) {
							LOAD_ERROR("Invalid tile x pos!")
						}
						t.x = (unsigned char)b;
						b = al_fgetc(f);
						if (b == EOF) {
							LOAD_ERROR("Invalid tile x pos!")
						}
						unsigned char y = (unsigned char)b;
						if (l == 0) {
							t.solid = (y & 0x80) ? true : false;
							y = y & 0x7f;
						}
						else {
							t.solid = false;
						}
						t.y = y;
						if (t.x == 255) {
							t.number = -1;
							t.sheet = -1;
						}
						else {
							t.sheet = 0;
							if (tileSheets.size() > 0) {
								int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
								t.number = t.x + t.y * tw;
							}
							else {
								t.number = -777;
							}
						}
					}
				}
			}

#ifdef SUPPORT_GROUPS
			int num_groups = al_fread16le(f);

			if (num_groups < 0) {
				LOAD_ERROR("Invalid number of groups!")
			}

			for (int i = 0; i < num_groups; i++) {
				int b;
				b = al_fgetc(f);
				if (b == EOF) {
					LOAD_ERROR("Error in groups!")
				}
				int t = b;
				b = al_fgetc(f);
				if (b == EOF) {
					LOAD_ERROR("Error in groups!")
				}
				int l = b;
				int x = al_fread16le(f);
				if (x < 0) {
					LOAD_ERROR("Error in groups!")
				}
				int y = al_fread16le(f);
				if (y < 0) {
					LOAD_ERROR("Error in groups!")
				}
				int w = al_fread16le(f);
				if (w < 0) {
					LOAD_ERROR("Error in groups!")
				}
				int h = al_fread16le(f);
				if (h < 0) {
					LOAD_ERROR("Error in groups!")
				}
				Group g = { t, l, x, y, w, h };
				groups.push_back(g);
			}

#endif
		}
		else { // CP
			int w = al_fread32le(f);
			if ((w == EOF && (al_feof(f) || al_ferror(f))) || w >= 65536 || w <= 0) {
				LOAD_ERROR("Invalid width in map!")
			}
			int h = al_fread32le(f);
			if ((h == EOF && (al_feof(f) || al_ferror(f))) || h >= 65536 || h <= 0) {
				LOAD_ERROR("Invalid height in map!")
			}

			layers = al_fread32le(f);

			if (layers == EOF && (al_feof(f) || al_ferror(f)) || layers >= 4096 || layers <= 0) {
				LOAD_ERROR("Invalid number of layers in map!")
			}

			size(w, h);

			resizeScrollpane();
			
			int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;

			// for each layer
			for (int l = 0; l < layers; l++) {
				// read each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile &t = tiles[y][x][l];
						t.number = al_fread32le(f);
						if (t.number == EOF && (al_feof(f) || al_ferror(f))) {
							LOAD_ERROR("Invalid tile in map!")
						}
						t.x = t.number % tw;
						t.y = t.number / tw;
						int b;
						b = al_fgetc(f);
						if (b == EOF) {
							LOAD_ERROR("Invalid sheet in map!")
						}
						t.sheet = b;
						b = al_fgetc(f);
						if (b == EOF) {
							LOAD_ERROR("Invalid solid in map!")
						}
						t.solid = b;
					}
				}
			}
		}

		al_fclose(f);

#ifdef DUMP
		if (dump) {
			level_type = dump_type;
			std::string fake;
			if (level_type == WM2) {
				fake = "foo.ash";
			}
			else if (level_type == MO3) {
				fake = "foo.map";
			}
			else if (level_type == MO2) {
				fake = "foo.area";
			}
			else if (level_type == CP) {
				fake = "area";
			}
			get_level_type(fake); // set some stuff
			if (save(filename) == false) {
				al_show_native_message_box(display, "Save Error", "Invalid input!", "File could not be saved at this location!", 0, 0);
			}
			exit(0);
		}
#endif

		clear_undoes_redoes();
		
		marquee_buffer_filled = false;
		marquee_marked = false;
		marquee_floating = false;

		disable_copy_menu_item();
		disable_copy_all_menu_item();
		disable_cut_menu_item();
		disable_cut_all_menu_item();
		disable_delete_menu_item();
		disable_delete_all_menu_item();
		disable_select_none_menu_item();
		enable_select_all_menu_item();

		changed = false;

		add_to_recent(filename, true);
		
		layer = 0;

		return true;
	}

	void setLastSaveName(std::string name)
	{
		lastSaveName = name;
	}

	bool save(std::string filename, LEVEL_TYPE lt = NONE) {
		changed = false;

		if (lt == NONE) {
			LEVEL_TYPE old_type = level_type;
			level_type = get_level_type(filename);
			if (old_type != NONE && old_type != level_type) {
				int ret = al_show_native_message_box(display, "Warning", "Changing the level type can cause information loss. We recommend backing up this level before proceeding.", "Proceed anyway?", 0, ALLEGRO_MESSAGEBOX_YES_NO);
				if (ret != 1) {
					level_type = old_type;
					lastSaveName = lastSaveNameBak;
					operatingFilename = operatingFilenameBak;
					return false;
				}
			}
			if (level_type == CP || level_type == MO3) {
				for (int i = 0; i < layers; i++) {
					draw_solids[i] = true;
				}
			}
		}

		if (level_type == WM2) {
			const char *cFilename = filename.c_str();

			ALLEGRO_FILE *f = al_fopen(cFilename, "wb");

			if (f == nullptr) {
				return false;
			}

			al_fputc(f, 'A');
			al_fputc(f, 'S');
			al_fputc(f, 'H');
			al_fputc(f, '!');

			al_fwrite16le(f, (int16_t)tiles[0].size()); // width
			al_fwrite16le(f, (int16_t)tiles.size()); // height
			al_fputc(f, layers);

			// write solids
			for (unsigned int y = 0; y < tiles.size(); y++) {
				for (unsigned int x = 0; x < tiles[0].size(); x++) {
					bool is_solid = false;
					for (size_t i = 0; i < tiles[y][x].size(); i++) {
						if (tiles[y][x][i].solid) {
							is_solid = true;
							break;
						}
					}
					al_fputc(f, is_solid ? 1 : 0);
				}
			}

			// for each layer
			for (int l = 0; l < layers; l++) {
				// write each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile t = tiles[y][x][l];
						int flags;
						int tx, ty;

						if (t.sheet < 0 || t.number < 0) {
							flags = 0;
						}
						else {
							flags = 1;
						}

						al_fputc(f, flags);

						if (flags & 1) {
							int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
							tx = t.number % tw;
							ty = t.number / tw;

							al_fputc(f, tx);
							al_fputc(f, ty);
							al_fputc(f, t.sheet);
						}
					}
				}
			}

			al_fclose(f);
		}
		else if (level_type == MO2) {
			ALLEGRO_FILE *f = al_fopen(filename.c_str(), "wb");

			if (f == nullptr) {
				return false;
			}

			al_fwrite32le(f, tiles[0].size()); // width
			al_fwrite32le(f, tiles.size()); // height

			std::vector<int> used_tiles;

			for (int l = 0; l < layers; l++) {
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile t = tiles[y][x][l];
						if (t.number < 0) continue;
						if (std::find(used_tiles.begin(), used_tiles.end(), t.number) == used_tiles.end()) {
							used_tiles.push_back(t.number);
						}
					}
				}
			}

			al_fwrite32le(f, used_tiles.size());

			for (size_t i = 0; i < used_tiles.size(); i++) {
				al_fwrite32le(f, used_tiles[i]);
			}

			for (unsigned int y = 0; y < tiles.size(); y++) {
				for (unsigned int x = 0; x < tiles[0].size(); x++) {
					bool solid = false;
					for (int l = 0; l < General::startLayers; l++) {
						_Tile t = tiles[y][x][l];
						int idx = 0;
						for (; idx < (int)used_tiles.size(); idx++) {
							if (t.number == used_tiles[idx]) {
								break;
							}
						}
						al_fwrite32le(f, t.number < 0 ? -1 : idx);
						if (l == 0 && t.solid) {
							solid = true;
						}
					}
					al_fputc(f, solid);
				}
			}

			al_fclose(f);
		}
		else if (level_type == MO3) {
			const char *cFilename = filename.c_str();

			ALLEGRO_FILE *f = al_fopen(cFilename, "wb");

			if (f == nullptr) {
				return false;
			}

			al_fwrite16le(f, (int16_t)tiles[0].size()); // width
			al_fwrite16le(f, (int16_t)tiles.size()); // height
			al_fputc(f, layers);

			// for each layer
			for (int l = 0; l < layers; l++) {
				// write each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile t = tiles[y][x][l];
						int tx, ty;
						if (t.sheet < 0 || t.number < 0) {
							tx = 255;
							ty = 0;
						}
						else {
							int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
							tx = t.number % tw;
							ty = t.number / tw;
						}

						if (l == 0) {
							bool solid = false;
							for (int l2 = 0; l2 < layers; l2++) {
								if (tiles[y][x][l2].solid) {
									solid = true;
									break;
								}
							}
							if (solid) {
								ty |= 0x80;
							}
						}

						al_fputc(f, tx);
						al_fputc(f, ty);
					}
				}
			}

#ifdef SUPPORT_GROUPS
			al_fwrite16le(f, (int16_t)groups.size());

			for (size_t i = 0; i < groups.size(); i++) {
				Group &g = groups[i];
				al_fputc(f, g.type);
				al_fputc(f, g.layer);
				al_fwrite16le(f, (int16_t)g.x);
				al_fwrite16le(f, (int16_t)g.y);
				al_fwrite16le(f, (int16_t)g.w);
				al_fwrite16le(f, (int16_t)g.h);
			}

			/*
			al_fwrite16le(f, (int16_t)walls.size());

			for (size_t i = 0; i < walls.size(); i++) {
				Wall &w = walls[i];
				al_fwrite16le(f, (int16_t)w.x);
				al_fwrite16le(f, (int16_t)w.y);
				al_fwrite16le(f, (int16_t)w.z);
				al_fwrite16le(f, (int16_t)w.size_x);
				al_fwrite16le(f, (int16_t)w.size_y);
				al_fwrite16le(f, (int16_t)w.size_z);
			}
			*/
#endif

			al_fclose(f);
		}
		else { // CP
			const char *cFilename = filename.c_str();

			ALLEGRO_FILE *f = al_fopen(cFilename, "wb");

			if (f == nullptr) {
				return false;
			}

			al_fwrite32le(f, tiles[0].size()); // width
			al_fwrite32le(f, tiles.size()); // height
			al_fwrite32le(f, layers);

			// for each layer
			for (int l = 0; l < layers; l++) {
				// write each tile: tile number and sheet
				for (unsigned int y = 0; y < tiles.size(); y++) {
					for (unsigned int x = 0; x < tiles[0].size(); x++) {
						_Tile t = tiles[y][x][l];
						al_fwrite32le(f, t.number);
						al_fputc(f, t.sheet);
						al_fputc(f, t.solid);
					}
				}
			}

			al_fclose(f);
		}

		al_show_native_message_box(
			display,
			"Saved",
			"",
			"The map has been saved.",
			"OK",
			0
		);

		tgui::clearKeyState();

		add_to_recent(filename, true);

		return true;
	}

	bool load(void) {
		std::string path = getLoadSavePath(true);
		if (path == "") return false;
		lastSaveName = path;
		ALLEGRO_PATH *tmp = al_create_path(path.c_str());
		operatingFilename = std::string(al_get_path_filename(tmp));
		al_destroy_path(tmp);
		load(path);
		return true;
	}

	bool save(bool choose) {
		std::string name;
		if (!choose) {
			if (lastSaveName == "") {
				choose = true;
			}
			else {
				name = lastSaveName;
			}
		}
		if (choose) {
			name = getLoadSavePath(false);
			if (name == "") {
				return false;
			}
		}
		lastSaveNameBak = lastSaveName;
		operatingFilenameBak = operatingFilename;
		lastSaveName = name;
		ALLEGRO_PATH *tmp = al_create_path(name.c_str());
		operatingFilename = std::string(al_get_path_filename(tmp));
		al_destroy_path(tmp);
		bool ret = save(name);
		if (ret == false) {
			al_show_native_message_box(display, "Save Error", "Invalid input!", "File could not be saved at this location!", 0, 0);
		}
		return ret;
	}

	std::string getOperatingFilename(void) {
		return operatingFilename;
	}

	void record(void) {
		if (!recording) {
			macroStartX = macroStartY = -1;
			macro.clear();
		}
		recording = !recording;
	}

	bool isVisible(int l) {
		return visible[l];
	}

	void get_marquee(int *x1, int *y1, int *x2, int *y2) {
		if (x1)
			*x1 = MAX(0, MIN(marquee_x1, marquee_x2));
		if (y1)
			*y1 = MAX(0, MIN(marquee_y1, marquee_y2));
		if (x2)
			*x2 = MIN((int)tiles[0].size(), MAX(marquee_x1, marquee_x2)+1);
		if (y2)
			*y2 = MIN((int)tiles.size(), MAX(marquee_y1, marquee_y2)+1);
	}

	std::vector< std::vector< std::vector<_Tile> > > get_marquee_buffer() {
		return marquee_buffer;
	}

	bool is_marquee_floating() {
		return marquee_floating;
	}

	void get_marquee_float_xy(int *x, int *y)
	{
		if (x)
			*x = marquee_float_x;
		if (y)
			*y = marquee_float_y;
	}

	bool is_marquee_buffer_filled()
	{
		return marquee_buffer_filled;
	}

	bool is_marquee_marked()
	{
		return marquee_marked;
	}

	void toggleLayerVisibility(int layer) {
		visible[layer] = !visible[layer];
	}

	int getCurrentLayer() {
		return layer;
	}

	bool getChanged() {
		return changed;
	}

	void tilesLoaded() {
		if (tileSheets.size() == 0) {
			return;
		}
		int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
		for (int l = 0; l < layers; l++) {
			for (unsigned int y = 0; y < tiles.size(); y++) {
				for (unsigned int x = 0; x < tiles[0].size(); x++) {
					if (tiles[y][x][l].number >= 0) {
						tiles[y][x][l].number = tiles[y][x][l].x + tiles[y][x][l].y * tw;
					}
				}
			}
		}
	}

#ifdef SUPPORT_GROUPS
	void set_group_type(int type) {
		group_type = type;
	}
#endif

	void clear_undoes_redoes()
	{
#ifdef SUPPORT_GROUPS
		groups.clear();
		walls.clear();
#endif

		empty_undoes();
		empty_redoes();
	}

	void new_level(int level_w = General::areaSize, int level_h = General::areaSize) {
		layer = 0;
		layers = General::startLayers;
		clear_undoes_redoes();
		size(level_w, level_h);
		loadSavePath = al_create_path("");
		lastSaveName = "";
		operatingFilename = "";
		marquee_buffer_filled = false;
		marquee_marked = false;
		marquee_floating = false;
		changed = false;
		// you could use this - set it to false when used... right now it's not used
		sel_changed = false;
		num_marked = 0;
		level_type = NONE;
			
		disable_copy_menu_item();
		disable_copy_all_menu_item();
		disable_cut_menu_item();
		disable_cut_all_menu_item();
		disable_delete_menu_item();
		disable_delete_all_menu_item();
		disable_select_none_menu_item();
		enable_select_all_menu_item();
	}

	int getToolNum() {
		return tool;
	}

	void invertSelection()
	{
		if (marquee_floating == false && marquee_marked == false) {
			selectAll();
		}
		else {
			if (marquee_floating) {
				marquee_float_to_selection();
			}
			else {
				push_undo();
			}

			int w = tiles[0].size();
			int h = tiles.size();

			Lvl bak = tiles;
		
			selectNone();

			tiles_bak = tiles;

			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++) {
					if ((bak[y][x][0].flags & TF_MARKED) == 0) {
						sel(x, y);
					}
				}
			}

			marquee_x1 = INT_MAX;
			marquee_y1 = INT_MAX;
			marquee_x2 = INT_MIN;
			marquee_y2 = INT_MIN;

			for (int y = 0; y < (int)tiles.size(); y++) {
				for (int x = 0; x < (int)tiles[0].size(); x++) {
					if ((tiles[y][x][0].flags & TF_MARKED) == 0) {
						continue;
					}
					if (x < marquee_x1) {
						marquee_x1 = x;
					}
					if (x > marquee_x2) {
						marquee_x2 = x;
					}
					if (y < marquee_y1) {
						marquee_y1 = y;
					}
					if (y > marquee_y2) {
						marquee_y2 = y;
					}
				}
			}
		}
	}

	void selectAll()
	{
		if (tool != TOOL_MARQUEE && tool != TOOL_MAGIC_WAND) {
			tool = TOOL_MARQUEE;
		}

		if (marquee_floating) {
			marquee_float_to_selection();
		}
		else {
			push_undo();
		}

		A_Scrollpane *scrollpane = dynamic_cast<A_Scrollpane *>(parent);
		marquee_copy_cut_scroll_x = scrollpane->getOffsetX();
		marquee_copy_cut_scroll_y = scrollpane->getOffsetY();

		int w = tiles[0].size();
		int h = tiles.size();

		marquee_x1 = 0;
		marquee_y1 = 0;
		marquee_x2 = w-1;
		marquee_y2 = h-1;

		selectNone();

		tiles_bak = tiles;

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				sel(x, y);
			}
		}
					
		marquee_marked = true;
				
		enable_copy_menu_item();
		enable_copy_all_menu_item();
		enable_cut_menu_item();
		enable_cut_all_menu_item();
		enable_delete_menu_item();
		enable_delete_all_menu_item();
		enable_select_none_menu_item();
		disable_select_all_menu_item();
	}

	std::string getLastSaveName()
	{
		return lastSaveName;
	}

	A_Leveleditor(drawCallback callback, int layers, A_Tileselector *ts) :
		A_Canvas(callback),
		layers(layers),
		down(false),
		layer(0),
		number(-1),
		sheet(-1),
		lastSaveName(""),
		operatingFilename(""),
		statusX(-1),
		statusY(-1),
		tool(0),
		recording(false),
		cloneStartX(-1),
		ts(ts),
		marquee_marked(false),
		marquee_buffer_filled(false),
		marquee_floating(false),
		dragging_marquee(false)
#ifdef SUPPORT_GROUPS
		,group_type(0)
#endif
	{
		new_level();
	}

	~A_Leveleditor(void) {
		al_destroy_path(loadSavePath);
	}

protected:
	static const unsigned int MAX_UNDO = 32;

	ASH_Point mkpoint(int x, int y) {
		ASH_Point p;
		p.x = x;
		p.y = y;
		return p;
	}

	void unmark(std::pair<int, int> p)
	{
		int low_x = INT_MAX;
		int high_x = INT_MIN;
		int low_y = INT_MAX;
		int high_y = INT_MIN;
		for (int y = 0; y < (int)tiles.size(); y++) {
			for (int x = 0; x < (int)tiles[0].size(); x++) {
				if ((tiles[y][x][0].flags & TF_MARKED) == 0) {
					continue;
				}
				if (p.first == x && p.second == y) {
					continue;
				}
				if (x < low_x) {
					low_x = x;
				}
				if (x > high_x) {
					high_x = x;
				}
				if (y < low_y) {
					low_y = y;
				}
				if (y > high_y) {
					high_y = y;
				}
			}
		}
		marquee_x1 = low_x;
		marquee_y1 = low_y;
		marquee_x2 = high_x;
		marquee_y2 = high_y;
		if (marquee_x1 == -1 || marquee_x2 == INT_MAX || marquee_y1 == -1 || marquee_y2 == INT_MAX) {
			marquee_x1 = INT_MAX;
			marquee_y1 = INT_MAX;
			marquee_x2 = INT_MIN;
			marquee_y2 = INT_MIN;
		}
	}

	void magic(int firstx, int firsty, int layer, int x, int y, int tile_num, int tile_sheet, std::stack<ASH_Point> &stack, bool test_visible_layers, bool subtract) {
		ASH_Point neighbors_unclipped[4];
		neighbors_unclipped[0] = mkpoint(x-1, y);
		neighbors_unclipped[1] = mkpoint(x+1, y);
		neighbors_unclipped[2] = mkpoint(x, y+1);
		neighbors_unclipped[3] = mkpoint(x, y-1);

		std::vector<ASH_Point> neighbors;
		for (int i = 0; i < 4; i++) {
			ASH_Point p = neighbors_unclipped[i];
			if (p.x >= 0 && p.y >= 0 && p.x < (int)tiles[0].size() && p.y < (int)tiles.size()) {
				neighbors.push_back(p);
			}
		}

		std::vector<bool> spread(neighbors.size());

		for (int i = 0; i < (int)neighbors.size(); i++) {
			ASH_Point p = neighbors[i];

			int l;

			if (test_visible_layers) {
				for (l = layers-1; l >= 0; l--) {
					if (visible[l] == false) {
						continue;
					}
					if (tiles[p.y][p.x][l].number >= 0 && tiles[p.y][p.x][l].sheet >= 0) {
						break;
					}
				}

				if (l < 0 && tile_num < 0) {
					l = 0;
				}
			}
			else {
				l = layer;
			}

			if (l >= 0 && tiles[p.y][p.x][l].number == tile_num && tiles[p.y][p.x][l].sheet == tile_sheet) {
				spread[i] = true;
			}
			else {
				spread[i] = false;
			}
		}

		for (int i = 0; i < (int)neighbors.size(); i++) {
			if (spread[i] == false) {
				continue;
			}
			ASH_Point p = neighbors[i];
			bool this_v = subtract ? 0 : 1;
			bool that_v = (tiles[p.y][p.x][0].flags & TF_MARKED);
			if (this_v != that_v) {
				stack.push(p);
			}
		}

		int l;

		if (test_visible_layers) {
			for (l = layers-1; l >= 0; l--) {
				if (visible[l] == false) {
					continue;
				}
				if (tiles[y][x][l].number >= 0 && tiles[y][x][l].sheet >= 0) {
					break;
				}
			}

			if (l < 0 && tile_num < 0) {
				l = 0;
			}
		}
		else {
			l = layer;
		}

		if (l >= 0 && tiles[y][x][l].number == tile_num && tiles[y][x][l].sheet == tile_sheet) {
			std::pair<int, int> p(x, y);

			add_sub_magic(p, subtract);
		}
	}

	void add_sub_magic(std::pair<int, int> p, bool subtract)
	{
		if (subtract) {
			unsel(p.first, p.second);
		}
		else {
			sel(p.first, p.second);
			if (p.first < marquee_x1) {
				marquee_x1 = p.first;
			}
			if (p.first > marquee_x2) {
				marquee_x2 = p.first;
			}
			if (p.second < marquee_y1) {
				marquee_y1 = p.second;
			}
			if (p.second > marquee_y2) {
				marquee_y2 = p.second;
			}
		}
	}

	void fill(int firstx, int firsty, int layer, int x, int y, int tile_num, int tile_sheet, std::stack<ASH_Point> &stack, bool test_visible_layers, bool clear, int ts_x, int ts_y, int ts_w, int ts_h) {
		int dx = x - firstx;
		int dy = y - firsty;
		dx %= ts_w;
		dy %= ts_h;
		if (dx < 0) dx = ts_w + dx;
		if (dy < 0) dy = ts_h + dy;

		int tw = al_get_bitmap_width(tileSheets[0]) / General::tileSize;
		int new_tile;
		int new_sheet;
		if (clear) {
			new_tile = -1;
			new_sheet = -1;
		}
		else {
			new_tile = (ts_x+dx) + ((ts_y+dy)*tw);
			new_sheet = sheet;
		}

		ASH_Point neighbors_unclipped[4];
		neighbors_unclipped[0] = mkpoint(x-1, y);
		neighbors_unclipped[1] = mkpoint(x+1, y);
		neighbors_unclipped[2] = mkpoint(x, y+1);
		neighbors_unclipped[3] = mkpoint(x, y-1);

		std::vector<ASH_Point> neighbors;
		for (int i = 0; i < 4; i++) {
			ASH_Point p = neighbors_unclipped[i];
			if (p.x >= 0 && p.y >= 0 && p.x < (int)tiles[0].size() && p.y < (int)tiles.size()) {
				neighbors.push_back(p);
			}
		}

		std::vector<bool> spread(neighbors.size());

		for (int i = 0; i < (int)neighbors.size(); i++) {
			ASH_Point p = neighbors[i];

			int l;

			if (test_visible_layers) {
				for (l = layers-1; l >= 0; l--) {
					if (visible[l] == false) {
						continue;
					}
					if (tiles[p.y][p.x][l].number >= 0 && tiles[p.y][p.x][l].sheet >= 0) {
						break;
					}
				}

				if (l < 0 && tile_num < 0) {
					l = 0;
				}
			}
			else {
				l = layer;
			}

			if (l >= 0 && tiles[p.y][p.x][l].number == tile_num && tiles[p.y][p.x][l].sheet == tile_sheet) {
				spread[i] = true;
			}
			else {
				spread[i] = false;
			}
		}

		for (int i = 0; i < (int)neighbors.size(); i++) {
			if (spread[i] == false) {
				continue;
			}
			std::pair<int, int> p((int)neighbors[i].x, (int)neighbors[i].y);
			if ((tiles[p.second][p.first][0].flags & TF_USED) == 0) {
				stack.push(neighbors[i]);
			}
		}

		int l;

		if (test_visible_layers) {
			for (l = layers-1; l >= 0; l--) {
				if (visible[l] == false) {
					continue;
				}
				if (tiles[y][x][l].number >= 0 && tiles[y][x][l].sheet >= 0) {
					break;
				}
			}

			if (l < 0 && tile_num < 0) {
				l = 0;
			}
		}
		else {
			l = layer;
		}

		if (l >= 0) {
			_Tile &t = tiles[y][x][layer];
			_Tile &t2 = tiles[y][x][l];

			if (t2.number == tile_num && t2.sheet == tile_sheet) {
				bool go;
				int tx;
				int ty;

				if (clear) {
					go = true;
					tx = -1;
					ty = -1;
				}
				else {
					int th = al_get_bitmap_height(tileSheets[0]) / General::tileSize;

					tx = new_tile % tw;
					ty = new_tile / tw;

					if (tx >= 0 && tx < tw && ty >= 0 && ty < th && !blank[ty][tx][sheet]) {
						go = true;
					}
					else {
						go = false;
					}
				}

				if (go) {
					t.number = new_tile;
					t.x = tx;
					t.y = ty;
					t.sheet = new_sheet;
					changed = true;
				}
			}
		}

		tiles[y][x][0].flags |= TF_USED;
	}

	void sel(int x, int y)
	{
		if ((tiles[y][x][0].flags & TF_MARKED) == 0) {
			tiles[y][x][0].flags |= TF_MARKED;
			sel_changed = true;
			num_marked++;
			cols_marked[x]++;
			rows_marked[y]++;
		}
		enable_copy_menu_item();
		enable_copy_all_menu_item();
		enable_cut_menu_item();
		enable_cut_all_menu_item();
		enable_delete_menu_item();
		enable_delete_all_menu_item();
		enable_select_none_menu_item();
		if (num_marked == tiles.size() * tiles[0].size()) {
			disable_select_all_menu_item();
		}
	}

	void unsel(int x, int y)
	{
		if (tiles[y][x][0].flags & TF_MARKED) {
			tiles[y][x][0].flags &= ~TF_MARKED;
			sel_changed = true;
			num_marked--;
			cols_marked[x]--;
			rows_marked[y]--;
		}

		enable_select_all_menu_item();
		if (num_marked == 0) {
			disable_copy_menu_item();
			disable_copy_all_menu_item();
			disable_cut_menu_item();
			disable_cut_all_menu_item();
			disable_delete_menu_item();
			disable_delete_all_menu_item();
			disable_select_none_menu_item();
		}
	}

	int layers;
	bool down;
	int layer;
	int number;
	int sheet;

	typedef std::vector< std::vector< std::vector<_Tile> > > Lvl;
	Lvl tiles;
	Lvl tiles_bak;
	ALLEGRO_PATH *loadSavePath;
	std::string lastSaveName;
	std::string operatingFilename;
	std::string lastSaveNameBak;
	std::string operatingFilenameBak;
	int statusX, statusY;
	int tool;
	bool recording;
	int macroStartX, macroStartY;
	std::vector<_TilePlusPlus> macro;
	int cloneStartX, cloneStartY;
	int cloneTileX, cloneTileY;
	A_Tileselector *ts;
	int mover_src_layer, mover_dest_layer;
	std::vector< std::pair<int, int> > already_moved;

	std::vector<bool> visible;

	int marquee_x1;
	int marquee_y1;
	int marquee_x2;
	int marquee_y2;
	bool marquee_marked;
	bool marquee_buffer_filled;
	std::vector< std::vector< std::vector<_Tile> > > marquee_buffer;
	bool marquee_all;
	bool marquee_floating;
	int marquee_float_x;
	int marquee_float_y;
	bool dragging_marquee;
	int marquee_drag_x;
	int marquee_drag_y;
	int marquee_curr_x1;
	int marquee_curr_y1;
	int marquee_curr_x2;
	int marquee_curr_y2;

	float marquee_copy_cut_scroll_x;
	float marquee_copy_cut_scroll_y;

	struct Marquee {
		int marquee_x1;
		int marquee_y1;
		int marquee_x2;
		int marquee_y2;
		bool marquee_marked;
		bool marquee_buffer_filled;
		std::vector< std::vector< std::vector<_Tile> > > marquee_buffer;
		bool marquee_all;
		bool marquee_floating;
		int marquee_float_x;
		int marquee_float_y;
		bool dragging_marquee;
		int marquee_drag_x;
		int marquee_drag_y;
		Lvl tiles_bak;
		int marquee_curr_x1;
		int marquee_curr_y1;
		int marquee_curr_x2;
		int marquee_curr_y2;
	};

	std::deque<Lvl> undoes;
	std::deque<Lvl> redoes;
#ifdef SUPPORT_GROUPS
	std::deque< std::vector<Group> > group_undoes;
	std::deque< std::vector<Group> > group_redoes;
	std::deque< std::vector<Wall> > wall_undoes;
	std::deque< std::vector<Wall> > wall_redoes;
#endif

	std::deque<Marquee> marquee_undoes;
	std::deque<Marquee> marquee_redoes;

	std::deque<int> tool_undoes;
	std::deque<int> tool_redoes;
	std::deque< std::vector<bool> > draw_solids_undoes;
	std::deque< std::vector<bool> > draw_solids_redoes;

	std::deque< std::vector<bool> > visible_undoes;
	std::deque< std::vector<bool> > visible_redoes;
	
	std::deque<bool> changed_undoes;
	std::deque<bool> changed_redoes;

#ifdef SUPPORT_GROUPS
	std::deque< std::vector<bool> > draw_groups_undoes;
	std::deque< std::vector<bool> > draw_groups_redoes;
#endif

	std::deque<int> layer_undoes;
	std::deque<int> layer_redoes;

	std::deque<int> num_marked_undoes;
	std::deque<int> num_marked_redoes;

	std::deque< std::vector<int> > cols_marked_undoes;
	std::deque< std::vector<int> > cols_marked_redoes;
	std::deque< std::vector<int> > rows_marked_undoes;
	std::deque< std::vector<int> > rows_marked_redoes;

	std::vector<int> cols_marked;
	std::vector<int> rows_marked;

	int num_marked;

	int last_solid_x;
	int last_solid_y;

	bool changed;

#ifdef SUPPORT_GROUPS
	std::vector<Group> groups;
	int group_type;

	std::vector<Wall> walls;
	int selected_wall;
#endif

	int last_big_pencil_x;
	int last_big_pencil_y;

	LEVEL_TYPE level_type;

	bool is_rmb;

	bool sel_changed;
};

extern A_Leveleditor *levelEditor;

class A_Toolbar : public tgui::TGUIWidget {
public:
	static const int HEIGHT = 48;
	static const int NUM_ICONS = 13;
	
	enum ORDER {
		TB_PENCIL = 0,
		TB_FILL,
		TB_CLONE,
		TB_CLEAR,
		TB_SOLIDS,
		TB_MARQUEE,
		TB_MAGIC_WAND,
		TB_MOVER,
		TB_MACRO,
		TB_RECORD,
		TB_TOGGLE_LAYER,
		TB_TOGGLE_SOLIDS,
		TB_SCREENSHOT
	};

	virtual void draw(int abs_x, int abs_y) {
		al_draw_line(abs_x, abs_y+HEIGHT+3.0f, abs_x+width, abs_y+HEIGHT+3.0f, al_map_rgb(0, 0, 0), 1);

		ALLEGRO_BITMAP *bitmaps[] = {
			pencil_icon,
			fill_icon,
			clone_icon,
			clear_icon,
			solids_icon,
			marquee_icon,
			magic_wand_icon,
			mover_icon,
			macro_icon,
			record_icon,
			toggle_layer_icon,
			toggle_solids_icon,
			screenshot_icon
		};

		int tool = levelEditor->getToolNum();

		int index = getToolIndex(tool);

		ALLEGRO_COLOR red = al_map_rgb_f(1.0f, 0.5f, 0.5f);
		ALLEGRO_COLOR green = al_map_rgb_f(0.5f, 1.0f, 0.5f);
		ALLEGRO_COLOR blue = al_map_rgb_f(0.5f, 0.5f, 1.0f);

		if (index >= 0) {
			int x = abs_x + (HEIGHT+2) * index;
			al_draw_filled_rectangle(x+1, abs_y+1, x+HEIGHT+1, abs_y+HEIGHT+1, blue);
		}

		if (levelEditor->getRecording()) {
			int x = abs_x + (HEIGHT+2) * TB_RECORD;
			al_draw_filled_rectangle(x+1, abs_y+1, x+HEIGHT+1, abs_y+HEIGHT+1, red);
		}
			
		int layer = levelEditor->getCurrentLayer();
		bool draw_solids_bg;
		if (levelEditor->get_level_type() != CP) {
			draw_solids_bg = draw_solids[0];
		}
		else {
			draw_solids_bg = draw_solids[layer];
		}
		if (draw_solids_bg) {
			int x = abs_x + (HEIGHT+2) * TB_TOGGLE_SOLIDS;
			al_draw_filled_rectangle(x+1, abs_y+1, x+HEIGHT+1, abs_y+HEIGHT+1, green);
		}

		if (levelEditor->isVisible(layer)) {
			int x = abs_x + (HEIGHT+2) * TB_TOGGLE_LAYER;
			al_draw_filled_rectangle(x+1, abs_y+1, x+HEIGHT+1, abs_y+HEIGHT+1, green);
		}

		int x = abs_x;

		for (int i = 0; i < NUM_ICONS; i++) {
			ALLEGRO_BITMAP *b = bitmaps[i];
			int w = al_get_bitmap_width(b);
			int h = al_get_bitmap_height(b);
			al_draw_scaled_bitmap(b, 0, 0, w, h, x+1, abs_y+1, HEIGHT, HEIGHT, 0);
			x += HEIGHT+2;
		}
	}
		
	virtual void postDraw(int abs_x, int abs_y) {
		if (hover_icon >= 0) {
			bool draw_tooltip;
			int x = abs_x + (HEIGHT+2) * hover_icon;
			if (hover_icon >= 0 && al_get_time()-hover_start >= 1.0) {
				draw_tooltip = true;
			}
			else {
				draw_tooltip = false;
			}
			if (draw_tooltip) {
				std::string tooltips[] = {
					"Pencil (P)",
					"Fill (F)",
					"Clone (K)",
					"Clear (C)",
					"Solids (S)",
					"Marquee (Q)",
					"Magic Wand (W)",
					"Layer Mover (V)",
					"Macro (M)",
					"Record (R)",
					"Toggle Layer (T)",
					"Toggle Solids (Ctrl+T)",
					"Screenshot (Enter)",
					""
				};

				int count = 0;
				for (int i = 0; tooltips[i] != ""; i++) {
					count++;
				}
				
				if (hover_icon < count) {
					int w = al_get_text_width(help_font, tooltips[hover_icon].c_str());
					int yy = mouse_y + 24;
					al_draw_filled_rectangle(
						x+5, yy, x+5+w+10, yy+al_get_font_line_height(help_font)+10, al_map_rgb(255, 255, 255)
					);
					al_draw_rectangle(
						x+5.5f, yy+0.5f, x+5+w+10, yy+al_get_font_line_height(help_font)+10, al_map_rgb(0, 0, 0), 1.0f
					);
					al_draw_text(
						help_font,
						al_map_rgb(0, 0, 0),
						x+10, yy+5,
						0,
						tooltips[hover_icon].c_str()
					);
				}
			}
		}
	}

	virtual void mouseDown(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
		if (rel_x < 0 || rel_y < 0) {
			return;
		}

		int icon = rel_x / (HEIGHT+2);

		if (icon >= NUM_ICONS) {
			return;
		}

		if (icon == TB_PENCIL) {
			levelEditor->keyDown(ALLEGRO_KEY_P);
		}
		else if (icon == TB_CLEAR) {
			levelEditor->keyDown(ALLEGRO_KEY_C);
		}
		else if (icon == TB_FILL) {
			levelEditor->keyDown(ALLEGRO_KEY_F);
		}
		else if (icon == TB_SOLIDS) {
			levelEditor->keyDown(ALLEGRO_KEY_S);
		}
		else if (icon == TB_CLONE) {
			levelEditor->keyDown(ALLEGRO_KEY_K);
		}
		else if (icon == TB_MARQUEE) {
			levelEditor->keyDown(ALLEGRO_KEY_Q);
		}
		else if (icon == TB_MAGIC_WAND) {
			levelEditor->keyDown(ALLEGRO_KEY_W);
		}
		else if (icon == TB_MOVER) {
			levelEditor->keyDown(ALLEGRO_KEY_V);
		}
		else if (icon == TB_MACRO) {
			levelEditor->keyDown(ALLEGRO_KEY_M);
		}
		else if (icon == TB_RECORD) {
			levelEditor->keyDown(ALLEGRO_KEY_R);
		}
		else if (icon == TB_TOGGLE_LAYER) {
			levelEditor->keyDown(ALLEGRO_KEY_T);
		}
		else if (icon == TB_TOGGLE_SOLIDS) {
			int layer;
			if (levelEditor->get_level_type() != CP) {
				layer = 0;
			}
			else {
				layer = levelEditor->getCurrentLayer();
			}
			draw_solids[layer] = !draw_solids[layer];
		}
		else if (icon == TB_SCREENSHOT) {
			screenshot();
		}
	}

	virtual void mouseUp(int rel_x, int rel_y, int abs_x, int abs_y, int mb) {
	}

	virtual void mouseMove(int rel_x, int rel_y, int abs_x, int abs_y) {
		if (rel_x < 0 || rel_y < 0) {
			hover_icon = -1;
			return;
		}

		int icon = rel_x / (HEIGHT+2);

		if (icon >= NUM_ICONS) {
			hover_icon = -1;
			return;
		}

		hover_icon = icon;
		hover_start = al_get_time();
		mouse_y = abs_y;
	}

	int getToolIndex(int tool) {
		int indices[] = {
			A_Leveleditor::TOOL_PENCIL,
			A_Leveleditor::TOOL_FILL,
			A_Leveleditor::TOOL_CLONE,
			A_Leveleditor::TOOL_CLEAR,
			A_Leveleditor::TOOL_SOLID,
			A_Leveleditor::TOOL_MARQUEE,
			A_Leveleditor::TOOL_MAGIC_WAND,
			A_Leveleditor::TOOL_MOVER,
			A_Leveleditor::TOOL_MACRO,
			-1,
			-1,
			-1,
			-1
		};

		for (int i = 0; i < NUM_ICONS; i++) {
			if (indices[i] == tool) {
				return i;
			}
		}

		return -1;
	}

	A_Toolbar()
	{
		hover_icon = -1;
		hover_start = 0;
	}

protected:
	int hover_icon;
	double hover_start;
	int mouse_y;
};


class A_Blank : public tgui::TGUIWidget {
public:
	virtual void draw(int abs_x, int abs_y) {
		al_draw_filled_rectangle(
			abs_x+1,
			abs_y,
			abs_x+width,
			abs_y+height,
			al_map_rgb(255, 255, 255)
		);

		if (draw_line) {
			int cx, cy, cw, ch;
			tgui::getClip(&cx, &cy, &cw, &ch);
			tgui::clearClip();

			if (left) {
				al_draw_line(abs_x+width, abs_y-1, abs_x+width, abs_y+height+1, al_map_rgb(0, 0, 0), 1.0f);
			}
			else {
				al_draw_line(abs_x+1, abs_y-1, abs_x+1, abs_y+height+1, al_map_rgb(0, 0, 0), 1.0f);
			}
		
			tgui::setClip(cx, cy, cw, ch);
		}
	}

	A_Blank(bool left, bool draw_line) :
		left(left),
		draw_line(draw_line)
	{
	}

private:
	bool left;
	bool draw_line;
};

class InputGetterDummy : public tgui::TGUIWidget
{
public:
	static const int PADDING = 5;

	virtual void draw(int abs_x, int abs_y) {
		al_draw_filled_rectangle(
			abs_x+PADDING,
			abs_y+PADDING,
			abs_x+width+PADDING,
			abs_y+height+PADDING,
			al_map_rgba(0, 0, 0, 100)
		);

		al_draw_filled_rectangle(
			abs_x,
			abs_y,
			abs_x+width,
			abs_y+height,
			//color
			al_map_rgb(255, 255, 255)
		);

		al_draw_line(
			abs_x,
			abs_y+0.5,
			abs_x+width,
			abs_y+0.5,
			al_map_rgba(255, 255, 255, 150),
			1
		);

		al_draw_line(
			abs_x+0.5,
			abs_y,
			abs_x+0.5,
			abs_y+height-1,
			al_map_rgba(255, 255, 255, 150),
			1
		);

		al_draw_line(
			abs_x+width-0.5,
			abs_y+1,
			abs_x+width-0.5,
			abs_y+height,
			al_map_rgba(0, 0, 0, 150),
			1
		);

		al_draw_line(
			abs_x,
			abs_y+height-0.5,
			abs_x+width,
			abs_y+height-0.5,
			al_map_rgba(0, 0, 0, 150),
			1
		);
	}

	bool getAbsoluteChildPosition(tgui::TGUIWidget *widget, int *x, int *y)
	{
		if (child == widget) {
			int xx, yy;
			tgui::determineAbsolutePosition(this, &xx, &yy);
			*x = xx;
			*y = yy;
			return true;
		}

		if (child->getAbsoluteChildPosition(widget, x, y)) {
			return true;
		}

		return false;
	}

	virtual void translate(int xx, int yy) {
		x += xx;
		y += yy;

		if (child)
			child->translate(xx, yy);
	}

	virtual void resize(void) {
		resize_child();
	}

	void setSize(int w, int h)
	{
		width = w;
		height = h;
	}
	
	void setPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

class InputGetter : public tgui::TGUIWidget
{
public:
	static const int PADDING = 5;

	void handle_event(ALLEGRO_EVENT *event)
	{
		if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				pushEvent(TGUI_EVENT_OBJECT, (void *)this);
			}
			else if (event->keyboard.keycode == ALLEGRO_KEY_ENTER || event->keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				std::string result;
				if ((result = validate()) != "") {
					al_show_native_message_box(display, "Invalid Input", result.c_str(), "", 0, 0);
				}
				else {
					set_values();
				}
				pushEvent(TGUI_EVENT_OBJECT, (void *)this);
			}
			else if (event->keyboard.keycode == ALLEGRO_KEY_TAB) {
				switch_fields();
			}
			else {
				handle_key(get_current_string(), event);
			}
		}
		else if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			handle_mouse_down(event->mouse.x, event->mouse.y);
		}
	}

	void handle_key(std::string &s, ALLEGRO_EVENT *event)
	{
		if (
			event->keyboard.keycode >= ALLEGRO_KEY_0 &&
			event->keyboard.keycode <= ALLEGRO_KEY_9
		) {
			int n = event->keyboard.keycode - ALLEGRO_KEY_0;
			if (s.length() < 3) {
				char buf[2];
				buf[0] = n + '0';
				buf[1] = 0;
				//s += buf;
				s = s.substr(0, cursorPos) + buf + s.substr(cursorPos);
				cursorPos++;
			}
		}
		else if (
			event->keyboard.keycode >= ALLEGRO_KEY_PAD_0 &&
			event->keyboard.keycode <= ALLEGRO_KEY_PAD_9
		) {
			int n = event->keyboard.keycode - ALLEGRO_KEY_PAD_0;
			if (s.length() < 3) {
				char buf[2];
				buf[0] = n + '0';
				buf[1] = 0;
				//s += buf;
				s = s.substr(0, cursorPos) + buf + s.substr(cursorPos);
				cursorPos++;
			}
		}
		else if (event->keyboard.keycode == ALLEGRO_KEY_LEFT) {
			if (cursorPos > 0) {
				cursorPos--;
			}
		}
		else if (event->keyboard.keycode == ALLEGRO_KEY_RIGHT) {
			if (cursorPos < s.length()) {
				cursorPos++;
			}
		}
		else if (event->keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
			if (s.length() > 0 && cursorPos > 0) {
				//s = s.substr(0, s.length()-1);
				s = s.substr(0, cursorPos-1) + s.substr(cursorPos);
				cursorPos--;
			}
		}
		else if (event->keyboard.keycode == ALLEGRO_KEY_DELETE) {
			if (cursorPos < s.length() && s.length() > 0) {
				if (cursorPos == s.length()-1) {
					s = s.substr(0, cursorPos);
				}
				else {
					s = s.substr(0, cursorPos) + s.substr(cursorPos+1);
				}
			}
		}
	}

	virtual std::string validate() = 0;
	virtual void set_values() = 0;
	virtual std::string &get_current_string() = 0;
	virtual void handle_mouse_down(int x, int y) = 0;
	virtual void switch_fields() = 0;
	virtual int text_len() = 0;

	InputGetter(int w, int h)
	{
		width = w;
		height = h;

		cursorPos = 0;
	}

	int cursorPos;
};

class TileSizeGetter : public InputGetter
{
public:

	virtual std::string validate()
	{
		const int min = 1;
		const int max = 256;
		char buf[1000];
		sprintf(buf, "Tiles must be between %d and %d pixels.", min, max);
		std::string err = buf;
		if (text.length() <= 0) {
			return err;
		}
		int n = atoi(text.c_str());
		if (n < min || n > max) {
			return err;
		}
		return "";
	}

	virtual void set_values()
	{
		int ts = atoi(text.c_str());

		if (ts != General::tileSize) {
			int ret = al_show_native_message_box(display, "Warning", "Tile size changes can alter the level structure.", "Continue?", 0, ALLEGRO_MESSAGEBOX_YES_NO);

			if (ret == 1) {
				General::tileSize = atoi(text.c_str());
				loadTileSheets(tile_load_path.c_str());
				levelEditor->resizeScrollpane();
				levelScrollpane->scrollBy(-1000000, -1000000);
				tileSelector->setSelected(0, 0, 1, 1);
				my_tile_size = General::tileSize;
				save_tile_size = General::tileSize;
				
				int pane_size_x, pane_size_y;
				get_ss(&pane_size_x, &pane_size_y);
				tileScrollpane->setScrollSize(pane_size_x, pane_size_y);
			}
		}
	}

	virtual std::string &get_current_string()
	{
		return text;
	}

	virtual void handle_mouse_down(int x, int y)
	{
		int abs_x, abs_y;
		determineAbsolutePosition(this, &abs_x, &abs_y);

		int ok_w = al_get_text_width(font, "OK");
		int cancel_w = al_get_text_width(font, "Cancel");
		int tot_w = ok_w + cancel_w;

		if (x >= abs_x+width/2-(tot_w+20)/2-2-5 && y >= abs_y+20+al_get_font_line_height(font)-2+10 &&
			x < abs_x+width/2-(tot_w+20)/2+ok_w+2+5 && y < abs_y+20+al_get_font_line_height(font)*2+2+20) {
			ALLEGRO_EVENT e;
			e.type = ALLEGRO_EVENT_KEY_DOWN;
			e.keyboard.keycode = ALLEGRO_KEY_ENTER;
			handle_event(&e);
		}
		else if (x >= abs_x+width/2-(tot_w+20)/2+ok_w+20-2-5 && y >= abs_y+20+al_get_font_line_height(font)-2+10 &&
			x < abs_x+width/2-(tot_w+20)/2+ok_w+20+cancel_w+2+5 && y < abs_y+20+al_get_font_line_height(font)*2+2+20) {
			ALLEGRO_EVENT e;
			e.type = ALLEGRO_EVENT_KEY_DOWN;
			e.keyboard.keycode = ALLEGRO_KEY_ESCAPE;
			handle_event(&e);
		}
	}

	virtual void switch_fields()
	{
	}

	virtual void draw(int abs_x, int abs_y) {
		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+10,
			abs_y+PADDING+10,
			0,
			"Tile Size:"
		);

		al_draw_rectangle(
			abs_x+PADDING+20+100+0.5f-2.0f-2,
			abs_y+PADDING+10.5f-2.0f,
			abs_x+PADDING+20+100+0.5f-2.0f+100.0f+2.0f+2,
			abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+2.0f,
			al_map_rgb(0xff, 0, 0x27),
			1.0f
		);

		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+20+100+100-al_get_text_width(font, text.c_str()),
			abs_y+PADDING+10,
			0,
			text.c_str()
		);

		std::string ct = text.substr(0, cursorPos);

		int cp = abs_x+PADDING+20+100+100-al_get_text_width(font, text.c_str())+al_get_text_width(font, ct.c_str());

		al_draw_line(cp, abs_y+PADDING+10, cp, abs_y+PADDING+10+al_get_font_line_height(font)-1, al_map_rgb(0, 0, 255), 2);

		int ok_w = al_get_text_width(font, "OK");
		int cancel_w = al_get_text_width(font, "Cancel");
		int tot_w = ok_w + cancel_w;

		al_draw_filled_rectangle(
			abs_x+width/2-(tot_w+20)/2-2-5,
			abs_y+20+al_get_font_line_height(font)-2+10,
			abs_x+width/2-(tot_w+20)/2+ok_w+2+5,
			abs_y+20+al_get_font_line_height(font)*2+2+20,
			al_map_rgb(0, 0, 255)
		);

		al_draw_filled_rectangle(
			abs_x+width/2-(tot_w+20)/2+ok_w+20-2-5,
			abs_y+20+al_get_font_line_height(font)-2+10,
			abs_x+width/2-(tot_w+20)/2+ok_w+20+cancel_w+2+5,
			abs_y+20+al_get_font_line_height(font)*2+2+20,
			al_map_rgb(0, 0, 255)
		);
		
		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			abs_x+width/2-(tot_w+20)/2,
			abs_y+20+al_get_font_line_height(font)+2+10+2,
			0,
			"OK"
		);
		
		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			abs_x+width/2-(tot_w+20)/2+ok_w+20,
			abs_y+20+al_get_font_line_height(font)+2+10+2,
			0,
			"Cancel"
		);
	}

	int text_len()
	{
		return strlen(text.c_str());
	}

	TileSizeGetter(ALLEGRO_FONT *font, int initial_value) :
		InputGetter(PADDING*2+30+200, PADDING*2+30+al_get_font_line_height(font)*2+28+20)
	{
		char buf[1000];
		sprintf(buf, "%d", initial_value);
		text = buf;

		this->font = font;

		cursorPos = strlen(buf);
	}

private:
	std::string text;
	ALLEGRO_FONT *font;
};

class LevelSizeGetter : public InputGetter
{
public:

	virtual std::string validate()
	{
		const int min = 1;
		//65536 is the theoretical max, but this app is not compatible with it yet due to taking a lot of memory
		//const int max = 65536;
		const int max = MAX_LEVEL_SIZE;
		char buf[1000];
		sprintf(buf, "Maps must be between %dx%d and %dx%d.", min, min, max, max);
		std::string err = buf;
		if (w_text.length() <= 0) {
			return err;
		}
		if (h_text.length() <= 0) {
			return err;
		}
		int n = atoi(w_text.c_str());
		if (n < min || n > max) {
			return err;
		}
		n = atoi(h_text.c_str());
		if (n < min || n > max) {
			return err;
		}
		return "";
	}

	virtual void set_values()
	{
		int new_level_w = atoi(w_text.c_str());
		int new_level_h = atoi(h_text.c_str());
	
		new_map_real(new_level_w, new_level_h);
	}

	virtual std::string &get_current_string()
	{
		if (curr == 0) {
			return w_text;
		}
		else {
			return h_text;
		}
	}

	virtual void handle_mouse_down(int x, int y)
	{
		int abs_x, abs_y;
		determineAbsolutePosition(this, &abs_x, &abs_y);

		int ok_w = al_get_text_width(font, "OK");
		int cancel_w = al_get_text_width(font, "Cancel");
		int tot_w = ok_w + cancel_w;

		if (x >= abs_x+width/2-(tot_w+20)/2-2-5 && y >= abs_y+al_get_font_line_height(font)+10+20+al_get_font_line_height(font)-2+10 &&
			x < abs_x+width/2-(tot_w+20)/2+ok_w+2+5 && y < abs_y+al_get_font_line_height(font)+10+20+al_get_font_line_height(font)*2+2+20) {
			ALLEGRO_EVENT e;
			e.type = ALLEGRO_EVENT_KEY_DOWN;
			e.keyboard.keycode = ALLEGRO_KEY_ENTER;
			handle_event(&e);
		}
		else if (x >= abs_x+width/2-(tot_w+20)/2+ok_w+20-2-5 && y >= abs_y+al_get_font_line_height(font)+10+20+al_get_font_line_height(font)-2+10 &&
			x < abs_x+width/2-(tot_w+20)/2+ok_w+20+cancel_w+2+5 && y < abs_y+al_get_font_line_height(font)+10+20+al_get_font_line_height(font)*2+2+20) {
			ALLEGRO_EVENT e;
			e.type = ALLEGRO_EVENT_KEY_DOWN;
			e.keyboard.keycode = ALLEGRO_KEY_ESCAPE;
			handle_event(&e);
		}
		else if (x >= abs_x+PADDING+20+100+0.5f-2.0f && y >= abs_y+PADDING+10.5f-2.0f && x < abs_x+PADDING+20+100+0.5f-2.0f+100.0f+2.0f && y < abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+2.0f) {
			curr = 0;
			cursorPos = text_len();
		}
		else if (x >= abs_x+PADDING+20+100+0.5f-2.0f && y >= abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+10 && x < abs_x+PADDING+20+100+0.5f-2.0f+100.0f+2.0f && y < abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+2.0f+al_get_font_line_height(font)+10) {
			curr = 1;
			cursorPos = text_len();
		}
	}

	virtual void switch_fields()
	{
		if (curr == 0) {
			curr = 1;
		}
		else {
			curr = 0;
		}

		cursorPos = text_len();
	}

	virtual void draw(int abs_x, int abs_y) {
		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+10,
			abs_y+PADDING+10,
			0,
			"Width:"
		);
		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+10,
			abs_y+PADDING+10+al_get_font_line_height(font)+10,
			0,
			"Height:"
		);

		al_draw_rectangle(
			abs_x+PADDING+20+100+0.5f-2.0f-2,
			abs_y+PADDING+10.5f-2.0f,
			abs_x+PADDING+20+100+0.5f-2.0f+100.0f+2.0f+2,
			abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+2.0f,
			curr == 0 ? al_map_rgb(0xff, 0, 0x27) : al_map_rgb(0, 0, 0),
			1.0f
		);

		al_draw_rectangle(
			abs_x+PADDING+20+100+0.5f-2.0f-2,
			abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+10,
			abs_x+PADDING+20+100+0.5f-2.0f+100.0f+2.0f+2,
			abs_y+PADDING+10.5f-2.0f+al_get_font_line_height(font)+2.0f+al_get_font_line_height(font)+10,
			curr == 1 ? al_map_rgb(0xff, 0, 0x27) : al_map_rgb(0, 0, 0),
			1.0f
		);

		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+20+100+100-al_get_text_width(font, w_text.c_str()),
			abs_y+PADDING+10,
			0,
			w_text.c_str()
		);

		if (curr == 0) {
			std::string ct = w_text.substr(0, cursorPos);

			int cp = abs_x+PADDING+20+100+100-al_get_text_width(font, w_text.c_str())+al_get_text_width(font, ct.c_str());

			al_draw_line(cp, abs_y+PADDING+10, cp, abs_y+PADDING+10+al_get_font_line_height(font)-1, al_map_rgb(0, 0, 255), 2);
		}

		al_draw_text(
			font,
			al_map_rgb(0, 0, 0),
			abs_x+PADDING+20+100+100-al_get_text_width(font, h_text.c_str()),
			abs_y+PADDING+10+al_get_font_line_height(font)+10,
			0,
			h_text.c_str()
		);

		if (curr == 1) {
			std::string ct = h_text.substr(0, cursorPos);

			int cp = abs_x+PADDING+20+100+100-al_get_text_width(font, h_text.c_str())+al_get_text_width(font, ct.c_str());

			al_draw_line(cp, abs_y+PADDING+10+al_get_font_line_height(font)+10-1, cp, abs_y+PADDING+10+al_get_font_line_height(font)+10+al_get_font_line_height(font), al_map_rgb(0, 0, 255), 2);
		}

		int ok_w = al_get_text_width(font, "OK");
		int cancel_w = al_get_text_width(font, "Cancel");
		int tot_w = ok_w + cancel_w;

		al_draw_filled_rectangle(
			abs_x+width/2-(tot_w+20)/2-2-5,
			abs_y+20+al_get_font_line_height(font)-2+10+al_get_font_line_height(font)+10,
			abs_x+width/2-(tot_w+20)/2+ok_w+2+5,
			abs_y+20+al_get_font_line_height(font)*2+2+20+al_get_font_line_height(font)+10,
			al_map_rgb(0, 0, 255)
		);

		al_draw_filled_rectangle(
			abs_x+width/2-(tot_w+20)/2+ok_w+20-2-5,
			abs_y+20+al_get_font_line_height(font)-2+10+al_get_font_line_height(font)+10,
			abs_x+width/2-(tot_w+20)/2+ok_w+20+cancel_w+2+5,
			abs_y+20+al_get_font_line_height(font)*2+2+20+al_get_font_line_height(font)+10,
			al_map_rgb(0, 0, 255)
		);
		
		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			abs_x+width/2-(tot_w+20)/2,
			abs_y+20+al_get_font_line_height(font)+2+10+2+al_get_font_line_height(font)+10,
			0,
			"OK"
		);
		
		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			abs_x+width/2-(tot_w+20)/2+ok_w+20,
			abs_y+20+al_get_font_line_height(font)+2+10+2+al_get_font_line_height(font)+10,
			0,
			"Cancel"
		);
	}

	int text_len()
	{
		if (curr == 0) {
			return strlen(w_text.c_str());
		}
		else {
			return strlen(h_text.c_str());
		}
	}

	LevelSizeGetter(ALLEGRO_FONT *font) :
		InputGetter(PADDING*2+30+200, PADDING*2+30+al_get_font_line_height(font)*3+28+20+10)
	{
		char buf[1000];
		sprintf(buf, "%d", General::areaSize);
		w_text = buf;
		h_text = buf;

		this->font = font;

		curr = 0;

		cursorPos = strlen(buf);
	}

private:
	std::string w_text;
	std::string h_text;
	int curr;
	ALLEGRO_FONT *font;
};

#endif // WIDGETS_H
