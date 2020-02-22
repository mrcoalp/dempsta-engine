#include "dempsta.h"

class Game : public de::Application {
public:
    Game() = default;;

    ~Game() override = default;;
};

int main() {
    auto game = new Game();
    game->Run();
    delete game;

    return 0;
}
