#ifndef JCORD_UI
#define JCORD_UI

#include "ui.h"

void get_window(WINDOW **w, int y0, int x0, int height, int width)
{
    *w = newwin(height, width, y0, x0);
    werase(*w);
    box(*w, 0, 0);
    wrefresh(*w);
}

void waddbox(WINDOW *w, int y0, int x0, int height, int width, std::string content)
{
    mvwaddch(w, y0, x0, ACS_ULCORNER);
    mvwaddch(w, y0 + height - 1, x0, ACS_LLCORNER);
    mvwaddch(w, y0, x0 + width - 1, ACS_URCORNER);
    mvwaddch(w, y0 + height - 1, x0 + width - 1, ACS_LRCORNER);

    for(int x = x0 + 1; x < x0 + width - 1; x++)
    {
        mvwaddch(w, y0, x, ACS_HLINE);
        mvwaddch(w, y0 + height - 1, x, ACS_HLINE);
    }

    for(int y = y0 + 1; y < y0 + height - 1; y++)
    {
        mvwaddch(w, y, x0, ACS_VLINE);
        mvwaddch(w, y, x0 + width - 1, ACS_VLINE);
    }

    int amlines = content.length() / (width - 2) + 1;
    if(amlines > height - 2)
    {
        content = content.substr(0, amlines * (width - 2) - 3) + "...";
    }
    int xo = 0;
    int yo = 0;
    if(amlines == 1 && content.length() < (ulong)width - 2)
    {
        int margin = width - content.length() - 2;
        xo = margin / 2;
        yo = height / 2 - 1;
    }

    int y = y0 + 1 + yo;
    int x = x0 + 1 + xo;
    for(auto chr = content.begin(); chr != content.end(); chr++)
    {
        mvwaddch(w, y, x, *chr);
        x++;
        if(x >= x0 + width - 1) { x = x0 + 1; y++; }
    }

    wrefresh(w);
}

bool isdelimiter(char c)
{
    return !((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

std::string firstLetters(std::string str)
{
    std::string res;
    bool add = true;
    for(auto chr = str.begin(); chr != str.end(); chr++)
    {
        if(isdelimiter(*chr)) { add = true; }
        else if(add) { res += std::toupper(*chr); add = false; }
    }
    return res;
}

CLIUI::CLIUI(strVec *servers)
{
    //redirect buffer stuff
    std::ostream devnull(0);
    auto coutbuf = std::cout.rdbuf(devnull.rdbuf());

    this->srv = servers;

    main = nullptr;
    users = nullptr;
    bottom = nullptr;
    servers = nullptr;
    channels = nullptr;

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
    get_window(&this->servers, 0, 0, rows, WIDTH_SERVERS);
    get_window(&this->channels, 0, WIDTH_SERVERS, rows, WIDTH_CHANNELS);
    get_window(&this->main, 0, MAIN_OFFSET, rows - HEIGHT_TYPE, cols - MAIN_OFFSET - WIDTH_MEMBERS);
    get_window(&this->bottom, rows - HEIGHT_TYPE, MAIN_OFFSET, HEIGHT_TYPE, cols - MAIN_OFFSET - WIDTH_MEMBERS);
    get_window(&this->users, 0, cols - WIDTH_MEMBERS, rows, WIDTH_MEMBERS);
    this->render();
}

void CLIUI::render()
{
    //render servers
    if(this->servers != nullptr)
    {
        waddbox(this->servers, SERVER_MARGIN_Y, SERVER_MARGIN_X, SERVER_H, SERVER_W, "DM's");

        int yoff = 2 * SERVER_MARGIN_Y + SERVER_H;
        //std::vector<std::string> servers;//wtf? somehow get some servers?
        for(auto server = this->srv->begin(); server != this->srv->end(); server++)
        {
            waddbox(this->servers, yoff, SERVER_MARGIN_X, SERVER_H, SERVER_W, firstLetters(*server));
            yoff += SERVER_MARGIN_Y + SERVER_H;
        }
    }

    /*wrefresh(main);
    wrefresh(users);
    wrefresh(bottom);
    wrefresh(channels);*/
    refresh();
}

CLIUI::~CLIUI()
{
    delwin(main);
    delwin(users);
    delwin(bottom);
    delwin(servers);
    delwin(channels);
    refresh();
    endwin();
}

#endif
