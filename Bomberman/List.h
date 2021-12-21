#pragma once
#include "Entity.h"

template <typename T>
class List {
public:
    byte length;
    
    T data[longListLength];

    List();

    T& getItem(byte);
    
    void append(T);
    
    void remove(byte);
};

template <typename T>
class ShortList {
public:
    byte length;
    
    T data[shortListLength];

    ShortList();

    T& getItem(byte);
    
    void append(T);
    
    void remove(byte);
};
