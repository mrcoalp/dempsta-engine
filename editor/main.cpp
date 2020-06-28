#include <Core/entry.h>

#include "dempstaeditor.h"

class Editor : public de::Application {
public:
    Editor() { PushLayer(new DempstaEditor()); };

    ~Editor() override = default;
};

de::Application* de::CreateApplication() { return new Editor(); }
