#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

namespace ecs {
    // Keyboard events
    struct KeyPressedEvent { int keyCode; bool isRepeat; };
    struct KeyReleasedEvent { int keyCode; };

    // Mouse events
    struct MouseButtonPressedEvent { int button; float x, y; };
    struct MouseButtonReleasedEvent { int button; float x, y; };
    struct MouseMovedEvent { float x, y; float deltaX, deltaY; };
    struct MouseScrolledEvent { float xOffset, yOffset; };

    // Window events
    struct WindowResizedEvent { int width, height; };
    struct WindowClosedEvent {};

    // NEW: Just add more here as needed!
    struct WindowMinimizedEvent {};
    struct WindowMaximizedEvent {};
}

#endif