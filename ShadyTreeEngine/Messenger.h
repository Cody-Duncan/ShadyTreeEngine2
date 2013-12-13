#pragma once

#include "Message.h"
#include "DelegateFunc.h"

class Messenger
{
public:
    ST_API static Messenger& Instance();

    ST_API void forwardMessage(Message* m);

    template <class T, void (T::*TMethod)(Message*)>
    void registerHandler(MessageType type, T* object);

private:

    std::unordered_map<MessageType, std::vector<DelegateFunc>> recievers;

    Messenger(void);
    ~Messenger(void);
};


template <class T, void (T::*TMethod)(Message*)>
void Messenger::registerHandler(MessageType type, T* object)
{
    recievers[type].push_back(DelegateFunc::from_method<T, TMethod>(object));
}