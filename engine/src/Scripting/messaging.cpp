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
    for (auto& msg : s_messages) {  // Unload table refs
        msg.data.Unload();
    }
    s_messages.clear();
}
}  // namespace lua
