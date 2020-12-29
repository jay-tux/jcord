#ifndef JCORD_MAIN_CPP
#define JCORD_MAIN_CPP

#include "main.h"

// <editor-fold> Fetcher
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
    if(this->curserver == -1)
    {
        //do nothing cause there's nothing to do
    }
    else
    {
        //get server
        ServerFlake flake = this->servers[this->curserver].ID;
        std::vector<SleepyDiscord::Channel> data = this->discord->getServerChannels(flake).vector();
        for(auto it = data.begin(); it != data.end(); it++)
        {
            if((*it).type != TEXT_CHANNEL) { continue; }
            this->channels.push_back(*it);
            this->channelnames.push_back("#" + (*it).name);
        }

        std::vector<SleepyDiscord::ServerMember> mem = this->discord->listMembers(flake).vector();
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
    this->messages.clear();
    this->messagecnts.clear();

    SleepyDiscord::Channel chan = this->channels[this->curchan];
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

Fetcher::Fetcher(Window *w, Connector *conn)
{
    this->curserver = BOOT_SERVER;
    this->curchan = BOOT_CHAN;
    this->ui = w;
    this->discord = conn;
    this->servers = this->discord->getServers().vector();
    for(auto srv = this->servers.begin(); srv != this->servers.end(); srv++)
    {
        this->servernames.push_back((*srv).name);
    }
    fetchServer();
    fetchChannel();
}

std::vector<SleepyDiscord::Server> *Fetcher::getServers() { return &this->servers; }
std::vector<std::string> *Fetcher::getServernames() { return &this->servernames; }
int Fetcher::getServerIndex() { return this->curserver; }
int Fetcher::getChannelIndex() { return this->curchan; }

void Fetcher::start() { this->discord->run(); }
void Fetcher::stop() { this->discord->quit(); }

std::vector<std::string> *Fetcher::getChannelnames(int index)
{
    if(index == -1)
    {
        this->curserver = index;
        return &this->discord->DMnames;
    }

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

std::vector<std::string> *Fetcher::getMessages(int chan, int server)
{
    if(chan == this->curchan && server == this->curserver) { return &this->messagecnts; }

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
            return &this->messagecnts;
        }
    }

    return nullptr;
}

/*
std::vector<SleepyDiscord::Message> Fetcher::getMessages(SleepyDiscord::Channel chan, uint8_t cnt)
{
    return this->discord->getMessages(chan.ID, FETCHKEY, chan.lastMessageID, cnt).vector();
}

std::vector<SleepyDiscord::ServerMember> Fetcher::getMembers(ServerFlake server)
{
    return this->discord->listMembers(server).vector();
}*/
// </editor-fold>

// <editor-fold> Window
Window::Window()
{

}

Window::~Window()
{
    delete(this->cli);
}

void Window::setFetcher(Fetcher *f)
{
    this->data = f;
    int server = this->data->getServerIndex();
    int channel = this->data->getChannelIndex();
    this->cli = new CLIUI(this->data->getServernames(),
        this->data->getChannelnames(server),
        this->data->getMembernames(server),
        this->data->getMessages(server, channel),
        server, channel);
}

void Window::start()
{
    while(1)
    {
        this->cli->render();
        usleep(100000);
    }
}
// </editor-fold>

int main()
{
    Connector mainConn(getToken(), SleepyDiscord::USER_CONTROLED_THREADS);
    Window render;
    Fetcher listen(&render, &mainConn);
    render.setFetcher(&listen);
    std::thread listener(&Fetcher::start, listen);
    std::thread renderer(&Window::start, render);

    listener.join();
    renderer.join();
}

#endif
