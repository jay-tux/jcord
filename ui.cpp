#ifndef JCORD_UI
#define JCORD_UI

#include "ui.h"

void CLIUI::init_highlight()
{
    this->colors = has_colors();
    if(this->colors)
    {
        //actual initialization
        start_color();
        if(can_change_color() && USE_CHANGED) //use changed color macros, only if enabled
        {
            this->mod = true;
            if(!USE_DEFAULT)
            {
                //init off colors
                init_color(CHANGED_OFF_BACK_INDEX, CHANGED_OFF_BACK_R, CHANGED_OFF_BACK_G, CHANGED_OFF_BACK_B);
                init_color(CHANGED_OFF_FRONT_INDEX, CHANGED_OFF_FRONT_R, CHANGED_OFF_FRONT_G, CHANGED_OFF_FRONT_B);
                init_pair(1, CHANGED_OFF_FRONT_INDEX, CHANGED_OFF_BACK_INDEX);
            }

            //init shrouded colors
            init_color(CHANGED_SHROUD_BACK_INDEX, CHANGED_SHROUD_BACK_R, CHANGED_SHROUD_BACK_G, CHANGED_SHROUD_BACK_B);
            init_color(CHANGED_SHROUD_FRONT_INDEX, CHANGED_SHROUD_FRONT_R, CHANGED_SHROUD_FRONT_G, CHANGED_SHROUD_FRONT_B);
            init_pair(2 - USE_DEFAULT, CHANGED_SHROUD_FRONT_INDEX, CHANGED_SHROUD_BACK_INDEX);

            //init on colors
            init_color(CHANGED_ON_BACK_INDEX, CHANGED_ON_BACK_R, CHANGED_ON_BACK_G, CHANGED_ON_BACK_B);
            init_color(CHANGED_ON_FRONT_INDEX, CHANGED_ON_FRONT_R, CHANGED_ON_FRONT_G, CHANGED_ON_FRONT_B);
            init_pair(3 - USE_DEFAULT, CHANGED_ON_FRONT_INDEX, CHANGED_ON_BACK_INDEX);

        }
        else //use default macros
        {
            this->mod = false;
            if(!USE_DEFAULT)
            {
                init_pair(1, DEFAULT_OFF_FRONT, DEFAULT_OFF_BACK);
            }
            init_pair(2 - USE_DEFAULT, DEFAULT_SHROUD_FRONT, DEFAULT_SHROUD_BACK);
            init_pair(3 - USE_DEFAULT, DEFAULT_ON_FRONT, DEFAULT_ON_BACK);
        }

        short obr, obg, obb, ofr, ofg, ofb, sbr, sbg, sbb, sfr, sfg, sfb, nbr, nbg, nbb, nfr, nfg, nfb;
        short ob, of, sb, sf, nb, nf;
        pair_content(1, &ob, &of);
        pair_content(2, &sb, &sf);
        pair_content(3, &nb, &nf);

        color_content(ob, &obr, &obg, &obb);
        color_content(of, &ofr, &ofg, &ofb);
        color_content(sb, &sbr, &sbg, &sbb);
        color_content(sf, &sfr, &sfg, &sfb);
        color_content(nb, &nbr, &nbg, &nbb);
        color_content(nf, &nfr, &nfg, &nfb);

        //set off
        highlight_off(nullptr);
    }
}

void CLIUI::highlight_on(WINDOW *w)
{
    if(!this->colors) { return; } //do nothing if no color support
    //attron(COLOR_PAIR(3 - USE_DEFAULT));
    wattron(w == nullptr ? stdscr : w, COLOR_PAIR(3 - USE_DEFAULT));
    /*
    for this the _shroud, and the _off method goes:
            USE_DEFAULT     !USE_DEFAULT
    _on       pair 2            pair 3
    _shroud   pair 1            pair 2
    _off      pair 0            pair 1
    pairs are defined using the USE_DEFAULT macro
    */
}

void CLIUI::highlight_off(WINDOW *w)
{
    if(!this->colors) { return; } //do nothing if no color support
    //attron(COLOR_PAIR(1 - USE_DEFAULT));
    wattron(w == nullptr ? stdscr : w, COLOR_PAIR(1 - USE_DEFAULT));
}

void CLIUI::highlight_shroud(WINDOW *w)
{
    if(!this->colors) { return; } //do nothing if no color support
    //attron(COLOR_PAIR(2 - USE_DEFAULT));
    wattron(w == nullptr ? stdscr : w, COLOR_PAIR(2 - USE_DEFAULT));
}

void get_window(WINDOW **w, int y0, int x0, int height, int width)
{
    *w = newwin(height, width, y0, x0);
    werase(*w);
    nodelay(*w, 1);
    box(*w, 0, 0);
    wrefresh(*w);
}

std::string toLength(std::string start, int len, bool centered)
{
    int add = len - start.length();
    if(add < 0)
    {
        return start.substr(0, len - 3) + "...";
    }
    else
    {
        std::stringstream res;
        int left = add;
        if(centered)
        {
            left = add / 2;
            for(int i = 0; i < left; i++) { res << " "; }
            if(add % 2 != 0) { left++; }
        }
        res << start;
        for(int i = 0; i < left; i++) { res << " "; }
        return res.str();
    }
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

    std::string toWrite = toLength(content, (height - 2) * (width - 2), true);
    /*int amlines = content.length() / (width - 2) + 1;
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
    }*/
    int y = y0 + 1; int x = x0 + 1;
    for(auto chr = toWrite.begin(); chr != toWrite.end(); chr++)
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

CLIUI::CLIUI(strVec *servers, strVec *channels, strVec *users, strVec *messages, int server, int channel)
{
    //redirect buffer stuff
    std::ostream devnull(0);
    /*auto coutbuf =*/ std::cout.rdbuf(devnull.rdbuf());

    this->usr = users;
    this->srv = servers;
    this->chn = channels;
    this->msg = messages;

    this->cursor.server = server;
    this->cursor.maxserver = servers->size();
    this->cursor.channel = channel;
    this->cursor.maxchannel = channels->size();
    this->cursor.highlighted = 0;
    this->cursor.focused = Tab::USERS;

    main = nullptr;
    users = nullptr;
    bottom = nullptr;
    servers = nullptr;
    channels = nullptr;

    setlocale(LC_ALL, "");
    initscr();
    init_highlight();
    cbreak();
    noecho();
    refresh();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    nodelay(stdscr, 1);
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
    this->render();
}

void CLIUI::prepare_highlight(WINDOW *w, int index, int checkagainst, Tab requested, bool useshroud)
{
    if(index == checkagainst)
    {
        if(this->cursor.focused == requested) highlight_on(w);
        else if(useshroud) highlight_shroud(w);
    }
    else
    {
        highlight_off(w);
    }
}

void CLIUI::render()
{
    int index = 0;

    //render servers
    if(this->servers != nullptr && this->srv != nullptr)
    {
        //-1 = DM's
        prepare_highlight(this->servers, index - 1, this->cursor.server, Tab::SERVERS, true);
        waddbox(this->servers, SERVER_MARGIN_Y, SERVER_MARGIN_X, SERVER_H, SERVER_W, "DM's");
        highlight_off(this->servers);
        //mvwprintw(this->servers, SERVER_MARGIN_Y, SERVER_MARGIN_X, "%d servers.", this->srv->size());

        int yoff = 2 * SERVER_MARGIN_Y + SERVER_H;
        for(auto server = this->srv->begin(); server != this->srv->end(); server++)
        {
            prepare_highlight(this->servers, index, this->cursor.server, Tab::SERVERS, true);
            waddbox(this->servers, yoff, SERVER_MARGIN_X, SERVER_H, SERVER_W, firstLetters(*server));
            highlight_off(this->servers);

            yoff += SERVER_H;
            index++;
        }
        wrefresh(this->servers);
    }

    index = 0;
    if(this->channels != nullptr && this->chn != nullptr)
    {

        std::string servername = this->cursor.server == -1 ? "Friends" : (*this->srv)[this->cursor.server];

        waddbox(this->channels, CHANNEL_MARGIN_Y, CHANNEL_MARGIN_X, CHANNEL_HEADER, CHANNEL_W, servername);
        //mvwprintw(this->channels, CHANNEL_MARGIN_Y, CHANNEL_MARGIN_X, "%d channels.", this->chn->size());

        int yoff = 2 * CHANNEL_MARGIN_Y + CHANNEL_HEADER;
        for(auto channel = this->chn->begin(); channel != this->chn->end(); channel++)
        {
            prepare_highlight(this->channels, index, this->cursor.channel, Tab::CHANNELS, true);
            mvwaddnstr(this->channels, yoff, CHANNEL_MARGIN_X, toLength(*channel, CHANNEL_W, false).c_str(), CHANNEL_W);
            highlight_off(this->channels);

            yoff += CHANNEL_H;
            index++;
        }
        wrefresh(this->channels);
    }

    index = 0;
    if(this->users != nullptr && this->usr != nullptr)
    {

        //mvwprintw(this->users, USER_MARGIN_Y, USER_MARGIN_X, "%d users.", this->usr->size());
        int yoff = USER_MARGIN_Y;
        for(auto user = this->usr->begin(); user != this->usr->end(); user++)
        {
            prepare_highlight(this->users, index, this->cursor.highlighted, Tab::USERS, false);
            mvwaddnstr(this->users, yoff, USER_MARGIN_X, (*user).c_str(), USER_W);
            highlight_off(this->users);

            yoff += USER_H;
            index++;
        }
        wrefresh(this->users);
    }

    index = 0;
    if(this->main != nullptr && this->msg != nullptr)
    {
        //mvwprintw(this->main, MAIN_MARGIN_BOT, MAIN_MARGIN_X, "%d messages.", this->msg->size());
        int yoff = this->height - MAIN_START;
        for(auto it = this->msg->begin(); it != this->msg->end(); it++)
        {
            prepare_highlight(this->main, index, this->cursor.highlighted, Tab::MESSAGES, false);
            yoff = addMessage(this->main, yoff, MAIN_MARGIN_X, MAIN_MINY, this->width - MAIN_DIFF, *it, MAIN_MSG_TAB);
            highlight_off(this->main);

            index++;
        }
        wrefresh(this->main);
    }

    refresh();
}

Action CLIUI::resolveBindings()
{
    int key = getch();
    //NOT USING SWITCH -> undefined keys (aka BIND_NO_KEY) all resolve to -1
    // so if we'd use switch, we'd get "Error: duplicate case value" while compiling
    return Action::NONE;
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
