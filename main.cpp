#ifndef JCORD_MAIN
#define JCORD_MAIN

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
    /*if(index == -1)
    {
        this->curserver = index;
        return &this->discord->DMnames;
    }*/

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
        return &this->messagecnts;
    }

    return nullptr;
}

bool Fetcher::sendMessage(std::string msg)
{
    SleepyDiscord::Message res = (SleepyDiscord::Message)this->discord->sendMessage(this->channels[this->curchan].ID, msg);
    return res.channelID.string() != "";
}
// </editor-fold>

// <editor-fold> IFetcher Events
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
        this->data->getMessages(server, channel, false),
        server, channel);
}

void Window::start()
{
    int newindex;
    std::vector<std::string> *ch, *us, *ms;
    while(1)
    {
        Action todo = this->cli->resolveBindings();
        if(todo == Action::STOP) { break; }
        //if(todo == Action::NONE || todo == Action::NO_RERENDER) { continue; }

        switch(todo)
        {
            case CHANGE_SERVER: {
                    newindex = this->cli->getCursor()->server;
                    ch = this->data->getChannelnames(newindex);
                    us = this->data->getMembernames(newindex);
                    ms = this->data->getMessages(0, newindex, true);
                    reset_cursor(this->cli->getCursor(), ch->size(), us->size(), ms->size());
                    break;
                }

            case CHANGE_CHANNEL: {
                    newindex = this->cli->getCursor()->channel;
                    ms = this->data->getMessages(newindex, this->cli->getCursor()->server, true);
                    this->cli->getCursor()->highlighted = 0;
                    this->cli->getCursor()->maxmsg = ms->size();
                    break;
                }

            case SEND_MESSAGE: {
                    this->data->sendMessage(this->cli->getCursor()->current);
                    this->cli->getCursor()->current = "";
                    this->cli->getCursor()->strind = 0;
                    break;
            }
            case LOAD_MORE_MESSAGES: { break; }
            default: { break; } //compiler warnings are annoying
        }

        if(todo != Action::NO_RERENDER || this->data->force_render())
        {
            this->cli->render();
        }
    }
    this->data->stop();
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
