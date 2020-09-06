#pragma once

#include <functional>
#include <queue>

#include "Scripting/API/databuffer.h"

namespace lua {
struct Message {
    std::string ID;
    lua::DataBuffer* Data;
    std::string Sender;

    Message(std::string id, lua::DataBuffer* data, std::string sender)
        : ID(std::move(id)), Data(data), Sender(std::move(sender)) {}
};

class MessageHandler {
public:
    static void AddMessage(const Message& msg);

    static void HandleMessages(const std::function<void(const Message&)>& callback);

private:
    static std::queue<Message> s_messageQueue;
};
}  // namespace lua
