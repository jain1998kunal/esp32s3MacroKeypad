#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <functional>
#include <vector>

class Dispatcher {
public:
    using Listener = std::function<void(uint8_t)>;

    void registerListener(Listener cb) {
        listeners.push_back(cb);
    }

    void dispatch(uint8_t index) {
        for (auto& listener : listeners) {
            listener(index);
        }
    }

private:
    std::vector<Listener> listeners;
};

#endif
