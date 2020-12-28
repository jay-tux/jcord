#ifndef JCORD_FETCHER
#define JCORD_FETCHER

#include "fetcher.h"
//included here to avoid conflicts
//#include <ncurses.h>
//#include <locale.h>

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


void Connector::resolve(SleepyDiscord::Message m)
{
    auto channel = this->extrChannel(m);
    auto server  = this->extrServer (m);
    std::cout << "Message in"
                << " #" << ((!channel || channel->name == "") ? "(no channel)" : channel->name)
                << " (" << ((!server  || server->name == "")  ? "(no server)"  : server->name)
                << ") by " << m.author.username << ": "
                << m.content << std::endl;
    std::cout << "? ";
    std::string msg;
    std::getline(std::cin, msg);
    if(msg != "") this->sendMessage(m.channelID, msg);
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
    return;
}

#endif
