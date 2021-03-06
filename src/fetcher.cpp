#ifndef JCORD_CONNECTOR
#define JCORD_CONNECTOR

#include "headers/fetcher.h"

std::string Fetcher::msgToString(SleepyDiscord::Message msg, bool isServer)
{
    std::string user;
    if(isServer)
    {
        auto id = msg.author.ID;
        auto iterator = this->servers[this->curserver].findMember(id);
        try
        {
            SleepyDiscord::ServerMember mem = *iterator;
            user = (mem.nick == "") ? msg.author.username : mem.nick;
        }
        catch(std::logic_error&)
        {
            user = msg.author.username;
        }
    }
    else
    {
        user = msg.author.username;
    }

    if(msg.type == TYPE_DEFAULT)
    {
        return "@" + user + ": " + msg.content + (msg.embeds.size() != 0 ?
            ("<" + std::to_string(msg.embeds.size()) + " embed(s)>") : "");
    }
    else if(msg.type == TYPE_JOIN || msg.type == TYPE_SJOIN)
    {
        return "JOIN> @" + user;
    }
    else if(msg.type == TYPE_QUIT)
    {
        return "QUIT> @" + user;
    }
    else if(msg.type == TYPE_PIN)
    {
        return "PIN BY> @" + user;
    }
    else
    {
        return "";
    }
}

void Fetcher::fetchServer()
{
    this->channels.clear();
    this->channelnames.clear();
    this->members.clear();
    this->membernames.clear();
    if(!this->ready) { return; } //wait till ready...
    if(this->curserver == -1)
    {
        //fetch DM cache
        auto vec = this->dms->getCache();
        for(auto dm = vec.begin(); dm != vec.end(); dm++)
        {
            SleepyDiscord::Channel fetched = (SleepyDiscord::Channel)this->discord->getChannel(**dm);
            this->channels.push_back(fetched);
            this->channelnames.push_back("@" + fetched.recipients[0].username);
        }
    }
    else
    {
        //get server
        SleepyDiscord::Server s = this->servers[this->curserver];
        std::vector<SleepyDiscord::Channel> data = this->discord->getServerChannels(s.ID).vector();
        SleepyDiscord::Snowflake<SleepyDiscord::User> usr = this->discord->getID();
        SleepyDiscord::ServerMember us = (SleepyDiscord::ServerMember)this->discord->getMember(s, usr);

        for(auto it = data.begin(); it != data.end(); it++)
        {
            if((*it).type != TEXT_CHANNEL) { continue; }
            SleepyDiscord::Permission perms = SleepyDiscord::getPermissions(s, us, *it);
            SleepyDiscord::Permission allow = perms & SleepyDiscord::Permission::VIEW_CHANNEL;
            if(allow)
            {
                this->channels.push_back(*it);
                this->channelnames.push_back("#" + (*it).name);
            }
        }

        std::vector<SleepyDiscord::ServerMember> mem = this->discord->listMembers(s.ID).vector();
        for(auto it = mem.begin(); it != mem.end(); it++)
        {
            SleepyDiscord::User u = (*it).user;
            this->members.push_back(u);
            this->membernames.push_back("@" + (*it).nick + (u.bot ? "[BOT]" : ""));
        }
    }
}

void Fetcher::fetchChannel()
{
    if(!this->ready) { return; } //wait till ready...
    this->messages.clear();
    this->messagecnts.clear();

    SleepyDiscord::Channel chan = this->channels[this->curchan];
    try
    {
        std::vector<SleepyDiscord::Message> msgs = this->discord->getMessages(chan.ID, FETCHKEY, chan.lastMessageID, FETCH_COUNT).vector();
        SleepyDiscord::Message last = this->discord->getMessage(chan.ID, chan.lastMessageID);
        this->messages.push_back(last);
        //this->messagecnts.push_back("@" + last.author.username + ": " + last.content);
        this->messagecnts.push_back(msgToString(last, this->curserver != -1));
        for(auto it = msgs.begin(); it != msgs.end(); it++)
        {
            this->messages.push_back(*it);
            //this->messagecnts.push_back("@" + (*it).author.username + ": " + (*it).content);
            this->messagecnts.push_back(msgToString(*it, this->curserver != -1));
        }
    }
    catch(SleepyDiscord::ErrorCode) {} //empty message history/message history can't be read/whatever weird nonsense is happening
}

Fetcher::Fetcher(Window *w, Connector *conn)
{
    this->dms = new DMCache();
    this->curserver = BOOT_SERVER;
    this->curchan = BOOT_CHAN;
    this->ui = w;
    this->discord = conn;
    this->discord->setFetcher(this);
    this->rerender = false;
}

Fetcher::~Fetcher()
{
    //delete this->dms;
    //we don't delete cause that would cause segfaults (in ~std::deque), somehow...
    //this doesn't seem to cause memory leaks, as far as valgrind tells me ¯\_(ツ)_/¯
}

std::vector<SleepyDiscord::Server> *Fetcher::getServers() { return &this->servers; }
std::vector<std::string> *Fetcher::getServernames() { return &this->servernames; }
int Fetcher::getServerIndex() { return this->curserver; }
int Fetcher::getChannelIndex() { return this->curchan; }

void Fetcher::start() { this->discord->run(); }

void Fetcher::stop()
{
    this->dms->finalize();
    this->discord->quit();
}

std::vector<std::string> *Fetcher::getChannelnames(int index)
{
    if(index == this->curserver) { return &this->channelnames; }

    if(index < (int)this->servers.size() && index >= -1)
    {
        this->curserver = index;
        fetchServer();
        return &this->channelnames;
    }

    return nullptr;
}

std::vector<std::string> *Fetcher::getMembernames(int index)
{
    if(index == this->curserver) { return &this->membernames; }

    if(index < (int)this->servers.size() && index >= -1)
    {
        this->curserver = index;
        fetchServer();
        return &this->membernames;
    }

    return nullptr;
}

void Fetcher::fetchRecipients(int dmchannel)
{
    if(this->curserver == -1)
    {
        SleepyDiscord::Channel chan = this->discord->getChannel(this->channels[dmchannel]);
        this->members.clear();
        this->membernames.clear();
        for(auto it = chan.recipients.begin(); it != chan.recipients.end(); it++)
        {
            SleepyDiscord::User u = *it;
            this->members.push_back(u);
            this->membernames.push_back("@" + u.username + "#" + u.discriminator);
        }
    }
}

bool Fetcher::force_render()
{
    if(this->rerender)
    {
        this->rerender = false;
        return true;
    }
    return false;
}

std::vector<std::string> *Fetcher::getMessages(int chan, int server, bool override)
{
    if(chan == this->curchan && server == this->curserver && !override) { return &this->messagecnts; }

    if(server < (int)this->servers.size() && server >= -1)
    {
        if(this->curserver != server)
        {
            this->curserver = server;
            fetchServer();
        }

        if(chan < (int)this->channels.size() && chan >= 0)
        {
            this->curchan = chan;
            fetchChannel();
        }
        else
        {
            this->messagecnts.clear();
            this->messages.clear();
        }
        if(this->curserver == -1) { fetchRecipients(chan); }
        return &this->messagecnts;
    }

    return nullptr;
}

bool Fetcher::sendMessage(std::string msg)
{
    try
    {
        SleepyDiscord::Message res = (SleepyDiscord::Message)this->discord->sendMessage(this->channels[this->curchan].ID, msg);
        return res.channelID.string() != "";
    }
    catch(SleepyDiscord::ErrorCode &e)
    {
        return false;
    }

}

bool Fetcher::createDM(std::string uid)
{
    try
    {
        SleepyDiscord::Channel dm = (SleepyDiscord::Channel)this->discord->createDirectMessageChannel(uid);
        this->dms->push_dm(dm.ID);
        fetchServer();
        return true;
    }
    catch(SleepyDiscord::ErrorCode &)
    {
        return false;
    }
}

void Fetcher::onMessage(SleepyDiscord::Message msg)
{
    if(msg.channelID == this->channels[curchan].ID && (msg.serverID == this->servers[curserver].ID ||
        (msg.serverID == "" && curserver == -1)))
    {
        this->messages.insert(this->messages.begin(), msg);
        this->messagecnts.insert(this->messagecnts.begin(), this->msgToString(msg, this->curserver != -1));
        this->rerender = true;
    }
    else if(msg.serverID == "") //dm!
    {
        this->dms->receiveDM(msg);
    }
}

void Fetcher::onReady(SleepyDiscord::Ready r) {
    this->ready = true;
    std::vector<SleepyDiscord::Server> tmp = this->discord->getServers().vector();
    for(auto srv = tmp.begin(); srv != tmp.end(); srv++)
    {
        SleepyDiscord::Server actual = (SleepyDiscord::Server)this->discord->getServer((*srv).ID);
        this->servers.push_back(actual);
        this->servernames.push_back(actual.name);
    }
    this->fetchServer();
    this->fetchChannel();
    this->rerender = true;
}

#endif
