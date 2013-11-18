#pragma once
class Component
{
public:
    Component(void);
    Component(int _id, bool _active);
    ~Component(void);

    int id;
    bool active;
};

