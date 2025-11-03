#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace ecs {

    class EventBus {
    private:
        // Base interface for type-erased callbacks
        struct IEventCallback {
            virtual ~IEventCallback() = default;
            virtual void Execute(void* event) = 0;
        };

        // Template wrapper for specific event type callbacks
        template<typename T>
        struct EventCallback : IEventCallback {
            std::function<void(const T&)> callback;

            EventCallback(std::function<void(const T&)> cb) : callback(cb) {}

            void Execute(void* event) override {
                callback(*static_cast<T*>(event));
            }
        };

        // Queued event wrapper
        struct QueuedEvent {
            std::type_index type;
            std::shared_ptr<void> data;

            template<typename T>
            QueuedEvent(std::type_index t, const T& event)
                : type(t), data(std::make_shared<T>(event)) {
            }
        };

        // Map of event type -> list of callbacks
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<IEventCallback>>> subscribers;

        // Queue of events to be processed
        std::queue<QueuedEvent> eventQueue;

    public:
        // Subscribe a callback to an event type
        template<typename T>
        void Subscribe(std::function<void(const T&)> callback) {
            auto typeIndex = std::type_index(typeid(T));
            subscribers[typeIndex].push_back(
                std::make_unique<EventCallback<T>>(callback)
            );
        }

        // Emit an event (adds to queue)
        template<typename T>
        void Emit(const T& event) {
            auto typeIndex = std::type_index(typeid(T));
            eventQueue.emplace(typeIndex, event);
        }

        // Process all queued events
        void ProcessEvents() {
            while (!eventQueue.empty()) {
                auto& queuedEvent = eventQueue.front();

                // Find subscribers for this event type
                auto it = subscribers.find(queuedEvent.type);
                if (it != subscribers.end()) {
                    // Call all subscribers
                    for (auto& callback : it->second) {
                        callback->Execute(queuedEvent.data.get());
                    }
                }

                eventQueue.pop();
            }
        }

        // Clear all queued events without processing
        void Clear() {
            while (!eventQueue.empty()) {
                eventQueue.pop();
            }
        }

        // Get number of queued events
        size_t GetQueueSize() const {
            return eventQueue.size();
        }

        // Check if there are any subscribers for an event type
        template<typename T>
        bool HasSubscribers() const {
            auto typeIndex = std::type_index(typeid(T));
            auto it = subscribers.find(typeIndex);
            return it != subscribers.end() && !it->second.empty();
        }
    };

}

#endif // EVENTBUS_H
