#ifndef JCORD_UI_H
#define JCORD_UI_H

#include <ncurses.h>
#include <locale.h>
#include <iostream>

#define WIDTH_SERVERS 10
#define WIDTH_CHANNELS 40
#define WIDTH_MEMBERS 40
#define HEIGHT_TYPE 5
#define MAIN_OFFSET (WIDTH_SERVERS + WIDTH_CHANNELS)

class CLIUI {
    public:
        CLIUI();
        ~CLIUI();
        void render();

    private:
        WINDOW *main;
        WINDOW *users;
        WINDOW *bottom;
        WINDOW *channels;
        WINDOW *servers;
};

#endif
