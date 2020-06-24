#include <Core/entry.h>
#include <Platform/OpenGL/openglshader.h>

#include <glm/gtc/matrix_transform.hpp>

#include "dempstaeditor.h"

class Game : public de::Application {
public:
    Game() { PushLayer(new DempstaEditor()); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
