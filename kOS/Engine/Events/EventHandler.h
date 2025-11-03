#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "EventBus.h"

namespace Event {

    // Singleton class that manages the global event bus
    class EventHandler {
    public:
        // Get the singleton instance
        static EventHandler* GetInstance() {
            static EventHandler instance;
            return &instance;
        }

        // Get reference to the event bus
        ecs::EventBus& GetBus() {
            return eventBus;
        }

        // Delete copy constructor and assignment operator
        EventHandler(const EventHandler&) = delete;
        EventHandler& operator=(const EventHandler&) = delete;

    private:
        // Private constructor for singleton
        EventHandler() = default;
        ~EventHandler() = default;

        // The main event bus
        ecs::EventBus eventBus;
    };

}

#endif // EVENTHANDLER_H
