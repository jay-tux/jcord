#ifndef JCORD_MAIN_CPP
#define JCORD_MAIN_CPP

#include "main.h"

// <editor-fold> Fetcher
void Fetcher::fetchServer()
{
    this->channels.clear();
    this->channelnames.clear();
    if(this->curserver == -1)
    {
        //get DMs
        std::vector<SleepyDiscord::Channel> data = this->discord->getDirectMessageChannels().vector();
        for(auto it = data.begin(); it != data.end(); it++)
        {
            this->channels.push_back(*it);
            this->channelnames.push_back("#" + (*it).name);
        }
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
    }
}

Fetcher::Fetcher(Window *w, Connector *conn)
{
    this->curserver = BOOT_SERVER;
    this->ui = w;
    this->discord = conn;
    this->servers = this->discord->getServers().vector();
    for(auto srv = this->servers.begin(); srv != this->servers.end(); srv++)
    {
        this->servernames.push_back((*srv).name);
    }
    fetchServer();
}

std::vector<SleepyDiscord::Server> *Fetcher::getServers() { return &this->servers; }
std::vector<std::string> *Fetcher::getServernames() { return &this->servernames; }
int Fetcher::getServerIndex() { return this->curserver; }

void Fetcher::start() { this->discord->run(); }
void Fetcher::stop() { this->discord->quit(); }

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

/*std::vector<SleepyDiscord::Server> Fetcher::getServers()
{
    return this->discord->getServers().vector();
}

std::vector<std::string> Fetcher::getServernames()
{
    std::vector<std::string> names;
    std::vector<SleepyDiscord::Server> srvrs = this->getServers();
    for(auto srv = srvrs.begin(); srv != srvrs.end(); srv++)
    {
        names.push_back((*srv).name);
    }
    return names;
}

std::vector<SleepyDiscord::Channel> Fetcher::getChannels(ServerFlake server)
{
    return this->discord->getServerChannels(server).vector();
}

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
    this->cli = new CLIUI(this->data->getServernames(),
        this->data->getChannelnames(this->data->getServerIndex()),
        this->data->getServerIndex());
}

void Window::start()
{
    while(1)
    {
        this->cli->render(this->data->getServerIndex());
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
