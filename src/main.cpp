#include <Core/entry.h>
#include <Platform/OpenGL/openglshader.h>

#include <glm/gtc/matrix_transform.hpp>

#include "gamelayer.h"

class Game : public de::Application {
public:
    Game() { PushLayer(new GameLayer()); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
