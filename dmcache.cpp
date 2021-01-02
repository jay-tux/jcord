#ifndef JCORD_DMCACHE
#define JCORD_DMCACHE

#include "dmcache.h"

std::string getFilePath()
{
#ifdef __unix__

#elif defined(_WIN32) || defined(_WIN64)
    //some windows file?
    return "";
#else
#error "Expecting either an Unix system (__unix__ macro defined) or a Windows system (_WIN32 or _WIN64 macro defined)"
#endif
}

DMCache::DMCache()
{
    //load cache from file
}

DMCache::~DMCache()
{
    //save cache to file
}

std::forward_list<ChannelFlake> &DMCache::getCache()
{
    return this->dmCache;
}

void DMCache::receiveDM(SleepyDiscord::Message msg)
{
    ChannelFlake src = msg.channelID;
    dmCache.remove(src);
    dmCache.push_front(src);
    int i = 16;
}

#endif
