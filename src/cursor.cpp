#ifndef JCORD_CURSOR
#define JCORD_CURSOR

#include "headers/cursor.h"

void setup_cursor(Cursor *curs, int server, int servers, int channel, int channels, int users, int msgs)
{
    curs->server = server;
    curs->maxserver = servers;
    curs->channel = channel;
    curs->maxchannel = channels;
    curs->highlighted = 0;
    curs->maxusers = users;
    curs->maxmsg = msgs;
    curs->focused = Tab::SERVERS;
    curs->current = "";
    curs->strind = 0;
    curs->inputmode = false;
}

void reset_cursor(Cursor *curs, int channels, int users, int msgs)
{
    curs->channel = 0;
    if(channels != -1) curs->maxchannel = channels;
    curs->highlighted = (curs->focused == Tab::SERVERS) ? curs->server : (curs->focused == Tab::CHANNELS) ? curs->channel : 0;
    if(users != -1) curs->maxusers = users;
    if(msgs != -1) curs->maxmsg = msgs;
    curs->current = "";
    curs->strind = 0;
}

Action send_message(Cursor *c, std::string mess)
{
    c->current = mess;
    return Action::SEND_MESSAGE;
}

Action interact(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS: {
            c->server = c->highlighted;
            return Action::CHANGE_SERVER;
        }
        case Tab::CHANNELS: {
            c->channel = c->highlighted;
            return c->channel == c->maxchannel ? Action::NEW_DM : Action::CHANGE_CHANNEL;
        }
        case Tab::TYPE:     return send_message(c, c->current);
        case Tab::USERS:    return c->server == -1 ? USER_DETAILS : MEMBER_DETAILS;
        case Tab::MESSAGES: return Action::MESSAGE_OPTIONS;
    }
}

Action focus_up(Cursor *c)
{
    switch(c->focused)
    {
        case Tab::SERVERS:
            if(c->highlighted >= 0)  c->highlighted--;
            break;
        case Tab::CHANNELS:
            if(c->highlighted > 0) c->highlighted--;
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
            if(c->highlighted  < c->maxserver - 1)  c->highlighted++;
            break;
        case Tab::CHANNELS:
            if(c->highlighted < c->maxchannel) c->highlighted++;
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
    c->highlighted = (t == Tab::SERVERS) ? c->server : (t == Tab::CHANNELS) ? c->channel : 0;
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
    c->highlighted = (c->focused == Tab::SERVERS) ? c->server : (c->focused == Tab::CHANNELS) ? c->channel : 0;
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
    c->highlighted = (c->focused == Tab::SERVERS) ? c->server : (c->focused == Tab::CHANNELS) ? c->channel : 0;
    return Action::NONE;
}

Action toggle_input(Cursor *c, bool enable)
{
    c->inputmode = enable;
    if(enable) { c->focused = Tab::TYPE; }
    return Action::NONE;
}

Action append(Cursor *c, char chr)
{
    if(c->strind == (int)c->current.length()) { c->current = c->current.append(1, chr); }
    else { c->current = c->current.insert(c->strind, 1, chr); }
    c->strind++;
    return Action::NONE;
}

Action remove(Cursor *c, bool del)
{
    if(c->strind == (int)c->current.length() &&  del) { return Action::NONE; } //delete     at   end
    if(c->strind == 0                        && !del) { return Action::NONE; } //backsp before begin
    c->current = c->current.erase(del ? c->strind : (c->strind - 1), 1);
    if(!del) { c->strind--; }
    return Action::NONE;
}

Action input_left(Cursor *c)
{
    if(c->strind > 0) c->strind--;
    return Action::NONE;
}

Action input_right(Cursor *c)
{
    if(c->strind < (int)c->current.length()) c->strind++;
    return Action::NONE;
}

Action input_up(Cursor *c)
{
    c->strind = 0;
    return Action::NONE;
}

Action input_down(Cursor *c)
{
    c->strind = c->current.length();
    return Action::NONE;
}

#endif
