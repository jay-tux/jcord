#ifndef JCORD_MAIN_CPP
#define JCORD_MAIN_CPP

#include "main.h"

// <editor-fold> Fetcher
Fetcher::Fetcher(Window *w, Connector *conn)
{
    this->ui = w;
    this->discord = conn;
    this->servers = this->discord->getServers().vector();
    for(auto srv = this->servers.begin(); srv != this->servers.end(); srv++)
    {
        this->servernames.push_back((*srv).name);
    }
}

std::vector<SleepyDiscord::Server> *Fetcher::getServers() { return &this->servers; }
std::vector<std::string> *Fetcher::getServernames() { return &this->servernames; }

void Fetcher::start()
{
    this->discord->run();
}

void Fetcher::stop()
{
    this->discord->quit();
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
    this->cli = new CLIUI(this->data->getServernames());
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
