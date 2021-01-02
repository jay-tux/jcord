#ifndef JCORD_FETCHER
#define JCORD_FETCHER

#include "fetcher.h"

// <editor-fold> Convertors/Extractors
std::unique_ptr<SleepyDiscord::Channel> Connector::extrChannel(SleepyDiscord::Message m)
{
    try
    {
        SleepyDiscord::Channel c = this->getChannel(m.channelID).cast();
        return std::make_unique<SleepyDiscord::Channel>(c);
    }
    catch(SleepyDiscord::ErrorCode&)
    {
        return std::unique_ptr<SleepyDiscord::Channel>(nullptr);
    }
}

std::unique_ptr<SleepyDiscord::Server> Connector::extrServer(SleepyDiscord::Message m)
{
    try
    {
        SleepyDiscord::Server s = this->getServer(m.serverID).cast();
        return std::make_unique<SleepyDiscord::Server>(s);
    }
    catch(SleepyDiscord::ErrorCode&)
    {
        return std::unique_ptr<SleepyDiscord::Server>(nullptr);
    }
}

std::string Connector::lastMsg(SleepyDiscord::Channel ch)
{
    try
    {
        return this->getMessage(ch.ID, ch.lastMessageID).cast().content;
    }
    catch(SleepyDiscord::ErrorCode&)
    {
        return "(no message)";
    }
}
// </editor-fold>

void Connector::setFetcher(IFetcher *f)
{
    this->f = f;
}

void Connector::resolve(SleepyDiscord::Message m)
{
    if(this->f != nullptr) { f->onMessage(m); }
}

void Connector::boot(SleepyDiscord::Ready r)
{
    this->updateStatus("Make Jay Insane Again");
    for(auto it = r.privateChannels.begin(); it != r.privateChannels.end(); it++)
    {
        this->DMs.push_back(*it);
        this->DMnames.push_back("@" + (*it).name);
    }
    int i = this->DMs.size();
    if(this->f != nullptr) { f->onReady(r); }
}

#endif
