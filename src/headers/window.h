#ifndef JCORD_WINDOW_H
#define JCORD_WINDOW_H

#include "fetcher.h"
#include "ui.h"

typedef enum _popupType {
    NO_POPUP, DM_POPUP, USER_POPUP, MEMBER_POPUP, MESSAGE_POPUP
} PopupType;

class Window {
    public:
        Window();
        ~Window();
        void setFetcher(Fetcher *f);
        Fetcher *getFetcher() { return this->data; }
        void start();
    private:
        Fetcher *data;
        CLIUI *cli;
        PopupType popup;
};

#endif
