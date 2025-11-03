#include "Config/pch.h"
#include "ECS/ECS.h"
#include "EventSystem.h"
#include "Events/EventHandler.h"

namespace ecs {

    void EventSystem::Init() {
        // Get event handler (no singleton, passed through system)
        eventHandler = Event::EventHandler::GetInstance();

        // Blacklist spammy events by default
        BlacklistEventFromLog<MouseMovedEvent>();

        // Initialize event logging
        InitializeEventLogging();

        std::cout << GetCategoryColor(LogCategory::SYSTEM)
            << "[EventSystem] Initialized with logging:"
            << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::INPUT) << "INPUT (Orange)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::GAMEPLAY) << "GAMEPLAY (Cyan)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::PHYSICS) << "PHYSICS (Magenta)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::UI) << "UI (Blue)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::AUDIO) << "AUDIO (Purple)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::NETWORK) << "NETWORK (Pink)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::SYSTEM) << "SYSTEM (Lime)" << EventColor::RESET << "\n";
        std::cout << GetCategoryColor(LogCategory::DEBUG) << "DEBUG (Teal)" << EventColor::RESET << "\n\n";
    }

    void EventSystem::Update() {
        // Process all queued events this frame
        eventHandler->GetBus().ProcessEvents();
    }

    void EventSystem::InitializeEventLogging() {
        // ========== INPUT EVENTS (ORANGE) ==========
        SubscribeWithLog<KeyPressedEvent>(LogCategory::INPUT, "KeyPressed",
            [](const KeyPressedEvent& e) {
                return "key=" + std::to_string(e.keyCode) +
                    (e.isRepeat ? " (repeat)" : "");
            });

        SubscribeWithLog<KeyReleasedEvent>(LogCategory::INPUT, "KeyReleased",
            [](const KeyReleasedEvent& e) {
                return "key=" + std::to_string(e.keyCode);
            });

        SubscribeWithLog<MouseButtonPressedEvent>(LogCategory::INPUT, "MouseButtonPressed",
            [](const MouseButtonPressedEvent& e) {
                return "btn=" + std::to_string(e.button) +
                    " at(" + std::to_string((int)e.x) + "," + std::to_string((int)e.y) + ")";
            });

        SubscribeWithLog<MouseButtonReleasedEvent>(LogCategory::INPUT, "MouseButtonReleased",
            [](const MouseButtonReleasedEvent& e) {
                return "btn=" + std::to_string(e.button);
            });

        SubscribeWithLog<MouseMovedEvent>(LogCategory::INPUT, "MouseMoved",
            [](const MouseMovedEvent& e) {
                return "pos(" + std::to_string((int)e.x) + "," + std::to_string((int)e.y) + ")";
            });

        SubscribeWithLog<MouseScrolledEvent>(LogCategory::INPUT, "MouseScrolled",
            [](const MouseScrolledEvent& e) {
                return "scroll(" + std::to_string(e.xOffset) + "," + std::to_string(e.yOffset) + ")";
            });

        SubscribeWithLog<WindowResizedEvent>(LogCategory::INPUT, "WindowResized",
            [](const WindowResizedEvent& e) {
                return std::to_string(e.width) + "x" + std::to_string(e.height);
            });

        // ========== GAMEPLAY EVENTS (CYAN) ==========
        // Add your gameplay events here as you create them
        // Example:
        // SubscribeWithLog<PlayerDamagedEvent>(LogCategory::GAMEPLAY, "PlayerDamaged",
        //     [](const PlayerDamagedEvent& e) {
        //         return "player=" + std::to_string(e.playerId) + " dmg=" + std::to_string(e.damage);
        //     });

        // ========== PHYSICS EVENTS (MAGENTA) ==========
        // Example:
        // SubscribeWithLog<CollisionEvent>(LogCategory::PHYSICS, "Collision",
        //     [](const CollisionEvent& e) {
        //         return "A=" + std::to_string(e.entityA) + " B=" + std::to_string(e.entityB);
        //     });

        // ========== UI EVENTS (BLUE) ==========
        // Example:
        // SubscribeWithLog<ButtonClickedEvent>(LogCategory::UI, "ButtonClicked",
        //     [](const ButtonClickedEvent& e) {
        //         return "id=" + e.buttonId;
        //     });

        // ========== AUDIO EVENTS (PURPLE) ==========
        // Example:
        // SubscribeWithLog<SoundPlayedEvent>(LogCategory::AUDIO, "SoundPlayed",
        //     [](const SoundPlayedEvent& e) {
        //         return "sound=" + e.soundName;
        //     });

        // ========== NETWORK EVENTS (PINK) ==========
        // Example:
        // SubscribeWithLog<PlayerConnectedEvent>(LogCategory::NETWORK, "PlayerConnected",
        //     [](const PlayerConnectedEvent& e) {
        //         return "id=" + std::to_string(e.playerId);
        //     });

        // ========== SYSTEM EVENTS (LIME) ==========
        // Example:
        // SubscribeWithLog<AssetLoadedEvent>(LogCategory::SYSTEM, "AssetLoaded",
        //     [](const AssetLoadedEvent& e) {
        //         return "asset=" + e.path;
        //     });
    }

}