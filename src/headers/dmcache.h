#ifndef JCORD_DMCACHE_H
#define JCORD_DMCACHE_H

#include "../../libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <deque>

typedef SleepyDiscord::Snowflake<SleepyDiscord::Channel> ChannelFlake;
typedef std::deque<ChannelFlake *> _cache;

class DMCache {
    public:
        DMCache();
        _cache &getCache();
        void receiveDM(SleepyDiscord::Message);
        void finalize();
        void push_dm(ChannelFlake);

    private:
        _cache dmCache;
        bool removed = false;
};

#endif
