#ifndef JCORD_POPUP
#define JCORD_POPUP

#include "headers/popup.h"

void ready_window(WINDOW *w)
{
    werase(w);
    box(w, 0, 0);
}

void get_window(WINDOW **w, int y0, int x0, int height, int width)
{
    *w = newwin(height, width, y0, x0);
    nodelay(*w, 1);
    keypad(*w, 1);
    ready_window(*w);
    wrefresh(*w);
}

Popup::Popup(std::string title, int ycenter, int xcenter, int h, int w)
{
    this->title = title;
    this->w = w;
    this->h = h;
    this->useinput = false;
    this->opts = nullptr;
    this->input = "";
    this->index = 0;
    get_window(&this->win, ycenter - h / 2, xcenter - w / 2, h, w);
    this->render();
}

void Popup::initialize_choices(std::vector<std::string> *choices)
{
    if(this->opts == nullptr && !useinput)
    {
        this->opts = choices;
    }
}

void Popup::initialize_input()
{
    if(this->opts == nullptr)
    {
        this->useinput = true;
    }
}

PopupAction Popup::act(PopupInput in, int key)
{
    switch(getMode())
    {
        case PopupMode::CHOICES:
            switch(in)
            {
                case PopupInput::MOVE_UP: {
                    if(this->index >0) this->index--;
                    break;
                };
                case PopupInput::MOVE_DOWN: {
                    if(this->index < (int)this->opts->size() - 1) this->index++;
                    break;
                };

                case PopupInput::EXIT: return PopupAction::QUIT;
                case PopupInput::ACTION_GENERAL: return PopupAction::RETURN;
                default: break; //stops GCC from yelling
            }

        case PopupMode::STRING:
            switch(in)
            {
                case PopupInput::MOVE_UP: {
                    this->index = 0;
                    break;
                }
                case PopupInput::MOVE_DOWN: {
                    this->index = input.length();
                    break;
                }
                case PopupInput::MOVE_LEFT: {
                    if(this->index > 0) this->index--;
                    break;
                }
                case PopupInput::MOVE_RIGHT: {
                    if(this->index < (int)input.length()) this->index++;
                    break;
                }
                case PopupInput::REMOVE: {
                    if(this->index != 0)
                    {
                        this->input = this->input.erase(this->index - 1);
                        this->index--;
                    }
                    break;
                }
                case PopupInput::DELETE: {
                    if(this->index != (int)input.length())
                    {
                        this->input = this->input.erase(this->index);
                    }
                    break;
                }
                case PopupInput::KEY: {
                    if(this->index == (int)this->input.length()) this->input = this->input.append(1, (char)key);
                    else this->input = this->input.insert(this->index, 1, (char)key);
                    this->index++;
                    break;
                }
                case PopupInput::ACTION_INPUT: return PopupAction::RETURN;
                case PopupInput::EXIT: return PopupAction::QUIT;
                default: break; //stops GCC from yelling
            }

            default: break; //stops GCC from yelling
    }
    return PopupAction::NOACTION;
}

PopupMode Popup::getMode()
{
    if(this->opts != nullptr) return PopupMode::CHOICES;
    if(this->useinput)        return PopupMode::STRING;
    return PopupMode::UNINIT;
}

void Popup::render()
{
    ready_window(this->win);
    mvwaddnstr(this->win, 0, 1, this->title.c_str(), this->w - 2);
    wrefresh(this->win);
}

Popup::~Popup()
{
    delwin(win);
}

#endif
