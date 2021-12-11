#pragma once
#include "Entity.h"

template <typename T>
class List {
public:
    byte length;
    
    T data[16];

    List();

    T& getItem(byte);
    
    void append(T);
    
    void remove(byte);
};
