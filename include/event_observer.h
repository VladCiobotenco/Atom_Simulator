#ifndef OOP_EVENT_OBSERVER_H
#define OOP_EVENT_OBSERVER_H

class event_observer {
public:
    virtual ~event_observer() = default;

    virtual void onItemSelected() = 0;
};


#endif