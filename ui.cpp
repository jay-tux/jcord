#ifndef JCORD_UI
#define JCORD_UI

#include "ui.h"

void newwin(WINDOW *w, int y0, int x0, int height, int width)
{
    w = newwin(height, width, y0, x0);
    werase(w);
    box(w, 0, 0);
    wrefresh(w);
}

CLIUI::CLIUI()
{
    //redirect buffer stuff
    std::ostream devnull(0);
    auto coutbuf = std::cout.rdbuf(devnull.rdbuf());

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    refresh();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    //coord: in (row, col)
    //servers: 7 cols wide      (0,  0) ~> (max,  7)
    //channels: 10 cols wide    (0,  7) ~> (max, 17)
    //main: the rest            (0, 17) ~> (max - 3, max - 10)
    //bottom: 3 rows high       (max - 3, 17) ~> (max, max - 10)
    //users: 10 cols wide       (0, max - 10) ~> (max, max)
    newwin(this->servers, 0, 0, rows, WIDTH_SERVERS);
    newwin(this->channels, 0, WIDTH_SERVERS, rows, WIDTH_CHANNELS);
    newwin(this->main, 0, MAIN_OFFSET, rows - HEIGHT_TYPE, cols - MAIN_OFFSET - WIDTH_MEMBERS);
    newwin(this->bottom, rows - HEIGHT_TYPE, MAIN_OFFSET, HEIGHT_TYPE, cols - MAIN_OFFSET - WIDTH_MEMBERS);
    newwin(this->users, 0, cols - WIDTH_MEMBERS, rows, WIDTH_MEMBERS);
}

void CLIUI::render()
{
    //
}

CLIUI::~CLIUI()
{
    delwin(main);
    delwin(users);
    delwin(bottom);
    delwin(servers);
    delwin(channels);
}

#endif
