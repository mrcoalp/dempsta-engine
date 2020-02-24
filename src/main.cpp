#include "dempsta.h"

class Game : public de::Application {
public:
    Game() = default;

    ~Game() override = default;
};

de::Application* de::CreateApplication() {
    return new Game();
}
