#ifndef JCORD_MAIN_H
#define JCORD_MAIN_H

#include "libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"
#include "token.h"
#include "ui.h"
#include <iostream>
#include <thread>

#include "fetcher.h"

class Window;

class Fetcher {
    public:
        Fetcher(Window *w, Connector *conn);
        void start();
        void stop();
    private:
        Window *ui;
        Connector *discord;
};

class Window {
    public:
        Window();
        ~Window();
        void setFetcher(Fetcher *f);
        void start();
    private:
        Fetcher *data;
        CLIUI *cli;
};

#endif
