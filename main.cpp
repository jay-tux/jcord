#ifndef JCORD_MAIN_CPP
#define JCORD_MAIN_CPP

#include "main.h"

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

// <editor-fold> Window/Fetcher
Fetcher::Fetcher(Window *w, Connector *conn)
{
    this->ui = w;
    this->discord = conn;
}

void Fetcher::start()
{
    this->discord->run();
}

void Fetcher::stop()
{
    this->discord->quit();
}

void Window::setFetcher(Fetcher *f)
{
    this->data = f;
}

void Window::start()
{
    std::cout << "WINDOW RUNNING" << std::endl;
}
// </editor-fold>

// <editor-fold> Connector
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

void Connector::boot()
{
    this->updateStatus("Make Jay Insane Again");

    std::cout << "=== SERVER LIST ===" << std::endl;
    auto servers = this->getServers().vector();
    for(auto it = servers.begin(); it != servers.end(); it++)
    {
        auto channels = this->getServerChannels(it->ID).vector();
        std::cout << it->name << "(" << channels.size() << " channels): " << std::endl;
        for(auto ch = channels.begin(); ch != channels.end(); ch++)
        {
            std::cout << "\t#" << ch->name << ": " << this->lastMsg(*ch) << std::endl;
        }
    }
    std::cout << "=== END OF SERVER LIST ===" << std::endl;

    std::cout << "=== DIRECT MESSAGES LIST ===" << std::endl;
    auto dms = this->getDirectMessageChannels().vector();
    for(auto dm = dms.begin(); dm != dms.end(); dm++)
    {
        std::cout << dm->name << std::endl;
    }
    std::cout << "=== END OF DMS LIST ===" << std::endl;
}
// </editor-fold>

int main()
{
    Connector mainConn(getToken(), SleepyDiscord::USER_CONTROLED_THREADS);
    Window render;
    Fetcher listen(&render, &mainConn);
    render.setFetcher(&listen);
    //std::thread listener([](Fetcher &listen) { listen.start(); }, listen);
    //std::thread ui([](Window &renderer) { renderer.start(); }, renderer);
    //mainConn.run();
    std::thread listener(&Fetcher::start, listen);
    std::thread renderer(&Window::start, render);

    listener.join();
    renderer.join();
}

#endif
