#ifndef JCORD_MAIN_H
#define JCORD_MAIN_H

#include "../../libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"
#include "token.h"
#include "ui.h"
#include "dmcache.h"
#include "fetcher.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <functional>
#include <cctype>

typedef SleepyDiscord::Snowflake<SleepyDiscord::Server> ServerFlake;

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
