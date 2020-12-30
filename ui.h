#ifndef JCORD_UI_H
#define JCORD_UI_H

#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "cursor.h"
#include "color.h"
#include "keybinds.h"
#include "offsets.h"

#define IS_KEY(inp, key, keyalt) \
    ((key != BIND_NO_KEY && inp == key) || \
    (keyalt != BIND_NO_KEY && inp == keyalt))

typedef std::vector<std::string> strVec;

typedef enum _action {
    NONE,
    CHANGE_SERVER,
    CHANGE_CHANNEL,
    SEND_MESSAGE
} Action;

class CLIUI { //MVC: VIEW
    public:
        CLIUI(strVec *, strVec *, strVec *, strVec *, int, int);
        ~CLIUI();
        void render();
        Cursor *getCursor();
        Action resolveBindings();

    protected:
        void init_highlight();
        void highlight_on(WINDOW *);
        void highlight_shroud(WINDOW *);
        void highlight_off(WINDOW *);
        void prepare_highlight(WINDOW *, int, int, Tab, bool);

    private:
        //cursor
        Cursor cursor;
        bool colors;
        bool mod;
        //windows
        WINDOW *main;
        WINDOW *users;
        WINDOW *bottom;
        WINDOW *channels;
        WINDOW *servers;
        //data sources
        strVec *srv;
        strVec *chn;
        strVec *usr;
        strVec *msg;
        //sizes
        int width;
        int height;
};

#endif
