#pragma once

#include <iostream>
#include "log.h"

namespace de {
    class Application {
    public:
        Application();

        virtual ~Application();

        void Run();
    };
}  // namespace de
