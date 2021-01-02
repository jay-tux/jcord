#ifndef JCORD_FETCHER_H
#define JCORD_FETCHER_H

#include "../../libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"

class IFetcher {
    public:
        virtual void onMessage(SleepyDiscord::Message) = 0;
        virtual void onReady(SleepyDiscord::Ready) = 0;
};

class Connector : public SleepyDiscord::DiscordClient {
    public:
        std::string token;
        using SleepyDiscord::DiscordClient::DiscordClient;
        void onMessage(SleepyDiscord::Message m) override { resolve(m); }
        void onReady(SleepyDiscord::Ready r) override { boot(r); }
        std::vector<SleepyDiscord::Channel> DMs;
        std::vector<std::string> DMnames;
        void setFetcher(IFetcher *);
    protected:
        std::unique_ptr<SleepyDiscord::Channel> extrChannel(SleepyDiscord::Message);
        std::unique_ptr<SleepyDiscord::Server>  extrServer (SleepyDiscord::Message);
        std::string lastMsg(SleepyDiscord::Channel);
    private:
        void resolve(SleepyDiscord::Message);
        void boot(SleepyDiscord::Ready);
        IFetcher *f = nullptr;
};

#endif
