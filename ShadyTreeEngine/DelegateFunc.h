#pragma once
#include "Message.h"

//borrowed from The Impossibly Fast C++ Delegates
//author: Sergey Ryazanov

//Delegate for Functions with signature:
// void Object::Func(Message*)
class DelegateFunc
{
public:
    DelegateFunc(): object_ptr(0), stub_ptr(0)
    {}

    template <class T, void (T::*TMethod)(Message*)>
    static DelegateFunc from_method(T* object_ptr)
    {
        DelegateFunc d;
        d.object_ptr = object_ptr;             // store the ref to the object
        d.stub_ptr = &method_stub<T, TMethod>; // reference stub method
        return d;
    }

    
    /// <summary>
    /// Runs the message handler.
    /// </summary>
    /// <param name="a1">The message.</param>
    void operator()(Message* a1) const
    {
        return (*stub_ptr)(object_ptr, a1); //call stub method with object and arg
    }

    bool IsValid()
    {
        if(object_ptr)
            return true;
        return false;
    }

private:
    //type the stub function so it can be referenced.
    typedef void (*stub_type)(void* object_ptr, Message*);

    void* object_ptr;   //store object as void*
    stub_type stub_ptr; //store stub ptr

    //method_stub retypes the object, and calls the method.
    template <class T, void (T::*TMethod)(Message*)>
    static void method_stub(void* object_ptr, Message* a1)
    {
        T* p = static_cast<T*>(object_ptr);
        return (p->*TMethod)(a1); // #2
    }
};
