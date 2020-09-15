#pragma once

#include <functional>
#include <queue>

#include "Scripting/API/databuffer.h"

namespace lua {
struct Message {
    std::string id;
    lua::DataBuffer* data;
    std::string sender;

    Message(std::string id, lua::DataBuffer* data, std::string sender)
        : id(std::move(id)), data(data), sender(std::move(sender)) {}
};

class MessageHandler {
public:
    static void AddMessage(const Message& msg);

    static void HandleMessages(const std::function<void(const Message&)>& callback);

private:
    static std::queue<Message> s_messageQueue;
};
}  // namespace lua
