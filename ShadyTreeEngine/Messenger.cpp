#include "stdafx.h"
#include "Messenger.h"


Messenger::Messenger(void)
{
}


Messenger::~Messenger(void)
{
}

Messenger& Messenger::Instance()
{
    static Messenger msgr;
    return msgr;
}

void Messenger::forwardMessage(Message* m)
{
    if(recievers.find(m->type) != recievers.end())
    {
        for(auto iter = recievers[m->type].begin(); iter != recievers[m->type].end(); ++iter)
        {
            (*iter)(m); //call delegate function
        }
    }
}
