#ifndef JCORD_UI_H
#define JCORD_UI_H

#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <functional>

#define WIDTH_SERVERS 15
#define WIDTH_CHANNELS 40
#define WIDTH_MEMBERS 40
#define HEIGHT_TYPE 5
#define SERVER_MARGIN_Y 1
#define SERVER_MARGIN_X 1
#define CHANNEL_MARGIN_X 1
#define CHANNEL_MARGIN_Y 1
#define SERVER_H 5
#define CHANNEL_H 3
#define SERVER_W (WIDTH_SERVERS - 2 * SERVER_MARGIN_X)
#define CHANNEL_W (WIDTH_CHANNELS - 2 * CHANNEL_MARGIN_X)
#define MAIN_OFFSET (WIDTH_SERVERS + WIDTH_CHANNELS)

typedef std::vector<std::string> strVec;

class CLIUI {
    public:
        CLIUI(strVec *servers);
        ~CLIUI();
        void render();

    private:
        //windows
        WINDOW *main;
        WINDOW *users;
        WINDOW *bottom;
        WINDOW *channels;
        WINDOW *servers;
        //data sources
        strVec *srv;
};

#endif
