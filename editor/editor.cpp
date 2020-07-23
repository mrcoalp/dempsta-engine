#include <Core/entry.h>

#include "dempstaeditor.h"

class Editor : public de::Application {
public:
    Editor() : de::Application({"Dempsta Editor", 1366, 768}) { PushLayer(new de::DempstaEditor()); };

    ~Editor() override = default;
};

de::Application* de::CreateApplication() { return new Editor(); }
