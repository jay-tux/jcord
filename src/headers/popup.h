#ifndef JCORD_POPUP_H
#define JCORD_POPUP_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <sstream>
#include "../../libs/ClipboardXX/include/ClipboardXX.hpp"

void ready_window(WINDOW *);
void get_window(WINDOW **, int, int, int, int);

typedef enum _popupMode {
    UNINIT, CHOICES, STRING
} PopupMode;

typedef enum _popupInput {
    MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
    POPUP_ACTION, KEY, EXIT, REMOVE, DELETE, NOINPUT, PASTE
} PopupInput;

typedef enum _popupAction {
    NOACTION, QUIT, RETURN
} PopupAction;

std::string lastn(std::string, int, int, int *);
std::string toLength(std::string, int, bool);

class Popup {
    public:
        Popup(std::string, int, int, int, int);
        void initialize_choices(std::vector<std::string> *);
        void initialize_input();
        PopupMode getMode();
        PopupAction act(PopupInput, int);
        void render();
        void close();
        std::string getString();
        std::string getOption();

    private:
        std::string title;
        int w;
        int h;
        WINDOW *win;
        bool useinput;
        std::vector<std::string> *opts;
        std::string input;
        int index;
        bool cleaned;
};


#endif
