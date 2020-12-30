#ifndef JCORD_CURSOR
#define JCORD_CURSOR

#include "cursor.h"

Action send_message(Cursor *c, std::string mess)
{
    c->current = mess;
    return Action::SEND_MESSAGE;
}

Action interact(Cursor *c)
{
    if(c->focused == Tab::TYPE)             return send_message(c, c->current);
    else if(c->focused == Tab::SERVERS)     return Action::CHANGE_SERVER;
    else if(c->focused == Tab::CHANNELS)    return Action::CHANGE_CHANNEL;
    else if(c->focused == Tab::MESSAGES)    return Action::NONE;            // TODO: interact w messages
    else                                    return Action::NONE;            //TODO: interact w users
}

Action focus_up(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS:
            if(c->server > 0)  c->server--;
            break;
        case Tab::CHANNELS:
            if(c->channel > 0) c->channel--;
            break;
        case Tab::TYPE:
            c->strind = 0;
            break;
        case Tab::MESSAGES:
            if(c->channel < c->maxmsg - 1) c->highlighted++; //oldest messages stored as last
            break;
        case Tab::USERS:
            if(c->highlighted > 0) c->highlighted--;
            break;
    }
    return Action::NONE;
}

Action focus_down(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS:
            if(c->server  < c->maxserver - 1)  c->server++;
            break;
        case Tab::CHANNELS:
            if(c->channel < c->maxchannel - 1) c->channel++;
            break;
        case Tab::TYPE:
            c->strind = c->current.length();
            break;
        case Tab::MESSAGES:
            if(c->highlighted > 0) c->highlighted--;
            else return Action::LOAD_MORE_MESSAGES;
            break;
        case Tab::USERS:
            if(c->highlighted < c->maxusers - 1) c->highlighted++;
            break;
    }
    return Action::NONE;
}

Action focus_direct(Cursor *c, Tab t)
{
    c->focused = t;
    c->highlighted = 0;
    return Action::NONE;
}

Action focus_left(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS:  c->focused = Tab::SERVERS;  break;
        case Tab::CHANNELS: c->focused = Tab::SERVERS;  break;
        case Tab::TYPE:
        case Tab::MESSAGES: c->focused = Tab::CHANNELS; break;
        case Tab::USERS:    c->focused = Tab::MESSAGES; break;
    }
    c->highlighted = 0;
    return Action::NONE;
}

Action focus_right(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS:  c->focused = Tab::CHANNELS; break;
        case Tab::CHANNELS: c->focused = Tab::MESSAGES; break;
        case Tab::TYPE:
        case Tab::MESSAGES: c->focused = Tab::USERS;    break;
        case Tab::USERS:    c->focused = Tab::USERS;    break;
    }
    c->highlighted = 0;
    return Action::NONE;
}

Action toggle_input(Cursor *c, bool enable)
{
    c->inputmode = enable;
    if(enable) { c->focused = Tab::TYPE; }
    return Action::NONE;
}

#endif
