#ifndef JCORD_DMCACHE_H
#define JCORD_DMCACHE_H

#include "libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"

typedef SleepyDiscord::Snowflake<SleepyDiscord::Channel> ChannelFlake;

class DMCache {
    public:
        DMCache();
        ~DMCache();
        std::forward_list<ChannelFlake> &getCache();
        void receiveDM(SleepyDiscord::Message);

    private:
        std::forward_list<ChannelFlake> dmCache;
};

#endif
