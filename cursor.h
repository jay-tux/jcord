#ifndef JCORD_CURSOR_H
#define JCORD_CURSOR_H

#include <string>

typedef enum _action {
    NO_RERENDER,
    NONE,
    CHANGE_SERVER,
    CHANGE_CHANNEL,
    SEND_MESSAGE,
    LOAD_MORE_MESSAGES,
    STOP
} Action;

//Tab::TYPE not in move loop
typedef enum _tab {
    SERVERS, CHANNELS, TYPE, MESSAGES, USERS
} Tab;

typedef struct _cursor {
    int server;
    int maxserver;
    int channel;
    int maxchannel;
    int highlighted;
    int maxusers;
    int maxmsg;
    Tab focused;
    std::string current;
    int strind;
    bool inputmode;
} Cursor;

/*Action change_server(Cursor *);
Action change_channel(Cursor *);*/
//Action send_message(Cursor *, std::string)
Action focus_up(Cursor *);
Action focus_down(Cursor *);
Action focus_direct(Cursor *, Tab t);
Action focus_left(Cursor *);
Action focus_right(Cursor *);
Action interact(Cursor *);
Action toggle_input(Cursor *, bool);

#endif
