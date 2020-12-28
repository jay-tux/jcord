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

//returns next start point
int addMessage(WINDOW *w, int yoff, int x0, int miny, int maxw, std::string message, std::string tab)
{
    if(message == "") { return yoff; }
    std::vector<std::string> lines;
    lines.push_back("");
    int curlen = 0;
    for(auto chr = message.begin(); chr != message.end(); chr++)
    {
        if(*chr == '\n')
        {
            lines.push_back(tab);
            curlen = tab.length();
        }
        else if(curlen == maxw)
        {
            lines.push_back(tab + *chr);
            curlen = tab.length() + 1;
        }
        else
        {
            lines.back() += *chr;
            curlen++;
        }
    }

    int y = yoff - lines.size();//- 1;
    if(y <= miny) { return yoff; }
    for(int ln = 0; ln < (int)lines.size(); ln++)
    {
        mvwaddnstr(w, y + ln, x0, lines[ln].c_str(), maxw);
    }
    return y;
}

CLIUI::CLIUI(strVec *servers, strVec *channels, strVec *users, strVec *messages, int server)
{
    //redirect buffer stuff
    std::ostream devnull(0);
    /*auto coutbuf =*/ std::cout.rdbuf(devnull.rdbuf());

    this->usr = users;
    this->srv = servers;
    this->chn = channels;
    this->msg = messages;

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
    this->width = cols;
    this->height = rows;
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
    this->render(server);
}

void CLIUI::render(int server)
{
    //render servers
    if(this->servers != nullptr && this->srv != nullptr)
    {
        waddbox(this->servers, SERVER_MARGIN_Y, SERVER_MARGIN_X, SERVER_H, SERVER_W, "DM's");
        //mvwprintw(this->servers, SERVER_MARGIN_Y, SERVER_MARGIN_X, "%d servers.", this->srv->size());

        int yoff = 2 * SERVER_MARGIN_Y + SERVER_H;
        for(auto server = this->srv->begin(); server != this->srv->end(); server++)
        {
            waddbox(this->servers, yoff, SERVER_MARGIN_X, SERVER_H, SERVER_W, firstLetters(*server));
            yoff += SERVER_H;
        }
        wrefresh(this->servers);
    }

    if(this->channels != nullptr && this->chn != nullptr)
    {
        std::string servername = server == -1 ? "Friends" : (*this->srv)[server];

        waddbox(this->channels, CHANNEL_MARGIN_Y, CHANNEL_MARGIN_X, CHANNEL_HEADER, CHANNEL_W, servername);
        //mvwprintw(this->channels, CHANNEL_MARGIN_Y, CHANNEL_MARGIN_X, "%d channels.", this->chn->size());

        int yoff = 2 * CHANNEL_MARGIN_Y + CHANNEL_HEADER;
        for(auto channel = this->chn->begin(); channel != this->chn->end(); channel++)
        {
            mvwaddnstr(this->channels, yoff, CHANNEL_MARGIN_X, (*channel).c_str(), CHANNEL_W);
            yoff += CHANNEL_H;
        }
        wrefresh(this->channels);
    }

    if(this->users != nullptr && this->usr != nullptr)
    {
        //mvwprintw(this->users, USER_MARGIN_Y, USER_MARGIN_X, "%d users.", this->usr->size());
        int yoff = USER_MARGIN_Y;
        for(auto user = this->usr->begin(); user != this->usr->end(); user++)
        {
            mvwaddnstr(this->users, yoff, USER_MARGIN_X, (*user).c_str(), USER_W);
            yoff += USER_H;
        }
        wrefresh(this->users);
    }

    if(this->main != nullptr && this->msg != nullptr)
    {
        //mvwprintw(this->main, MAIN_MARGIN_BOT, MAIN_MARGIN_X, "%d messages.", this->msg->size());
        int yoff = this->height - MAIN_START;
        for(auto it = this->msg->begin(); it != this->msg->end(); it++)
        {
            //yoff = addMessage(this->main, yoff - MAIN_MARGIN_X, MAIN_MINY, MAIN_MARGIN_X, this->width - MAIN_DIFF, *it, MAIN_MSG_TAB);
            //addMessage(WINDOW *w,       int yoff, int x0, int miny, int maxw, std::string message, std::string tab)
            yoff = addMessage(this->main, yoff, MAIN_MARGIN_X, MAIN_MINY, this->width - MAIN_DIFF, *it, MAIN_MSG_TAB);
        }
        wrefresh(this->main);
    }

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
