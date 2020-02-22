#include "dempsta.h"

class Game : public de::Application {
public:
    Game() = default;

    ~Game() override = default;
};

de::Application* CreateApplication() {
    return new Game();
}
