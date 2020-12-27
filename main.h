#ifndef JCORD_MAIN_H
#define JCORD_MAIN_H

#include "libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"
#include "token.h"
#include "ui.h"
#include <iostream>
#include <thread>

class Connector : public SleepyDiscord::DiscordClient {
    public:
        std::string token;
        using SleepyDiscord::DiscordClient::DiscordClient;
        void onMessage(SleepyDiscord::Message m) override { resolve(m); }
        void onReady(SleepyDiscord::Ready r) override { boot(); }
    protected:
        std::unique_ptr<SleepyDiscord::Channel> extrChannel(SleepyDiscord::Message);
        std::unique_ptr<SleepyDiscord::Server>  extrServer (SleepyDiscord::Message);
        std::string lastMsg(SleepyDiscord::Channel);
    private:
        void resolve(SleepyDiscord::Message m);
        void boot();
};

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
