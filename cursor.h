#ifndef JCORD_CURSOR_H
#define JCORD_CURSOR_H

typedef enum _tab {
    SERVERS, CHANNELS, TYPE, MESSAGES, USERS
} Tab;

typedef struct _cursor {
    int server;
    int maxserver;
    int channel;
    int maxchannel;
    int highlighted;
    Tab focused;
} Cursor;

#endif
