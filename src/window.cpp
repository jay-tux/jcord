#ifndef JCORD_WINDOW
#define JCORD_WINDOW

#include "headers/window.h"

Window::Window()
{
    this->popup = PopupType::NO_POPUP;
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

            case NEW_DM: {
                    Popup *popup = this->cli->createPopup("Enter User ID", 50, 5);
                    popup->initialize_input();
                    this->popup = DM_POPUP;
                    break;
                }

            case USER_DETAILS: {
                    break; //nothing useful to do
                }

            case MEMBER_DETAILS: {
                    break;
                }

            case POPUP_QUIT: {
                    switch(this->popup)
                    {
                        case DM_POPUP: {
                            std::string uid = this->cli->getPopupResult();
                            this->data->createDM(uid);
                            this->cli->exitPopup();
                            break;
                        }

                        case USER_POPUP: break;
                        case MEMBER_POPUP: break;
                        case MESSAGE_POPUP: break;
                        case NO_POPUP: break;
                    }
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

#endif
