/******************************************************************/
/*!
\file      Delegate.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief     Deletegate class for event handling with subscription and invocation capabilities.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#pragma once

#include "Config/pch.h"

template<typename... Args>
class Delegate {
public:
    using Func = std::function<void(Args...)>;
    using ID = std::uint64_t;

    // Subscribe and return an ID (to unsubscribe later)
    ID Add(Func f) {
        ID id = nextId++;
        callbacks[id] = std::move(f);
        return id;
    }

    // Remove by ID
    void Remove(ID id) {
        callbacks.erase(id);
    }

    // Trigger all callbacks
    void Invoke(Args... args) {
        for (auto& [id, cb] : callbacks) {
            cb(args...);
        }
    }

    // Clear all
    void Clear() {
        callbacks.clear();
    }

private:
    std::unordered_map<ID, Func> callbacks;
    ID nextId = 1;
};
