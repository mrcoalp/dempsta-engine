#include <Core/entry.h>

#include "dempstaeditor.h"

class Game : public de::Application {
public:
    Game() { PushLayer(new DempstaEditor()); };

    ~Game() override = default;
};

de::Application* de::CreateApplication() { return new Game(); }
