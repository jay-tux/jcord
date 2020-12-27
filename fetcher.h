#ifndef JCORD_FETCHER_H
#define JCORD_FETCHER_H

#include "libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"

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

#endif
