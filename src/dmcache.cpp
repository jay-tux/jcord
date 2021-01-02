#ifndef JCORD_DMCACHE
#define JCORD_DMCACHE

#include "headers/dmcache.h"

std::string getFilePath()
{
#ifdef __unix__
    std::string home = std::getenv("HOME");
    return home + "/.jcord.cache.dms";

#elif defined(_WIN32) || defined(_WIN64)
    std::string appdata = std::getenv("APPDATA");
    return appdata + "/.jcord.cache.dms";
    return "";

#else
#error "Expecting either an Unix system (__unix__ macro defined) or a Windows system (_WIN32 or _WIN64 macro defined)"
#endif
}

DMCache::DMCache()
{
    //load cache from file
    std::string path = getFilePath();
    std::ifstream load(getFilePath());
    if(load.good())
    {
        std::string curr;
        while(!load.eof())
        {
            std::getline(load, curr);
            if(curr != "") { dmCache.push_back(new ChannelFlake(curr)); }
        }
    }
}

void DMCache::finalize()
{
    //save cache to file
    if(removed) { return; }
    removed = true;
    std::ofstream save(getFilePath());
    if(!save.good()) { return; }

    while(!dmCache.empty())
    {
        std::string next = dmCache.front()->string();
        save << next << std::endl;
        dmCache.pop_front();
    }

    save.close();
}

_cache &DMCache::getCache()
{
    return this->dmCache;
}

void DMCache::receiveDM(SleepyDiscord::Message msg)
{
    ChannelFlake *src = new ChannelFlake(msg.channelID);
    std::remove(dmCache.begin(), dmCache.end(), src);
    dmCache.push_front(src);
}

#endif
