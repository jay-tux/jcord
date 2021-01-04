#ifndef JCORD_POPUP
#define JCORD_POPUP

#include "headers/popup.h"

std::string toLength(std::string start, int len, bool centered)
{
    int add = len - start.length();
    if(add < 0)
    {
        return start.substr(0, len - 3) + "...";
    }
    else
    {
        std::stringstream res;
        int left = add;
        if(centered)
        {
            left = add / 2;
            for(int i = 0; i < left; i++) { res << " "; }
            if(add % 2 != 0) { left++; }
        }
        res << start;
        for(int i = 0; i < left; i++) { res << " "; }
        return res.str();
    }
}

std::string lastn(std::string str, int center, int amount, int *offset)
{
    int len = str.length();
    if(len <= amount)
    {
        *offset = 0;
        return str.append(amount - (int)str.length(), ' ');
    }

    if(center - amount / 2 <= 0)
    {
        *offset = 0;
        return str.substr(0, amount);
    }

    if(center + amount / 2 >= len)
    {
        *offset = len - amount;
        return str.substr(len - amount, amount);
    }

    *offset = center - amount / 2;
    return str.substr(center - amount / 2, center + amount / 2);
}

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
    this->cleaned = false;
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
                case PopupInput::POPUP_ACTION: return PopupAction::RETURN;
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
                case PopupInput::PASTE: {
                    try
                    {
                        CClipboardXX clip;
                        std::string inputdata;
                        clip >> inputdata;
                        if(this->index == (int)this->input.length()) this->input = this->input.append(inputdata);
                        else this->input = this->input.insert(this->index, inputdata);
                        this->index += inputdata.length();
                    }
                    catch(CExceptionXX &e) {} //do nothing if paste fails /shrug
                    break;
                }
                case PopupInput::POPUP_ACTION: return PopupAction::RETURN;
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
    switch(getMode())
    {
        case PopupMode::STRING: {
                std::string toWrite = toLength(this->input, (this->h - 2) * (this->w - 2), true);
                int y = 1; int x = 1;
                for(auto chr = toWrite.begin(); chr != toWrite.end(); chr++)
                {
                    mvwaddch(this->win, y, x, *chr);
                    x++;
                    if(x >= this->w - 1) { x = 1; y++; }
                }
                break;
            }

        case PopupMode::CHOICES: break;
        case PopupMode::UNINIT: break;
    }
    wrefresh(this->win);
}

void Popup::close()
{
    if(cleaned) return;
    this->cleaned = true;
    delwin(this->win);
}

std::string Popup::getString() { return this->input; }
std::string Popup::getOption() { return (*this->opts)[this->index]; }

#endif
