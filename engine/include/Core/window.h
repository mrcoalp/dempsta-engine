#pragma once

#include <GLFW/glfw3.h>
#include "Core/pch.h"
#include "Core/log.h"
#include "Events/event.h"

namespace de {
    /**
     * @brief Contains relevant window properties.
     */
    struct WindowProps {
        std::string title;
        unsigned int width;
        unsigned int height;

        explicit WindowProps(
                std::string title = "Default Dempsta Title",
                unsigned int width = 1280,
                unsigned int height = 720
        ) : title(std::move(title)), width(width), height(height) {}
    };

    class Window {
    public:
        /**
         * @brief Event callback function is a void function that receives an event.
         */
        using EventCallbackFn = std::function<void(Event & )>;

        explicit Window(const WindowProps& windowProps);

        ~Window();

        /**
         * @brief Called once per frame. Checks for window updates.
         */
        void OnUpdate();

        /**
         * @brief Getter for the window width.
         * @return Window width.
         */
        [[nodiscard]] inline unsigned int GetWidth() const {
            return data.height;
        }

        /**
          * @brief Getter for the window height.
          * @return Window height.
          */
        [[nodiscard]] inline unsigned int GetHeight() const {
            return data.height;
        }

        /**
         * @brief Event callback function setter in WindowData.
         * @param callback Function to define callback.
         */
        inline void SetEventCallback(const EventCallbackFn& callback) {
            data.EventCallback = callback;
        }

        /**
         * @brief VSync attribute setter
         * @param enabled Whether or not to enable vsync.
         */
        void SetVSync(bool enabled);

        /**
         * @brief Getter for vsync.
         * @return
         */
        [[nodiscard]] bool IsVSync() const;

        /**
         * @brief Getter for glfw window pointer.
         * @return
         */
        [[nodiscard]] inline void* GetNativeWindow() const {
            return window;
        }

    private:
        void Shutdown();

    private:
        GLFWwindow* window;

        struct WindowData {
            std::string title;
            unsigned int width;
            unsigned int height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData data;
    };
}
