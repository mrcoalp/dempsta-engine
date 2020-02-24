#include "dempsta.h"

class ExampleLayer : public de::Layer {
public:
    explicit ExampleLayer(const std::string& name) : Layer(name) {}

    void OnUpdate() final {
        LOG_DEBUG("{0} update", GetName());
    }

    void OnEvent(de::Event& e) final {
        LOG_DEBUG("{0} {1}", GetName(), e.ToString());
    }
};

class Game : public de::Application {
public:
    Game() {
        PushLayer(new ExampleLayer("belo exemplo"));
    };

    ~Game() override = default;
};

de::Application* de::CreateApplication() {
    return new Game();
}
