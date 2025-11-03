#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "System.h"
#include "Events/EventHandler.h"
#include "Events/InputEvents.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>

namespace ecs {

    // ANSI Color codes for event categories
    namespace EventColor {
        const std::string ORANGE = "\033[38;5;208m";  // Orange - INPUT
        const std::string CYAN = "\033[38;5;51m";   // Cyan - GAMEPLAY
        const std::string MAGENTA = "\033[38;5;201m";  // Magenta - PHYSICS
        const std::string BLUE = "\033[38;5;39m";   // Blue - UI
        const std::string PURPLE = "\033[38;5;135m";  // Purple - AUDIO
        const std::string PINK = "\033[38;5;213m";  // Pink - NETWORK
        const std::string LIME = "\033[38;5;154m";  // Lime - SYSTEM
        const std::string TEAL = "\033[38;5;80m";   // Teal - DEBUG
        const std::string RESET = "\033[0m";         // Reset
    }

    class EventSystem : public ISystem {
    public:
        // Event categories for logging
        enum class LogCategory {
            INPUT,      // Orange   - Keyboard, mouse, window events
            GAMEPLAY,   // Cyan     - Player actions, game logic
            PHYSICS,    // Magenta  - Collisions, physics interactions
            UI,         // Blue     - Button clicks, menu events
            AUDIO,      // Purple   - Sound effects, music
            NETWORK,    // Pink     - Multiplayer, network messages
            SYSTEM,     // Lime     - Engine systems, resource loading
            DEBUG       // Teal     - Debug/development events
        };

        // Logging controls
        bool loggingEnabled = true;
        bool useColors = true;

        void Init() override;
        void Update() override;

        // Category control
        void EnableLoggingForCategory(LogCategory cat) { categoryEnabled[cat] = true; }
        void DisableLoggingForCategory(LogCategory cat) { categoryEnabled[cat] = false; }

        // Blacklist specific event types from logging
        template<typename T>
        void BlacklistEventFromLog() {
            blacklistedEvents.insert(std::type_index(typeid(T)));
        }

        REFLECTABLE(EventSystem)

    private:
        Event::EventHandler* eventHandler = nullptr;

        std::unordered_map<LogCategory, bool> categoryEnabled = {
            {LogCategory::INPUT,    true},
            {LogCategory::GAMEPLAY, true},
            {LogCategory::PHYSICS,  true},
            {LogCategory::UI,       true},
            {LogCategory::AUDIO,    true},
            {LogCategory::NETWORK,  true},
            {LogCategory::SYSTEM,   true},
            {LogCategory::DEBUG,    true}
        };

        std::unordered_set<std::type_index> blacklistedEvents;

        std::string GetCategoryColor(LogCategory cat) const {
            if (!useColors) return "";

            switch (cat) {
            case LogCategory::INPUT:    return EventColor::ORANGE;
            case LogCategory::GAMEPLAY: return EventColor::CYAN;
            case LogCategory::PHYSICS:  return EventColor::MAGENTA;
            case LogCategory::UI:       return EventColor::BLUE;
            case LogCategory::AUDIO:    return EventColor::PURPLE;
            case LogCategory::NETWORK:  return EventColor::PINK;
            case LogCategory::SYSTEM:   return EventColor::LIME;
            case LogCategory::DEBUG:    return EventColor::TEAL;
            default:                    return "";
            }
        }

        std::string CategoryToString(LogCategory cat) const {
            switch (cat) {
            case LogCategory::INPUT:    return "INPUT";
            case LogCategory::GAMEPLAY: return "GAMEPLAY";
            case LogCategory::PHYSICS:  return "PHYSICS";
            case LogCategory::UI:       return "UI";
            case LogCategory::AUDIO:    return "AUDIO";
            case LogCategory::NETWORK:  return "NETWORK";
            case LogCategory::SYSTEM:   return "SYSTEM";
            case LogCategory::DEBUG:    return "DEBUG";
            default:                    return "UNKNOWN";
            }
        }

        template<typename T>
        void SubscribeWithLog(LogCategory category, const std::string& eventName,
            std::function<std::string(const T&)> formatter) {
            eventHandler->GetBus().Subscribe<T>(
                [this, category, eventName, formatter](const T& e) {
                    // Check if logging is enabled
                    if (!loggingEnabled) return;

                    // Check category enabled
                    if (!categoryEnabled[category]) return;

                    // Check blacklist
                    if (blacklistedEvents.count(std::type_index(typeid(T)))) return;

                    // Get color for this category
                    std::string color = GetCategoryColor(category);
                    std::string reset = useColors ? EventColor::RESET : "";

                    // Log with category color
                    std::cout << color
                        << "[" << CategoryToString(category) << "] "
                        << eventName;

                    std::string details = formatter(e);
                    if (!details.empty()) {
                        std::cout << " | " << details;
                    }
                    std::cout << reset << "\n";
                }
            );
        }

        void InitializeEventLogging();
    };

}

#endif // EVENTSYSTEM_H