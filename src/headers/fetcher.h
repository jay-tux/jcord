#ifndef JCORD_FETCHER_H
#define JCORD_FETCHER_H

#define TYPE_DEFAULT (SleepyDiscord::Message::MessageType::DEFAULT)
#define TYPE_JOIN (SleepyDiscord::Message::MessageType::RECIPIENT_ADD)
#define TYPE_SJOIN (SleepyDiscord::Message::MessageType::GUILD_MEMBER_JOIN)
#define TYPE_PIN (SleepyDiscord::Message::MessageType::CHANNEL_PINNED_MESSAGE)
#define TYPE_QUIT (SleepyDiscord::Message::MessageType::RECIPIENT_REMOVE)

#define FETCHKEY (SleepyDiscord::DiscordClient::GetMessagesKey::before)
#define TEXT_CHANNEL (SleepyDiscord::Channel::ChannelType::SERVER_TEXT)
#define BOOT_SERVER (0)
#define BOOT_CHAN 0
#define FETCH_COUNT 50

#include <string>
#include <vector>
#include "connector.h"
#include "dmcache.h"
#include "../../libs/sleepy-discord/include/sleepy_discord/sleepy_discord.h"

class Window;

class Fetcher : public IFetcher { //MVC: MODEL
    public:
        Fetcher(Window *w, Connector *conn);
        void start();
        void stop();
        std::vector<SleepyDiscord::Server> *getServers();
        std::vector<std::string> *getServernames();
        std::vector<std::string> *getChannelnames(int);
        std::vector<std::string> *getMembernames(int);
        std::vector<std::string> *getMessages(int, int, bool);
        int getServerIndex();
        int getChannelIndex();
        bool sendMessage(std::string);
        bool force_render();
        std::vector<SleepyDiscord::Server> servers;
        bool createDM(std::string);

        void onMessage(SleepyDiscord::Message);
        void onReady(SleepyDiscord::Ready);
        ~Fetcher();
    protected:
        void fetchServer();
        void fetchChannel();
        void fetchRecipients(int);
        bool ready;
    private:
        std::string msgToString(SleepyDiscord::Message, bool);
        Window *ui;
        Connector *discord;
        DMCache *dms;
        std::vector<SleepyDiscord::Channel> channels;
        std::vector<SleepyDiscord::User> members;
        std::vector<SleepyDiscord::Message> messages;
        std::vector<std::string> servernames;
        std::vector<std::string> channelnames;
        std::vector<std::string> membernames;
        std::vector<std::string> messagecnts;
        int curserver;
        int curchan;
        bool rerender;
};


#endif
