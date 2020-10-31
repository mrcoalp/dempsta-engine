#pragma once

#include <functional>
#include <vector>

#include "Scripting/API/databuffer.h"

namespace lua {
struct Message {
    std::string id;
    std::string sender;
    lua::LuaDynamicMap data;

    Message(std::string id, std::string sender) : id(std::move(id)), sender(std::move(sender)) {}
    Message(std::string id, std::string sender, const lua::LuaDynamicMap& data) : id(std::move(id)), sender(std::move(sender)), data(data) {}
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
