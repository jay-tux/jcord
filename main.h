#ifndef JCORD_MAIN_H
#define JCORD_MAIN_H

#include "libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"
#include "token.h"
#include "ui.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <functional>
#include <cctype>

#include "fetcher.h"

#define FETCHKEY (SleepyDiscord::DiscordClient::GetMessagesKey::na)
#define TEXT_CHANNEL (SleepyDiscord::Channel::ChannelType::SERVER_TEXT)
#define BOOT_SERVER (0)

typedef SleepyDiscord::Snowflake<SleepyDiscord::Server> ServerFlake;

class Window;

class Fetcher {
    public:
        Fetcher(Window *w, Connector *conn);
        void start();
        void stop();
        std::vector<SleepyDiscord::Server> *getServers();
        std::vector<std::string> *getServernames();
        std::vector<std::string> *getChannelnames(int);
        int getServerIndex();
        //std::vector<SleepyDiscord::Channel> *getChannels(ServerFlake);
        //std::vector<SleepyDiscord::Message> *getMessages(SleepyDiscord::Channel, uint8_t);
        //std::vector<SleepyDiscord::ServerMember> *getMembers(ServerFlake);
    protected:
        void fetchServer();
    private:
        Window *ui;
        Connector *discord;
        std::vector<SleepyDiscord::Server> servers;
        std::vector<std::string> servernames;
        std::vector<SleepyDiscord::Channel> channels;
        std::vector<std::string> channelnames;
        std::vector<SleepyDiscord::ServerMember> members;
        int curserver;
};

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
};

#endif
