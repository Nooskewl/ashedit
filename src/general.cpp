#define ALLEGRO_STATICLINK

#include "general.h"

int General::areaSize = 20;

int General::tileSize = 16;

int General::startLayers = 4;
bool General::can_add_and_delete_layers = true;
#ifndef SCALE
#define SCALE 3
#endif
int General::scale = SCALE;

ALLEGRO_EVENT_SOURCE evtsrc;

void pushEvent(int type, void *data)
{
        ALLEGRO_EVENT event;
        event.user.type = ALLEGRO_GET_EVENT_TYPE('T','G','U','I');
        event.user.data1 = TGUI_EVENT_OBJECT;
        event.user.data2 = (intptr_t)data;

        al_emit_user_event(&evtsrc, &event, NULL);
}
