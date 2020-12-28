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

#define FETCHKEY (SleepyDiscord::DiscordClient::GetMessagesKey::before)
#define TEXT_CHANNEL (SleepyDiscord::Channel::ChannelType::SERVER_TEXT)
#define BOOT_SERVER (0)
#define BOOT_CHAN 0
#define FETCH_COUNT 50

#define TYPE_DEFAULT (SleepyDiscord::Message::MessageType::DEFAULT)
#define TYPE_JOIN (SleepyDiscord::Message::MessageType::RECIPIENT_ADD)
#define TYPE_SJOIN (SleepyDiscord::Message::MessageType::GUILD_MEMBER_JOIN)
#define TYPE_PIN (SleepyDiscord::Message::MessageType::CHANNEL_PINNED_MESSAGE)
#define TYPE_QUIT (SleepyDiscord::Message::MessageType::RECIPIENT_REMOVE)

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
        std::vector<std::string> *getMembernames(int);
        std::vector<std::string> *getMessages(int, int);
        int getServerIndex();
        int getChannelIndex();
    protected:
        void fetchServer();
        void fetchChannel();
    private:
        std::string msgToString(SleepyDiscord::Message msg, bool isServer);
        Window *ui;
        Connector *discord;
        std::vector<SleepyDiscord::Server> servers;
        std::vector<std::string> servernames;
        std::vector<SleepyDiscord::Channel> channels;
        std::vector<std::string> channelnames;
        std::vector<SleepyDiscord::User> members;
        std::vector<std::string> membernames;
        std::vector<SleepyDiscord::Message> messages;
        std::vector<std::string> messagecnts;
        int curserver;
        int curchan;
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
