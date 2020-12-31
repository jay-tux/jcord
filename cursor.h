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
    int offset;
} Cursor;

void setup_cursor(Cursor *, int, int, int, int, int, int);
void reset_cursor(Cursor *, int, int, int);

Action focus_up(Cursor *);
Action focus_down(Cursor *);
Action focus_direct(Cursor *, Tab t);
Action focus_left(Cursor *);
Action focus_right(Cursor *);

Action interact(Cursor *);
Action toggle_input(Cursor *, bool);

Action input_left(Cursor *);
Action input_right(Cursor *);
Action input_up(Cursor *);
Action input_down(Cursor *);
Action append(Cursor *, char);
Action remove(Cursor *, bool); //true is delete, false is backspace

#endif
