#ifndef JCORD_MAIN
#define JCORD_MAIN

#include "headers/main.h"

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
