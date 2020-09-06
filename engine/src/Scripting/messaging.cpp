#include "Scripting/messaging.h"

namespace lua {
std::queue<Message> MessageHandler::s_messageQueue;

void MessageHandler::AddMessage(const Message& msg) { s_messageQueue.emplace(msg); }

void MessageHandler::HandleMessages(const std::function<void(const Message&)>& callback) {
    while (!s_messageQueue.empty()) {
        const auto& msg = s_messageQueue.front();
        callback(msg);
        // Ensure data buffer deletion
        delete msg.Data;
        s_messageQueue.pop();
    }
}
}  // namespace lua
