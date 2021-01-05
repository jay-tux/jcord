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
#include "popup.h"

#define IS_KEY(inp, key, keyalt) \
    ((key != BIND_NO_KEY && inp == key) || \
    (keyalt != BIND_NO_KEY && inp == keyalt))

#define IS_CHR(inp) (inp >= 32 && inp <= 126)
#define IS_RMF(inp) (inp == KEY_DC)
#define IS_RMB(inp) (inp == KEY_BACKSPACE || inp == '\b' || inp == 127)

typedef std::vector<std::string> strVec;

class CLIUI {
    public:
        CLIUI(strVec *, strVec *, strVec *, strVec *, int, int);
        ~CLIUI();
        void render();
        Cursor *getCursor();
        Action resolveBindings();
        Popup *createPopup(std::string, int, int);
        void exitPopup();
        bool inPopupMode();
        std::string getPopupResult();
        std::string getNth(int, Tab);

    protected:
        void init_highlight();
        void highlight_on(WINDOW *);
        void highlight_shroud(WINDOW *);
        void highlight_off(WINDOW *);
        void prepare_highlight(WINDOW *, int, int, int, Tab, bool);

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
        Popup *popup;
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
