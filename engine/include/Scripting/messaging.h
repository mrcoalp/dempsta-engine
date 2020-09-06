#pragma once

#include <queue>

#include "Scripting/API/databuffer.h"

namespace de {
struct Message {
    std::string ID;
    lua::DataBuffer* Data;
    std::string Sender;

    Message(std::string id, lua::DataBuffer* data, std::string sender)
        : ID(std::move(id)), Data(data), Sender(std::move(sender)) {}
};

class MessageHandler {
public:
    MessageHandler() = default;

    void AddMessage(const Message& msg) { m_messageQueue.emplace(msg); }

    void HandleMessages(std::function<void(const Message&)> callback) {
        while (!m_messageQueue.empty()) {
            const auto& msg = m_messageQueue.front();
            callback(msg);
            // Ensure data buffer deletion
            delete msg.Data;
            m_messageQueue.pop();
        }
    }

private:
    std::queue<Message> m_messageQueue;
};
}  // namespace de
