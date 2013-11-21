#pragma once
class ST_API Component
{
public:
    Component(void);
    Component(int _id, bool _active);
    ~Component(void);

    int id;
    bool active;
};

