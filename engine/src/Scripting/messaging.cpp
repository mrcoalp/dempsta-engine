#include "Scripting/messaging.h"

namespace lua {
std::vector<Message> MessageHandler::s_messages;

void MessageHandler::AddMessage(const Message& msg) { s_messages.emplace_back(msg); }

void MessageHandler::HandleMessages(MessageCallback&& callback) {
    for (const auto& msg : s_messages) {
        callback(msg);
    }
}

void MessageHandler::ClearMessages() {
    for (const auto& msg : s_messages) {
        delete msg.data;
    }
    s_messages.clear();
}
}  // namespace lua
