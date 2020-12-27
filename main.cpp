#ifndef JCORD_MAIN_CPP
#define JCORD_MAIN_CPP

#include "main.h"

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

Window::Window()
{
    this->cli = new CLIUI();
}

Window::~Window()
{
    delete(this->cli);
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
