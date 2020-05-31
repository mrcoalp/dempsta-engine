#include <Core/entry.h>
#include <Platform/OpenGL/openglshader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "layer2d.h"

class Game : public de::Application {
public:
    Game() { PushLayer(new Layer2D()); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
