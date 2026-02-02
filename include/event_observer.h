#ifndef OOP_EVENT_OBSERVER_H
#define OOP_EVENT_OBSERVER_H

#include <string>

class event_observer {
public:
    virtual ~event_observer() = default;

    virtual void onEvent(const std::string& eventType) = 0;
};


#endif