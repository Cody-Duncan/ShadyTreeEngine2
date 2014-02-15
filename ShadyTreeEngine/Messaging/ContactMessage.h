#pragma once

#include "Physics\Contact.h"

class ContactMessage : public Message
{
public:
    Contact contact;
    int recieverIndex;
    ContactMessage() : Message(Collision) {}
    ContactMessage(Contact _c, int _recieverIndex) : Message(Collision), contact(_c), recieverIndex(_recieverIndex) {}
};