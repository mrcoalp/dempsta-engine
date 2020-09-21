#pragma once

#include <functional>
#include <vector>

#include "Scripting/API/databuffer.h"

namespace lua {
struct Message {
    std::string id;
    lua::DataBuffer* data;
    std::string sender;

    Message(std::string id, lua::DataBuffer* data, std::string sender)
        : id(std::move(id)), data(data), sender(std::move(sender)) {}
};

using MessageCallback = std::function<void(const Message&)>;

class MessageHandler {
public:
    static void AddMessage(const Message& msg);

    static void HandleMessages(MessageCallback&& callback);

    static void ClearMessages();

private:
    static std::vector<Message> s_messages;
};
}  // namespace lua
