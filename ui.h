#ifndef JCORD_UI_H
#define JCORD_UI_H

#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "cursor.h"
#include "color.h"

#include <signal.h> //REMOVE THIS AFTER DEBUGGING!

#define WIDTH_SERVERS 15
#define WIDTH_CHANNELS 40
#define WIDTH_MEMBERS 40
#define HEIGHT_TYPE 5
#define SERVER_MARGIN_Y 1
#define SERVER_MARGIN_X 1
#define CHANNEL_MARGIN_X 1
#define CHANNEL_MARGIN_Y 1
#define USER_MARGIN_X 1
#define USER_MARGIN_Y 4
#define MAIN_MARGIN_X 2
#define MAIN_MSG_TAB "  "
#define MAIN_MARGIN_BOT 1
#define MAIN_MINY 1
#define SERVER_H 5
#define CHANNEL_H 2
#define USER_H 2
#define CHANNEL_HEADER 5
#define SERVER_W (WIDTH_SERVERS - 2 * SERVER_MARGIN_X)
#define CHANNEL_W (WIDTH_CHANNELS - 2 * CHANNEL_MARGIN_X)
#define USER_W (WIDTH_MEMBERS - 2 * USER_MARGIN_X)
#define MAIN_OFFSET (WIDTH_SERVERS + WIDTH_CHANNELS)
#define MAIN_DIFF (WIDTH_SERVERS + WIDTH_CHANNELS + WIDTH_MEMBERS + 2 * MAIN_MARGIN_X)
#define MAIN_START (HEIGHT_TYPE + MAIN_MARGIN_BOT)

typedef std::vector<std::string> strVec;

class CLIUI {
    public:
        CLIUI(strVec *, strVec *, strVec *, strVec *, int, int);
        ~CLIUI();
        void render();
        Cursor *getCursor();

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
